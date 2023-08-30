#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;

class trade {
    
struct Order {
    string orderID;
    string clientID;
    string instrument;
    string side;
    string status;
    double price;
    int quantity;
};

int trade() {
    //start measuring time
    auto start = chrono::high_resolution_clock::now();

    //string file_name = "order.csv";               //for testing logic errors
    string file_name = "order_million.csv";       //for testing timing
    //string file_name = "order_mem_overload.csv";    //for testing memory handling

    ifstream inputFile(file_name);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return 1;
    }

    vector<Order> orders;
    int count;
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

        ++orderCounter;
        Order order;
        order.orderID = "Ord" + to_string(orderCounter);
        order.status = "valid";
        order.clientID = tokens[0];
        order.instrument = tokens[1];
        order.side = tokens[2];
        order.quantity = stod(tokens[3]);
        order.price = stoi(tokens[4]);

        // Input Validation
        if (order.clientID.empty()) {
            order.status = "Invalid Client ID";
            continue;
        }
        if (order.instrument.empty()) {
            order.status = "Invalid instrument";
            continue;
        }
        if (order.side.empty() || order.side != "1" ){
            order.status = "Invalid side";
            continue;
        }

        orders.push_back(order);
    
    }
    //time to read csv file
    auto end = chrono::high_resolution_clock::now();
    auto read_csv_duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    // Now 'orders' vector contains all the orders from the CSV file
    // You can process them further or print them if needed
    
    inputFile.close();
}

}