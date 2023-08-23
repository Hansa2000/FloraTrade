#include <iostream>
#include <vector>
#include <queue>

struct Order {
    std::string clientOrderID;
    std::string instrument;
    std::string side;
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
    std::vector<Order> orders = {
        {"1", "FlowerA", "buy", 10, 100},
        {"2", "FlowerB", "sell", 15, 50},
        {"3", "FlowerA", "sell", 12, 75}
    };

    std::priority_queue<Order, std::vector<Order>, BuyComparator> buyQueue;
    std::priority_queue<Order, std::vector<Order>, SellComparator> sellQueue;

    for (const Order& order : orders) {
        if (order.side == "buy") {
            buyQueue.push(order);
        } else if (order.side == "sell") {
            sellQueue.push(order);
        }
    }

    // Print the buy queue
    std::cout << "Buy Queue (Highest Price First):" << std::endl;
    while (!buyQueue.empty()) {
        std::cout << "Price: " << buyQueue.top().price << "\tQuantity: " << buyQueue.top().quantity << std::endl;
        buyQueue.pop();
    }

    // Print the sell queue
    std::cout << "Sell Queue (Least Price First):" << std::endl;
    while (!sellQueue.empty()) {
        std::cout << "Price: " << sellQueue.top().price << "\tQuantity: " << sellQueue.top().quantity << std::endl;
        sellQueue.pop();
    }

    return 0;
}
