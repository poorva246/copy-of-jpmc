#ifndef TRADING_SIMULATION_H
#define TRADING_SIMULATION_H

#include "StockExchange.h"
#include "Action.h"
#include <vector>
#include <thread>
#include <iostream>
#include <cstdlib>  
#include <ctime>    
#include <thread>

class StockExchange;

class TradingSimulation {
private:
    double trading_hours;
    double time_step;
    int stock_count;
    int trader_count;
    StockExchange* exchange;
    OrderManager* orderManager;
     unordered_map<string,Trader*> traders;
    vector<string> stock_symbols;

public:
    // Constructor: Initialize exchange and traders
    TradingSimulation(double trading_hours, double time_step, int stock_count, int trader_count, vector<string>& stock_symbols);

    // Run the full simulation
    void runSimulation();

    // Execute a trading step
    void performTradeStep(double timestamp);

    // Display final trader results
    void displayResults();

    // Destructor: Clean up dynamically allocated traders
    ~TradingSimulation();
};

#endif // TRADING_SIMULATION_H