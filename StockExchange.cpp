#include "OrderManager.h"


    StockExchange::StockExchange(double trading_hours, double time_step, vector<string>& stock_symbols, OrderManager* orderManager) {
    this->trading_hours = trading_hours;
    this->time_step = time_step;
    this->orderManager = orderManager;

    for (string stock : stock_symbols) {
        this->last_closing_prices[stock] = 50.0;  // ✅ Should set default value
        buyOrders[stock] = priority_queue<Order, vector<Order>, BuyOrderComparator>();
        sellOrders[stock] = priority_queue<Order, vector<Order>, SellOrderComparator>();
    }
}

     void StockExchange::registerTrader(Trader* trader) {
        traders[trader->getID()] = trader;
    }
    double StockExchange::LastClosingPrice(string stock_symbol){
        return this->last_closing_prices[stock_symbol];
    }
    pair<double,double> StockExchange::CurrentBestBidandOffer(string stock_symbol){
        if(buyOrders[stock_symbol].empty() || sellOrders[stock_symbol].empty())
            return {0.0,0.0};
        return {buyOrders[stock_symbol].top().price, sellOrders[stock_symbol].top().price};
    }
    // function to return top 5 buy and sell orders (note that method is modifying the priority queues)
    pair<vector<Order>, vector<Order> > StockExchange::Top5BuyandSellOrders(string stock_symbol){
        vector<Order> buy, sell;
        for (int i = 0; i < 5; i++) {
            if(!buyOrders[stock_symbol].empty())
            {buy.push_back(buyOrders[stock_symbol].top());
            buyOrders[stock_symbol].pop();
            }
            if(!sellOrders[stock_symbol].empty()){
            sell.push_back(sellOrders[stock_symbol].top());
            sellOrders[stock_symbol].pop();
            }
        }
        reverse(buy.begin(), buy.end());
        reverse(sell.begin(), sell.end());
        cancelRemainingOrders(stock_symbol);
       // print top5 orders
        return {buy, sell};
    }
    // check if an order is accepted or no
    // return accepted orders
    void StockExchange::addOrder(Order order){
       if(order.type == "BUY"){
           buyOrders[order.stock_symbol].push(order);
       }
       else{
           sellOrders[order.stock_symbol].push(order);
           cout<<"Order added for trader "<<order.trader_id<<endl;
       }
    }
    void StockExchange::AcceptedOrders(string stock_symbol){
        pair<vector<Order>, vector<Order> > top5 = Top5BuyandSellOrders(stock_symbol);
        // print top 5 orders
    }

    void StockExchange::MatchOrders(string stock_symbol){
        pair<vector<Order>, vector<Order> > top5 = Top5BuyandSellOrders(stock_symbol);
        vector<Order> buy = top5.first;
        vector<Order> sell = top5.second;
        while(!buy.empty() && !sell.empty()){
            Order buy_order = buy.back();
            Order sell_order = sell.back();
            int trade_quantity = min(buy_order.quantity, sell_order.quantity);
            cout<<buy_order.price<<" "<<sell_order.price<<endl;
            if(buy_order.price >= sell_order.price){
                // match the orders
                buy.pop_back();
                sell.pop_back();
                buy_order.quantity -= trade_quantity;
                sell_order.quantity -= trade_quantity;
                if (buy_order.quantity > 0) buyOrders[stock_symbol].push(buy_order);
                if (sell_order.quantity > 0) sellOrders[stock_symbol].push(sell_order);
                cout<<"Updating trader values..."<<endl;
                orderManager->updateTraderValue(traders[buy_order.trader_id], traders[sell_order.trader_id], sell_order.price, trade_quantity, stock_symbol);
            }
            else{
                while(!buy.empty()){
                    buyOrders[stock_symbol].push(buy.back());
                    buy.pop_back();
                }
                while(!sell.empty()){
                    sellOrders[stock_symbol].push(sell.back());
                    sell.pop_back();
                }
                break;
            }
        }
    }
    void StockExchange::cancelRemainingOrders(string stock_symbol){
        while(!buyOrders[stock_symbol].empty()) buyOrders[stock_symbol].pop();
        while(!sellOrders[stock_symbol].empty()) sellOrders[stock_symbol].pop();
    }
    void StockExchange::closeMarket(){
        // what id sellOrders is empty?
        for(auto stock : last_closing_prices){
            last_closing_prices[stock.first] = sellOrders[stock.first].top().price;
        }
        for(auto stock : last_closing_prices){
            cancelRemainingOrders(stock.first);
        }
    }
