#include "TradingSimulation.h"
#include "OrderManager.h"

// ✅ Constructor: Initialize Stock Exchange & Traders
TradingSimulation::TradingSimulation(double trading_hours,double time_step, int stock_count, int trader_count, vector<string>& stock_symbols) {

    this->trading_hours = trading_hours;
    this->time_step = time_step;
    this->stock_count = stock_count;
    this->trader_count = trader_count;
    this->stock_symbols = stock_symbols;
    orderManager = new OrderManager();  // Allocate memory for OrderManager
    cout << "DEBUG: Creating StockExchange object..." << endl;
    exchange = new StockExchange(trading_hours, time_step, stock_symbols, orderManager);
    cout << "DEBUG: StockExchange object created!" << endl;
    // initialize traders
    for(int i=0;i<trader_count;i++){
        cout<<"Enter the trader id: \n";
        string id;
        cin>>id;
        cout<<"Enter the balance: \n";
        double balance;
        cin>>balance;
        unordered_map<string, int> portfolio;
        cout<<"Enter the stock symbols and quantity: \n";
        for(int j = 0; j < stock_count; j++) {
            string stock;
            int quantity;
            cin >> stock >> quantity;
            portfolio[stock] = quantity;
        }
        Trader* trader = new Trader(id, balance, portfolio);
        traders[id] = trader;
        exchange->registerTrader(trader);
        cout<<"Trader "<<id<<" has been registered successfully\n";
    }
}

// ✅ Run Simulation
void TradingSimulation::runSimulation() {
    // Run simulation for trading hours
    cout<<"Running simulation..."<<endl;
    for (double time = 0; time < trading_hours; time += time_step) {
        // Perform a trading step
        cout<<"Performing trade step..."<<endl;
        performTradeStep(time);
        this_thread::sleep_for(chrono::duration<double>(time_step));
        // displayResults();
    }
    // Display final results
    displayResults();
    
}

void TradingSimulation::performTradeStep(double timestamp) {
    for (auto it : traders) {
        Trader* trader = it.second;
        cout<<"Enter the order type (BUY/SELL) for trader "<<trader->getID()<<endl;
        string order_type;
        cin>>order_type;
        cout<<"Enter the stock symbol for trader "<<trader->getID()<<endl;
        string stock_symbol;    
        cin>>stock_symbol;
        cout<<"Enter the quantity for trader "<<trader->getID()<<endl;
        int quantity;
        cin>>quantity;

        // Decide price using Action's pricing logic
        Action action;
        double price = action.FindPrice(stock_symbol, exchange);

        if(order_type == "BUY"){
            double total_cost = price * quantity;
            if (!trader->canAffordPurchase(total_cost)) {
                if (!trader->depositFundsIfNeeded(total_cost)) {
                    cout << "Trader " << trader->getID() << " cannot afford to buy " << quantity << " stocks of " << stock_symbol << " at $" << price << " each.\n";
                    continue;
                }
            }
        } else {
            if (!trader->canSellStock(stock_symbol, quantity)) {
                cout << "Trader " << trader->getID() << " does not have " << quantity << " stocks of " << stock_symbol << " to sell.\n";
                continue;
            }
        }
        // Execute order
        cout<<"Order is executing..."<<endl;
        orderManager->CreateAndAddOrder(order_type, quantity, stock_symbol, trader->getID(), price, timestamp, exchange);
    
}
    // Match orders
    cout<<"Matching orders..."<<endl;
    for(const string& stock : stock_symbols)exchange->MatchOrders(stock);
}

// ✅ Display final results
void TradingSimulation::displayResults() {
    cout << "\nFinal Trader Balances & Portfolio:\n";
    for (auto it : traders) {
        Trader* trader = it.second;
        cout << "Trader " << trader->getID() << " | Balance: $" << trader->getBalance() << "\n";
        for (const string& stock : stock_symbols) {
            cout << "   - " << stock << " Stocks: " << trader->getStockQuantity(stock) << "\n";
        }
    }
}

// ✅ Destructor: Clean up trader objects
TradingSimulation::~TradingSimulation() {
    delete orderManager;
}