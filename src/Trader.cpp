#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct Order {
    string clientOrderID;
    string instrument;
    string side;
    double price;
    int quantity;
};

void visualizeOrders(const vector<Order>& orders) {
    std::cout << "Client Order ID\tInstrument\tSide\tPrice\tQuantity" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    
    for (const Order& order : orders) {
        std::cout << order.clientOrderID << "\t\t"
                  << order.instrument << "\t\t"
                  << order.side << "\t"
                  << order.price << "\t"
                  << order.quantity << std::endl;
    }
}

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

    //just for visualize
    visualizeOrders(orders);
    return 0;
}
