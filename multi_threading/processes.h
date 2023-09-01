void read_csv (vector<Order>* orders,int* orderCounter,ifstream* inputFile,int breakCount) {
    orders->clear();
    int counter = 0;
    string line;
    while (getline( *inputFile , line)) {
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

        *orderCounter = *orderCounter+1;

        Order order;
        order.orderID = "Ord" + to_string(*orderCounter);
        order.clientID = tokens[0];
        order.instrument = tokens[1];
        order.side = tokens[2];
        order.quantity = stod(tokens[3]);
        order.price = stoi(tokens[4]);

        orders->push_back(order);
        counter++;
        if(counter==breakCount) break;
        
    }

}

void process(
    Order order,
    std::vector<TradeDetails>* trades,
    priority_queue<Order, vector<Order>, BuyComparator>* buyQueue,
    priority_queue<Order, vector<Order>, SellComparator>* sellQueue
) 
{
        auto start = std::chrono::high_resolution_clock::now();

        // Record trade details
        TradeDetails trade;
        trade.orderID = order.orderID;
        trade.clientID = order.clientID;
        trade.instrument = order.instrument;
        trade.side = order.side;
        trade.execStatus = "New";
        trade.quantity = order.quantity;
        trade.price = order.price;
        auto end = std::chrono::high_resolution_clock::now();
        trade.duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        if (order.side == "1") {
            buyQueue->push(order);
        } else if (order.side == "2") {
            sellQueue->push(order);
        }

        //trading
            if (!(!buyQueue->empty() && !sellQueue->empty() && buyQueue->top().price >= sellQueue->top().price)) trades->push_back(trade);
            else
                while (!buyQueue->empty() && !sellQueue->empty() && buyQueue->top().price >= sellQueue->top().price) {
                    // Execute trade logic

                    //pop orders from queues
                    Order buyOrder = buyQueue->top();
                    Order sellOrder = sellQueue->top();
                    buyQueue->pop();
                    sellQueue->pop();

                    // Update quantities, execute price, etc.
                    
                    if (buyOrder.quantity < sellOrder.quantity)
                    {
                        trade.quantity = buyOrder.quantity;
                        sellOrder.quantity -= trade.quantity;
                        sellQueue->push(sellOrder);

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
                                trades->push_back(trade_);
                                trade.execStatus = "pFill";
                            }
                            else{
                                TradeDetails trade_;
                                trade_.orderID = sellOrder.orderID;
                                trade_.clientID = sellOrder.clientID;
                                trade_.instrument = sellOrder.instrument;
                                trade_.side =  sellOrder.side;
                                trade_.execStatus = "pFill";
                                trade_.quantity =  trade.quantity;
                                trade_.price = trade.price = sellOrder.price;
                                auto end = chrono::high_resolution_clock::now();
                                trade_.duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
                                trades->push_back(trade_);
                                trade.execStatus = "Fill";
                            }

                        }
                    else if (buyOrder.quantity > sellOrder.quantity)
                        {
                        trade.quantity = sellOrder.quantity;
                        trade.execStatus = "pFill";
                        buyOrder.quantity -= sellOrder.quantity;
                        buyQueue->push(buyOrder);

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
                                trades->push_back(trade_);
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
                                trades->push_back(trade_);
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
                                trades->push_back(trade_);
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
                                trades->push_back(trade_);
                            }
                            trade.execStatus = "Fill";
                        }
                    
                
                        // Store trade details in a container or log file
                        auto process_end = chrono::high_resolution_clock::now();
        		trade.duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        		trades->push_back(trade);
    }

}


void process_merged(vector<Order> orders,
            vector<TradeDetails>* trades,
            priority_queue<Order, vector<Order>, BuyComparator>* RoseBuyQueue,
            priority_queue<Order, vector<Order>, SellComparator>* RoseSellQueue,
            priority_queue<Order, vector<Order>, BuyComparator>* LavenderBuyQueue,
            priority_queue<Order, vector<Order>, SellComparator>* LavenderSellQueue,
            priority_queue<Order, vector<Order>, BuyComparator>* LotusBuyQueue,
            priority_queue<Order, vector<Order>, SellComparator>* LotusSellQueue,
            priority_queue<Order, vector<Order>, BuyComparator>* TulipBuyQueue,
            priority_queue<Order, vector<Order>, SellComparator>* TulipSellQueue,
            priority_queue<Order, vector<Order>, BuyComparator>* OrchidBuyQueue,
            priority_queue<Order, vector<Order>, SellComparator>* OrchidSellQueue
            )
{
        trades->clear();
        for (const Order& order : orders) {
            if (order.instrument == "Rose") process(order,trades,RoseBuyQueue,RoseSellQueue);
            if (order.instrument == "Lavender")  process(order,trades,LavenderBuyQueue,LavenderSellQueue);
            if (order.instrument == "Lotus")  process(order,trades,LotusBuyQueue,LotusSellQueue);
            if (order.instrument == "Tulip")  process(order,trades,TulipBuyQueue,TulipSellQueue);
            if (order.instrument == "Orchid")  process(order,trades,OrchidBuyQueue,OrchidSellQueue); 
        }
}
