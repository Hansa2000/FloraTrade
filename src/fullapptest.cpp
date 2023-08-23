#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Order {
    string orderID;
    string clientOrderID;
    string instrument;
    string side;
    double price;
    int quantity;
};

struct ExecutionRecord {
    string orderID;
    string clientOrderID;
    string instrument;
    string side;
    string execStatus;
    int quantity;
    double price;
    string reason;
};

struct BuyComparator {
    bool operator()(const Order& a, const Order& b) const {
        return a.price < b.price; // Highest price has highest priority
    }
};

struct SellComparator {
    bool operator()(const Order& a, const Order& b) const {
        return a.price > b.price; // Least price has highest priority
    }
};

string generateOrderID() {
    static int orderCounter = 0;
    ++orderCounter;
    return "ORDER" + to_string(orderCounter);
}

int main() {
    vector<Order> orders;
    
    ifstream inputFile("order.csv");
    if (!inputFile.is_open()) {
        cerr << "Failed to open the input CSV file." << endl;
        return 1;
    }

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

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 5) {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        Order order;
        order.orderID = generateOrderID(); // Generate unique order ID
        order.clientOrderID = tokens[0];
        order.instrument = tokens[1];
        order.side = tokens[2];
        order.price = stod(tokens[3]);
        order.quantity = stoi(tokens[4]);

        orders.push_back(order);
    }

    inputFile.close();

    priority_queue<Order, vector<Order>, BuyComparator> buyQueue;
    priority_queue<Order, vector<Order>, SellComparator> sellQueue;

    for (const Order& order : orders) {
        if (order.side == "buy") {
            buyQueue.push(order);
        } else if (order.side == "sell") {
            sellQueue.push(order);
        }
    }

    vector<ExecutionRecord> executionRecords;

    while (!sellQueue.empty() && !buyQueue.empty()) {
        Order sellOrder = sellQueue.top();
        Order buyOrder = buyQueue.top();

        ExecutionRecord record;
        record.orderID = buyOrder.orderID;
        record.clientOrderID = buyOrder.clientOrderID;
        record.instrument = buyOrder.instrument;
        record.side = "buy";

        if (sellOrder.price <= buyOrder.price) {
            record.execStatus = "fill";
            record.quantity = min(buyOrder.quantity, sellOrder.quantity);
            record.price = buyOrder.price;
            sellOrder.quantity -= record.quantity;
            buyOrder.quantity -= record.quantity;

            if (sellOrder.quantity == 0) {
                sellQueue.pop();
            }

            if (buyOrder.quantity == 0) {
                buyQueue.pop();
            }
        } else {
            record.execStatus = "ejected";
            record.quantity = 0;
            record.price = 0;
            record.reason = "Sell price is higher than buy price";
            sellQueue.pop();
        }

        executionRecords.push_back(record);
    }

    while (!sellQueue.empty()) {
        Order sellOrder = sellQueue.top();
        ExecutionRecord record;
        record.orderID = sellOrder.orderID;
        record.clientOrderID = sellOrder.clientOrderID;
        record.instrument = sellOrder.instrument;
        record.side = "sell";
        record.execStatus = "ejected";
        record.quantity = 0;
        record.price = 0;
        record.reason = "No matching buy orders";
        sellQueue.pop();
        executionRecords.push_back(record);
    }

    while (!buyQueue.empty()) {
        Order buyOrder = buyQueue.top();
        ExecutionRecord record;
        record.orderID = buyOrder.orderID;
        record.clientOrderID = buyOrder.clientOrderID;
        record.instrument = buyOrder.instrument;
        record.side = "buy";
        record.execStatus = "ejected";
        record.quantity = 0;
        record.price = 0;
        record.reason = "No matching sell orders";
        buyQueue.pop();
        executionRecords.push_back(record);
    }

    ofstream outputFile("execution_rep.csv");
    if (outputFile.is_open()) {
        outputFile << "Order ID,Cl. Ord. ID,Instrument,Side,Exec Status,Quantity,Price,Reason" << endl;
        for (const ExecutionRecord& record : executionRecords) {
            outputFile << record.orderID << ","
                       << record.clientOrderID << ","
                       << record.instrument << ","
                       << record.side << ","
                       << record.execStatus << ","
                       << record.quantity << ","
                       << record.price << ","
                       << record.reason << endl;
        }
        outputFile.close();
        cout << "Execution report has been generated." << endl;
    } else {
        cerr << "Failed to open output CSV file." << endl;
    }

    return 0;
}
