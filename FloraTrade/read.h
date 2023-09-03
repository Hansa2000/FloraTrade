void read_csv(vector<Order>* orders, int* orderCounter, ifstream* inputFile, int breakCount) {
    orders->clear();
    int counter = 0;
    string line;
    while (getline(*inputFile, line)) {
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

        *orderCounter = *orderCounter + 1;

        Order order;
        // order.orderID = "Ord" + to_string(*orderCounter);
        order.orderID = to_string(*orderCounter);

        // Validate and set clientID
        if (tokens[0].length() <= 7) {
            for (char c : tokens[0]) {
            if (!std::isalnum(c)) { order.reason = "Invalid client ID: " + tokens[0];break;}}
            order.clientID = tokens[0];
        } else {
            order.reason = "Invalid client ID: " + tokens[0];
            orders->push_back(order);
            continue; // Skip this order
        }

        // Validate and set instrument
        if (tokens[1] == "Rose" || tokens[1] == "Lavender" || tokens[1] == "Lotus" || tokens[1] == "Tulip" || tokens[1] == "Orchid") {
            order.instrument = tokens[1];
        } else {
            order.reason = "Invalid instrument: " + tokens[1];
            orders->push_back(order);
            continue; // Skip this order
        }

        // Validate and set side
        if (tokens[2] == "1" || tokens[2] == "2") {
            order.side = tokens[2];
        } else {
            order.reason = "Invalid side: " + tokens[2];
            orders->push_back(order);
            continue; // Skip this order
        }

        // Validate and set quantity
        try {
            int quantity = stoi(tokens[3]);
            if (quantity >= 10 && quantity <= 1000 && quantity % 10 == 0) {
                order.quantity = quantity;
            } else {
                order.reason = "Invalid quantity: " + tokens[3];
                orders->push_back(order);
                continue; // Skip this order
            }
        } catch (const exception& e) {
            order.reason = "Invalid quantity: " + tokens[3];
            orders->push_back(order);
            continue; // Skip this order
        }

        // Validate and set price
        try {
            double price = stod(tokens[4]);
            if (price > 0) {
                order.price = price;
            } else {
                order.reason = "Invalid price: " + tokens[4];
                orders->push_back(order);
                continue; // Skip this order
            }
        } catch (const exception& e) {
            order.reason = "Invalid price: " + tokens[4];
            orders->push_back(order);
            continue; // Skip this order
        }

        orders->push_back(order);
        counter++;
        if (counter == breakCount) break;
    }
}