#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Order {
    string clientOrderID;
    string instrument;
    string side;
    double price;
    int quantity;
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

int main() {
    ifstream inputFile("order.csv");
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

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 5) {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }

        Order order;
        order.clientOrderID = tokens[0];
        order.instrument = tokens[1];
        order.side = tokens[2];
        order.price = stod(tokens[3]);
        order.quantity = stoi(tokens[4]);

        orders.push_back(order);

        
    }

    // Now 'orders' vector contains all the orders from the CSV file
    // You can process them further or print them if needed
    
    inputFile.close();

    priority_queue<Order, vector<Order>, BuyComparator> buyQueue;
    priority_queue<Order, vector<Order>, SellComparator> sellQueue;

    for (const Order& order : orders) {
        if (order.side == "2") {
            buyQueue.push(order);
        } else if (order.side == "1") {
            sellQueue.push(order);
        }
        //trading
        
    }

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

    return 0;
}
