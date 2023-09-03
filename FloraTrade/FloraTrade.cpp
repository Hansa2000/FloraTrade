#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <cctype>
using namespace std;

#include "time.h"
#include "utils.h"
#include "read.h"
#include "processes.h"


int main() {
    vector<string> fileOptions = {
        "order.csv",
        "order_million.csv",
        "order_mem_overload.csv",
        "order_multiinstrument.csv",
        "order_10000.csv",
        "order_100000.csv",
        "order_1000000.csv",
        "order_2000000.csv",
        "order_5000000.csv",
        "order_9000000.csv"
    };

    cout << "Available input files:" << endl;
    for (int i = 0; i < fileOptions.size(); ++i) {
        cout << i + 1 << ". " << fileOptions[i] << endl;
    }
    cout << fileOptions.size() + 1 << ". Enter custom file name" << endl;

    int selectedOption;
    do {
        cout << "Select an option (1-" << fileOptions.size() + 1 << "): ";
        cin >> selectedOption;
    } while (selectedOption < 1 || selectedOption > fileOptions.size() + 1);

    string file_name;
    if (selectedOption <= fileOptions.size()) {
        file_name = fileOptions[selectedOption - 1];
    } else {
        cout << "Enter the custom input file name: ";
        cin >> file_name;
    }

    ifstream inputFile(file_name);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return 1;
    }

    //configuring output file
    string out_file = "executed_trades_para.csv" ;
    string out_file_;
    cout << "Enter output file name : " ;
    cin >> out_file_ ;
    if(out_file_!="") out_file = out_file_;
    ofstream outputFile(out_file,ios::app);
    // Write CSV header
    outputFile << "Order ID,Client ID,Instrument,Side,Exec Status,Quantity,Price,Exec.Time,Reason" << endl;

    //start measuring time
    auto start = chrono::high_resolution_clock::now();
    std::chrono::steady_clock::time_point programStartTime = std::chrono::steady_clock::now();
    std::tm time_info = getInitialTimeInfo(); 

    //intializing containers and variables
    vector<Order> orders;
    string line;
    bool firstLine = true;
    
    int counter = 0;
    int breakCount = 10000;
    static int orderCounter = 0;

    priority_queue<Order, vector<Order>, BuyComparator> RoseBuyQueue; 
    priority_queue<Order, vector<Order>, SellComparator> RoseSellQueue;
    priority_queue<Order, vector<Order>, BuyComparator> LavenderBuyQueue;
    priority_queue<Order, vector<Order>, SellComparator> LavenderSellQueue;
    priority_queue<Order, vector<Order>, BuyComparator> LotusBuyQueue;
    priority_queue<Order, vector<Order>, SellComparator> LotusSellQueue;
    priority_queue<Order, vector<Order>, BuyComparator> TulipBuyQueue;
    priority_queue<Order, vector<Order>, SellComparator> TulipSellQueue;
    priority_queue<Order, vector<Order>, BuyComparator> OrchidBuyQueue;
    priority_queue<Order, vector<Order>, SellComparator> OrchidSellQueue;
    vector<TradeDetails> trades;
    
    //initial reading
    while (getline(inputFile, line)) {
        if (firstLine) {
            firstLine = false;
            continue; // Skip header line
        }

        istringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 5 && !(inputFile.eof())) {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        ++orderCounter;

        Order order;
        order.orderID = "Ord" + to_string(orderCounter);


        // Validate and set clientID
        if (tokens[0].length() <= 7) {
            for (char c : tokens[0]) {
            if (!std::isalnum(c)) { order.reason = "Invalid client ID: " + tokens[0];break;}}
            order.clientID = tokens[0];
        } else {
            order.reason = "Invalid client ID: " + tokens[0];
            orders.push_back(order);
            continue; // Skip this order
        }

        // Validate and set instrument
        if (tokens[1] == "Rose" || tokens[1] == "Lavender" || tokens[1] == "Lotus" || tokens[1] == "Tulip" || tokens[1] == "Orchid") {
            order.instrument = tokens[1];
        } else {
            order.reason = "Invalid instrument: " + tokens[1];
            orders.push_back(order);
            continue; // Skip this order
        }

        // Validate and set side
        if (tokens[2] == "1" || tokens[2] == "2") {
            order.side = tokens[2];
        } else {
            order.reason = "Invalid side: " + tokens[2];
            orders.push_back(order);
            continue; // Skip this order
        }

        // Validate and set quantity
        try {
            int quantity = stoi(tokens[3]);
            if (quantity >= 10 && quantity <= 1000 && quantity % 10 == 0) {
                order.quantity = quantity;
            } else {
                order.reason = "Invalid quantity: " + tokens[3];
                orders.push_back(order);
                continue; // Skip this order
            }
        } catch (const exception& e) {
            order.reason = "Invalid quantity: " + tokens[3];
            orders.push_back(order);
            continue; // Skip this order
        }

        // Validate and set price
        try {
            double price = stod(tokens[4]);
            if (price > 0) {
                order.price = price;
            } else {
                order.reason = "Invalid price: " + tokens[4];
                orders.push_back(order);
                continue; // Skip this order
            }
        } catch (const exception& e) {
            order.reason = "Invalid price: " + tokens[4];
            orders.push_back(order);
            continue; // Skip this order
        }

        orders.push_back(order);
        counter++;
        if(counter==breakCount) break;
        
    }

    process_merged(orders,&trades,
            &RoseBuyQueue,&RoseSellQueue,
            &LavenderBuyQueue,&LavenderSellQueue,
            &LotusBuyQueue,&LotusSellQueue,
            &TulipBuyQueue,&TulipSellQueue,
            &OrchidBuyQueue,&OrchidSellQueue,
            &programStartTime);
    orders.clear();

    //parallel reading
    while(!inputFile.eof()){
    thread in(read_csv,&orders,&orderCounter,&inputFile,breakCount);
    thread process(process_merged,orders,&trades,
            &RoseBuyQueue,&RoseSellQueue,
            &LavenderBuyQueue,&LavenderSellQueue,
            &LotusBuyQueue,&LotusSellQueue,
            &TulipBuyQueue,&TulipSellQueue,
            &OrchidBuyQueue,&OrchidSellQueue,&programStartTime);
    thread out(writeTradesToCSV,trades, &outputFile, &time_info);

    in.join();process.join();out.join();

    }
    
    thread process(process_merged,orders,&trades,
            &RoseBuyQueue,&RoseSellQueue,
            &LavenderBuyQueue,&LavenderSellQueue,
            &LotusBuyQueue,&LotusSellQueue,
            &TulipBuyQueue,&TulipSellQueue,
            &OrchidBuyQueue,&OrchidSellQueue,&programStartTime);
    thread out(writeTradesToCSV,trades, &outputFile, &time_info);
    process.join();out.join();
    // Output csv file
    writeTradesToCSV(trades, &outputFile, &time_info);
    outputFile.close();
    

    //exe_report(read_csv_duration,write_csv_duration,process_csv_duration);
    inputFile.close();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout <<"Total time it took was : "<< duration/1000000.0 << endl;


    return 0;
}
