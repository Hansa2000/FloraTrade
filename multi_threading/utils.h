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
void writeTradesToCSV(const vector<TradeDetails>& trades, ofstream* outputFile) {
    if (!outputFile->is_open()) {
        cerr << "Failed to open the output CSV file." << endl;
        return;
    }

    // Write trade details
    for (const TradeDetails& trade : trades) {
        *outputFile << trade.orderID << ","
                   << trade.clientID << ","
                   << trade.instrument << ","
                   << trade.side << ","
                   << trade.execStatus << ","
                   << trade.quantity << ","
                   << trade.price << ","
                   << trade.duration << endl; 
    }
    cout << "Part of trades written to successfully." << endl;
}


//execution report
void exe_report(int read_csv_duration,int write_csv_duration,int process_csv_duration){
    cout << "_________________________________________________________________" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "\t\tEXECUTION REPORT" << endl;
    cout << "_________________________________________________________________" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    
    cout << "  Time to read csv file\t\t | " << read_csv_duration/1000000.0<< " seconds.\n";
    cout << "  Time to write csv file\t | " << write_csv_duration/1000000.0<< " seconds.\n";
    cout << "  Time to process csv file\t | " << process_csv_duration/1000000.0<< " seconds.\n";
}