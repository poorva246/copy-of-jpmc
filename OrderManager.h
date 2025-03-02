#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H


#include <iostream>
#include "StockExchange.h"
using namespace std;

class OrderManager {

public:
// why are we defining these methods here instead of Trader class
    double FindCashBalance(Trader* t);
    void addCash(Trader* t, double amount);
    void withdrawCash(Trader* t, double amount);
    void updateTraderValue(Trader* buyer, Trader* seller, double price, int quantity, string stock_symbol);

    void CreateAndAddOrder(string order_type, int quantity, string stock_symbol, string trader_id,double price, double timestamp, StockExchange* exchange);
};

#endif