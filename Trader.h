#ifndef TRADER_H
#define TRADER_H

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;


class Trader {

private:
    string ID;
    double account_balance;
    unordered_map<string, int> portfolio;

public:
    // Account balance manipulation methods
    void addCash(double amount);
    bool deductCash(double amount);
    void addStock(string stock_symbol, int quantity);
    void removeStock(string stock_symbol, int quantity);
    // Constructor
    Trader(string id, double balance, unordered_map<string, int> portfolio);
    string getID();
    double getBalance();
    int getStockQuantity(string stock_symbol);
    bool canAffordPurchase(double total_cost);
    bool canSellStock(string stock_symbol, int quantity);
    bool depositFundsIfNeeded(double required_amount);

};

#endif