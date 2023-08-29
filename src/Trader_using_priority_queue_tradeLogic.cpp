#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
using namespace std;

struct Order {
    string orderID;
    string clientID;
    string instrument;
    string side;
    double price;
    int quantity;
};

struct TradeDetails {
    string orderID;
    string clientID;
    string instrument;
    string side;
    string execStatus;
    int quantity;
    double price;
    double duration;
};

// Comparator for the buy queue
struct BuyComparator {
    bool operator()(const Order& a, const Order& b) const {
        return a.price < b.price; // Highest price has highest priority
    }
};

// Comparator for the sell queue
struct SellComparator {
    bool operator()(const Order& a, const Order& b) const {
        return a.price > b.price; // Least price has highest priority
    }
};

// writing report to csv
void writeTradesToCSV(const vector<TradeDetails>& trades, const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "Failed to open the output CSV file." << endl;
        return;
    }

    // Write CSV header
    outputFile << "Order ID,Client ID,Instrument,Side,Exec Status,Quantity,Price,Duration" << endl;

    // Write trade details
    for (const TradeDetails& trade : trades) {
        outputFile << trade.orderID << ","
                   << trade.clientID << ","
                   << trade.instrument << ","
                   << trade.side << ","
                   << trade.execStatus << ","
                   << trade.quantity << ","
                   << trade.price << ","
                   << trade.duration << endl; 
    }

    outputFile.close();
    cout << "Trades written to " << filename << " successfully." << endl;
}


int main() {
    //start measuring time
    auto start = chrono::high_resolution_clock::now();

    //string file_name = "order.csv";               //for testing logic errors
    //string file_name = "order_million.csv";       //for testing timing
    string file_name = "order_mem_overload.csv";    //for testing memory handling

    ifstream inputFile(file_name);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return 1;
    }

    vector<Order> orders;
    string line;
    bool firstLine = true;

    while (getline(inputFile, line)) {
        if (firstLine) {
            firstLine = false;
            continue; // Skip header line
        }

        istringstream ss(line);
        string token;
        vector<string> tokens;
        static int orderCounter = 0;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 5) {
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

        
    }
    //time to read csv file
    auto end = chrono::high_resolution_clock::now();
    auto read_csv_duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    

    // Now 'orders' vector contains all the orders from the CSV file
    // You can process them further or print them if needed
    
    inputFile.close();

    priority_queue<Order, vector<Order>, BuyComparator> buyQueue;
    priority_queue<Order, vector<Order>, SellComparator> sellQueue;
    vector<TradeDetails> trades;

    auto process_start = chrono::high_resolution_clock::now();
    for (const Order& order : orders) {

        // Record trade details
            TradeDetails trade;
            trade.orderID = order.orderID;
            trade.clientID = order.clientID;
            trade.instrument = order.instrument;
            trade.side =  order.side;
            trade.execStatus = "New";
            trade.quantity =  order.quantity;
            trade.price = order.price;

        if (order.side == "1") {
            buyQueue.push(order);
        } else if (order.side == "2") {
            sellQueue.push(order);
        }

        //trading
        if (!(!buyQueue.empty() && !sellQueue.empty() && buyQueue.top().price >= sellQueue.top().price)) trades.push_back(trade);
        else
            while (!buyQueue.empty() && !sellQueue.empty() && buyQueue.top().price >= sellQueue.top().price) {
                // Execute trade logic

                //pop orders from queues
                Order buyOrder = buyQueue.top();
                Order sellOrder = sellQueue.top();
                buyQueue.pop();
                sellQueue.pop();

                // Update quantities, execute price, etc.
                
                if (buyOrder.quantity < sellOrder.quantity)
                {
                    trade.quantity = buyOrder.quantity;
                    sellOrder.quantity -= trade.quantity;
                    sellQueue.push(sellOrder);

                        if(trade.orderID==sellOrder.orderID){
                            TradeDetails trade_;
                            trade_.orderID = buyOrder.orderID;
                            trade_.clientID = buyOrder.clientID;
                            trade_.instrument = buyOrder.instrument;
                            trade_.side =  buyOrder.side;
                            trade_.execStatus = "Fill";
                            trade_.quantity =  trade.quantity;
                            trade_.price = trade.price = buyOrder.price;
                            auto end = chrono::high_resolution_clock::now();
                            trade_.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                            trades.push_back(trade_);
                            trade.execStatus = "pFill";
                        }
                        else{
                            TradeDetails trade_;
                            trade_.orderID = sellOrder.orderID;
                            trade_.clientID = sellOrder.clientID;
                            trade_.instrument = sellOrder.instrument;
                            trade_.side =  sellOrder.side;
                            trade_.execStatus = "Fill";
                            trade_.quantity =  trade.quantity;
                            trade_.price = trade.price = sellOrder.price;
                            auto end = chrono::high_resolution_clock::now();
                            trade_.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                            trades.push_back(trade_);
                            trade.execStatus = "Fill";
                        }

                    }
                else if (buyOrder.quantity > sellOrder.quantity)
                    {
                    trade.quantity = sellOrder.quantity;
                    trade.execStatus = "pFill";
                    buyOrder.quantity -= sellOrder.quantity;
                    buyQueue.push(buyOrder);

                    if(trade.orderID==sellOrder.orderID){
                            TradeDetails trade_;
                            trade_.orderID = buyOrder.orderID;
                            trade_.clientID = buyOrder.clientID;
                            trade_.instrument = buyOrder.instrument;
                            trade_.side =  buyOrder.side;
                            trade_.execStatus = "pFill";
                            trade_.quantity =  trade.quantity;
                            trade_.price = trade.price = buyOrder.price;
                            auto end = chrono::high_resolution_clock::now();
                            trade_.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                            trades.push_back(trade_);
                            trade.execStatus = "Fill";
                        }
                        else{
                            TradeDetails trade_;
                            trade_.orderID = sellOrder.orderID;
                            trade_.clientID = sellOrder.clientID;
                            trade_.instrument = sellOrder.instrument;
                            trade_.side =  sellOrder.side;
                            trade_.execStatus = "Fill";
                            trade_.quantity =  trade.quantity;
                            trade_.price = trade.price = sellOrder.price;
                            auto end = chrono::high_resolution_clock::now();
                            trade_.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                            trades.push_back(trade_);
                            trade.execStatus = "pFill";
                        }
                    }
                else 
                    {
                        if(trade.orderID==sellOrder.orderID){
                            TradeDetails trade_;
                            trade_.orderID = buyOrder.orderID;
                            trade_.clientID = buyOrder.clientID;
                            trade_.instrument = buyOrder.instrument;
                            trade_.side =  buyOrder.side;
                            trade_.execStatus = "Fill";
                            trade_.quantity =  trade.quantity;
                            trade_.price = trade.price = buyOrder.price;
                            auto end = chrono::high_resolution_clock::now();
                            trade_.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                            trades.push_back(trade_);
                        }
                        else{
                            TradeDetails trade_;
                            trade_.orderID = sellOrder.orderID;
                            trade_.clientID = sellOrder.clientID;
                            trade_.instrument = sellOrder.instrument;
                            trade_.side =  sellOrder.side;
                            trade_.execStatus = "Fill";
                            trade_.quantity =  trade.quantity;
                            trade_.price = trade.price = sellOrder.price;
                            auto end = chrono::high_resolution_clock::now();
                            trade_.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                            trades.push_back(trade_);
                        }
                        trade.execStatus = "Fill";
                    }
                
            
                    // Store trade details in a container or log file
                    auto end = chrono::high_resolution_clock::now();
                    trade.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                    trades.push_back(trade);
            
            }
        
    }

    //time to process trades
    auto process_end = chrono::high_resolution_clock::now();
    auto process_csv_duration = chrono::duration_cast<chrono::microseconds>(process_end - process_start).count();

    // Output csv file
    auto write_start = chrono::high_resolution_clock::now();
    writeTradesToCSV(trades, "executed_trades.csv");
    //time to read csv file
    auto write_end = chrono::high_resolution_clock::now();
    auto write_csv_duration = chrono::duration_cast<chrono::microseconds>(write_end - write_start).count();

    // Print the buy queue
    cout << "Buy Queue (Highest Price First):" << endl;
    while (!buyQueue.empty()) {
        cout << "Price: " << buyQueue.top().price << "\tQuantity: " << buyQueue.top().quantity << endl;
        buyQueue.pop();
    }

    // Print the sell queue
    cout << "Sell Queue (Least Price First):" << endl;
    while (!sellQueue.empty()) {
        cout << "Price: " << sellQueue.top().price << "\tQuantity: " << sellQueue.top().quantity << endl;
        sellQueue.pop();
    }

    //execution report
    cout << "_________________________________________________________________" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "\t\tEXECUTION REPORT" << endl;
    cout << "_________________________________________________________________" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    
    cout << "  Time to read csv file\t\t | " << read_csv_duration<< " time units.\n";
    cout << "  Time to write csv file\t | " << write_csv_duration<< " time units.\n";
    cout << "  Time to process csv file\t | " << process_csv_duration<< " time units.\n";
  

    return 0;
}
