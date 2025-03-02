// TradingSimulation.h
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
    unordered_map<string, Trader*> traders;
    vector<string> stock_symbols;
    unordered_map<string, double> initial_portfolios_value; // To track initial portfolio values

public:
    // Constructor: Initialize exchange and traders
    TradingSimulation(double trading_hours, double time_step, int stock_count, int trader_count, vector<string>& stock_symbols);

    // Run the full automated simulation
    void runSimulation();

    // Execute a single manual trade with a specified trader
    void executeSingleTrade(string trader_id);
    
    // Display top 5 bids and offers for a stock
    void displayTop5Orders(string stock_symbol);
    
    // Execute a trading step (automated mode)
    void performTradeStep(double timestamp);
    
    // Calculate and store initial portfolio values
    void storeInitialPortfolioValues();

    // Display final trader results including profits/losses
    void displayResults();

    // Get a trader by ID
    Trader* getTrader(string trader_id);
    
    // Get the list of all trader IDs
    vector<string> getTraderIDs();
    
    // Get the list of all stock symbols
    vector<string> getStockSymbols();
    
    // Calculate portfolio value for a trader
    double calculatePortfolioValue(string trader_id);

    // Destructor: Clean up dynamically allocated traders
    ~TradingSimulation();
};

#endif // TRADING_SIMULATION_H
