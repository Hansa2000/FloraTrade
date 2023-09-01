#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

#include "utils.h"
#include "processes.h"

int main() {
    //start measuring time
    auto start = chrono::high_resolution_clock::now();

    // string file_name = "order.csv";               //for testing logic errors
    // string file_name = "order_million.csv";       //for testing timing
    //string file_name = "order_mem_overload.csv";    //for testing memory handling
    // string file_name = "order_multiinstrument.csv";    //for testing different instrument handling
    string file_name = "order_10000000.csv";

    ifstream inputFile(file_name);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return 1;
    }

    vector<Order> orders;
    string line;
    bool firstLine = true;
    
    int counter = 0;
    int breakCount = 100000;
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

    ofstream outputFile("executed_trades_para.csv",ios::app);
    // Write CSV header
    outputFile << "Order ID,Client ID,Instrument,Side,Exec Status,Quantity,Price,Duration" << endl;
    
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
        order.clientID = tokens[0];
        order.instrument = tokens[1];
        order.side = tokens[2];
        order.quantity = stod(tokens[3]);
        order.price = stoi(tokens[4]);

        orders.push_back(order);
        counter++;
        if(counter==breakCount) break;
        
    }

    process_merged(orders,&trades,
            &RoseBuyQueue,&RoseSellQueue,
            &LavenderBuyQueue,&LavenderSellQueue,
            &LotusBuyQueue,&LotusSellQueue,
            &TulipBuyQueue,&TulipSellQueue,
            &OrchidBuyQueue,&OrchidSellQueue);
    orders.clear();

    //parallel reading
    while(!inputFile.eof()){
    thread in(read_csv,&orders,&orderCounter,&inputFile,breakCount);
    thread process(process_merged,orders,&trades,
            &RoseBuyQueue,&RoseSellQueue,
            &LavenderBuyQueue,&LavenderSellQueue,
            &LotusBuyQueue,&LotusSellQueue,
            &TulipBuyQueue,&TulipSellQueue,
            &OrchidBuyQueue,&OrchidSellQueue);
    thread out(writeTradesToCSV,trades, &outputFile);

    in.join();process.join();out.join();

    }
    
    thread process(process_merged,orders,&trades,
            &RoseBuyQueue,&RoseSellQueue,
            &LavenderBuyQueue,&LavenderSellQueue,
            &LotusBuyQueue,&LotusSellQueue,
            &TulipBuyQueue,&TulipSellQueue,
            &OrchidBuyQueue,&OrchidSellQueue);
    thread out(writeTradesToCSV,trades, &outputFile);
    process.join();out.join();
    // Output csv file
    writeTradesToCSV(trades, &outputFile);
    outputFile.close();
    

    //exe_report(read_csv_duration,write_csv_duration,process_csv_duration);
    inputFile.close();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

    cout <<"Total time it took was : "<< duration/1000000.0 << endl;


    return 0;
}
