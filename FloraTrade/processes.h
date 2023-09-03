// handling single order book - (single instrument)
void process(
    Order order,
    std::vector<TradeDetails>* trades,
    priority_queue<Order, vector<Order>, BuyComparator>* buyQueue,
    priority_queue<Order, vector<Order>, SellComparator>* sellQueue,
    std::chrono::steady_clock::time_point* programStartTime
) 
{
        // Record trade details
        TradeDetails trade;
        trade.orderID = order.orderID;
        trade.clientID = order.clientID;
        trade.instrument = order.instrument;
        trade.side = order.side;
        trade.execStatus = "New";
        trade.quantity = order.quantity;
        trade.price = order.price;
        trade.duration = get_time(programStartTime);

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
                                trade_.duration = get_time(programStartTime);
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
                                trade_.duration = get_time(programStartTime);
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
                                trade_.duration = get_time(programStartTime);
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
                                trade_.duration = get_time(programStartTime);
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
                                trade_.duration = get_time(programStartTime);
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
                                trade_.duration = get_time(programStartTime);
                                trades->push_back(trade_);
                            }
                            trade.execStatus = "Fill";
                        }
                    
                
                        // Store trade details in a container or log file
        		trade.duration = get_time(programStartTime);
        		trades->push_back(trade);
    }

}


// handling error orders
void process_err(
    Order order,
    std::vector<TradeDetails>* trades,
    std::chrono::steady_clock::time_point* programStartTime
)
{
    // Record error details
    TradeDetails trade;
    trade.orderID = order.orderID;
    trade.clientID = order.clientID;
    trade.instrument = order.instrument;
    trade.side = order.side;
    trade.execStatus = "Reject";
    trade.quantity = order.quantity;
    trade.price = order.price;
    trade.reason = order.reason;
    trade.duration = get_time(programStartTime);
    trades->push_back(trade);

} 

// processing all 5 order books
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
            priority_queue<Order, vector<Order>, SellComparator>* OrchidSellQueue,
            std::chrono::steady_clock::time_point* programStartTime
            )
{
        trades->clear();
        for (const Order& order : orders) {
            if (order.reason != "") process_err(order,trades,programStartTime);
            else {
                if (order.instrument == "Rose") process(order,trades,RoseBuyQueue,RoseSellQueue,programStartTime);
                if (order.instrument == "Lavender")  process(order,trades,LavenderBuyQueue,LavenderSellQueue,programStartTime);
                if (order.instrument == "Lotus")  process(order,trades,LotusBuyQueue,LotusSellQueue,programStartTime);
                if (order.instrument == "Tulip")  process(order,trades,TulipBuyQueue,TulipSellQueue,programStartTime);
                if (order.instrument == "Orchid")  process(order,trades,OrchidBuyQueue,OrchidSellQueue,programStartTime); 
            }
        }
}
