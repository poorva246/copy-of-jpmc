#ifndef STOCKEXCHANGE_H
#define STOCKEXCHANGE_H

#include "Order.h"
#include "Trader.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <algorithm>

class OrderManager;

class StockExchange {
    double trading_hours, time_step;
    unordered_map<string,priority_queue<Order, vector<Order>, BuyOrderComparator> > buyOrders;
    unordered_map<string,priority_queue<Order, vector<Order>, SellOrderComparator> > sellOrders;
    unordered_map<string,double> last_closing_prices;
    unordered_map<string,Trader*> traders;
    vector<string> stock_symbols;
    OrderManager* orderManager;

public :
    StockExchange(double trading_hours, double time_step, vector<string>&stock_symbols, OrderManager* orderManager);
    void registerTrader(Trader* trader);
    double LastClosingPrice(string stock_symbol);
    pair<double,double> CurrentBestBidandOffer(string stock_symbol);
    // function to return top 5 buy and sell orders (note that method is modifying the priority queues)
    pair<vector<Order>, vector<Order> > Top5BuyandSellOrders(string stock_symbol);
    void addOrder(Order order);
    void AcceptedOrders(string stock_symbol);
    void MatchOrders(string stock_symbol);
    void cancelRemainingOrders(string stock_symbol);
    void closeMarket();
};


#endif