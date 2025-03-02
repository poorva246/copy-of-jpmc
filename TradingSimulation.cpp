#include "TradingSimulation.h"

// Constructor implementation with random trader initialization
TradingSimulation::TradingSimulation(double trading_hours, double time_step, int stock_count, int trader_count, vector<string>& stock_symbols) {
    // Seed random number generator
    srand(time(nullptr));
    
    this->trading_hours = trading_hours;
    this->time_step = time_step;
    this->stock_count = stock_count;
    this->trader_count = trader_count;
    this->stock_symbols = stock_symbols;
    
    orderManager = new OrderManager();
    exchange = new StockExchange(trading_hours, time_step, stock_symbols, orderManager);
    
    // Initialize traders with random portfolios and balance
    for (int i = 0; i < trader_count; i++) {
        string id = "Trader" + to_string(i+1);
        
        // Random balance between $10,000 and $100,000
        double balance = 10000 + (rand() % 90000);
        
        unordered_map<string, int> portfolio;
        
        // Random portfolio of stocks
        for (const string& stock : stock_symbols) {
            // Random quantity between 0 and 5000 shares
            int quantity = rand() % 5001;
            if (quantity > 0) {
                portfolio[stock] = quantity;
            }
        }
        
        Trader* trader = new Trader(id, balance, portfolio);
        traders[id] = trader;
        exchange->registerTrader(trader);
        
        cout << "Trader " << id << " initialized with $" << balance << " and portfolio: ";
        for (const auto& [stock, qty] : portfolio) {
            if (qty > 0) {
                cout << stock << ":" << qty << " ";
            }
        }
        cout << endl;
    }
    
    // Store initial portfolio values for profit calculation
    storeInitialPortfolioValues();
}

// Store initial portfolio values for all traders
void TradingSimulation::storeInitialPortfolioValues() {
    for (const auto& [trader_id, trader] : traders) {
        initial_portfolios_value[trader_id] = calculatePortfolioValue(trader_id);
    }
}

// Calculate portfolio value for a trader
double TradingSimulation::calculatePortfolioValue(string trader_id) {
    Trader* trader = traders[trader_id];
    double total_value = trader->getBalance();
    
    // For each stock in the trader's portfolio, calculate its value
    for (const auto& stock_symbol : stock_symbols) {
        int quantity = trader->getStockQuantity(stock_symbol);
        if (quantity > 0) {
            // Use the last closing price for valuation
            double price = exchange->LastClosingPrice(stock_symbol);
            total_value += (price * quantity);
        }
    }
    
    return total_value;
}

// Execute a single manual trade with a specified trader
void TradingSimulation::executeSingleTrade(string trader_id) {
    if (traders.find(trader_id) == traders.end()) {
        cout << "Trader ID not found: " << trader_id << endl;
        return;
    }
    
    Trader* trader = traders[trader_id];
    
    // Ask for order details
    cout << "Enter order type (BUY/SELL): ";
    string order_type;
    cin >> order_type;
    
    // Convert to uppercase
    for (char& c : order_type) c = toupper(c);
    
    if (order_type != "BUY" && order_type != "SELL") {
        cout << "Invalid order type. Must be BUY or SELL." << endl;
        return;
    }
    
    cout << "Available stocks: ";
    for (const auto& stock : stock_symbols) {
        cout << stock << " ";
    }
    cout << endl;
    
    cout << "Enter stock symbol: ";
    string stock_symbol;
    cin >> stock_symbol;
    
    // Convert to uppercase
    for (char& c : stock_symbol) c = toupper(c);
    
    // Check if stock symbol is valid
    bool valid_stock = false;
    for (const auto& stock : stock_symbols) {
        if (stock == stock_symbol) {
            valid_stock = true;
            break;
        }
    }
    
    if (!valid_stock) {
        cout << "Invalid stock symbol." << endl;
        return;
    }
    
    // Fixed quantity as per requirements
    int quantity = 1000;
    cout << "Order quantity set to 1000 as per requirements." << endl;
    
    // Use action to determine randomized price
    Action action;
    double price = action.FindPrice(stock_symbol, exchange);
    cout << "Randomized price: $" << price << endl;
    
    // Verify the trader can execute the order
    if (order_type == "BUY") {
        double total_cost = price * quantity;
        if (!trader->canAffordPurchase(total_cost)) {
            cout << "Trader cannot afford the purchase. Total cost: $" << total_cost << endl;
            cout << "Current balance: $" << trader->getBalance() << endl;
            
            // Offer deposit option
            if (trader->depositFundsIfNeeded(total_cost)) {
                cout << "Deposit successful. New balance: $" << trader->getBalance() << endl;
            } else {
                cout << "Trade canceled due to insufficient funds." << endl;
                return;
            }
        }
    } else { // SELL
        if (!trader->canSellStock(stock_symbol, quantity)) {
            cout << "Trader does not have enough shares to sell." << endl;
            cout << "Current holdings of " << stock_symbol << ": " << trader->getStockQuantity(stock_symbol) << endl;
            cout << "Trade canceled." << endl;
            return;
        }
    }
    
    // Execute the order
    double timestamp = 0; // You might want to track a real timestamp
    orderManager->CreateAndAddOrder(order_type, quantity, stock_symbol, trader_id, price, timestamp, exchange);
    cout << "Order placed successfully!" << endl;
    
    // Try to match orders
    exchange->MatchOrders(stock_symbol);
    cout << "Order matching attempt completed." << endl;
}

// Display top 5 bids and offers for a stock
void TradingSimulation::displayTop5Orders(string stock_symbol) {
    // Convert to uppercase
    for (char& c : stock_symbol) c = toupper(c);
    
    // Check if stock symbol is valid
    bool valid_stock = false;
    for (const auto& stock : stock_symbols) {
        if (stock == stock_symbol) {
            valid_stock = true;
            break;
        }
    }
    
    if (!valid_stock) {
        cout << "Invalid stock symbol." << endl;
        return;
    }
    
    // Get top 5 orders without modifying the queues
    auto [buy_orders, sell_orders] = exchange->Top5BuyandSellOrders(stock_symbol);
    
    cout << "\n==== TOP 5 ORDERS FOR " << stock_symbol << " ====" << endl;
    
    // Display buy orders
    cout << "\nTOP BUY ORDERS (BID):" << endl;
    cout << "-------------------" << endl;
    if (buy_orders.empty()) {
        cout << "No buy orders available." << endl;
    } else {
        cout << "Price\tQuantity\tTrader ID\tTimestamp" << endl;
        for (const auto& order : buy_orders) {
            cout << "$" << order.price << "\t" << order.quantity << "\t\t" 
                 << order.trader_id << "\t\t" << order.timestamp << endl;
        }
    }
    
    // Display sell orders
    cout << "\nTOP SELL ORDERS (ASK):" << endl;
    cout << "--------------------" << endl;
    if (sell_orders.empty()) {
        cout << "No sell orders available." << endl;
    } else {
        cout << "Price\tQuantity\tTrader ID\tTimestamp" << endl;
        for (const auto& order : sell_orders) {
            cout << "$" << order.price << "\t" << order.quantity << "\t\t" 
                 << order.trader_id << "\t\t" << order.timestamp << endl;
        }
    }
    
    // Display best bid and ask
    auto [best_bid, best_ask] = exchange->CurrentBestBidandOffer(stock_symbol);
    cout << "\nBest Bid: $" << (best_bid > 0 ? to_string(best_bid) : "N/A") << endl;
    cout << "Best Ask: $" << (best_ask > 0 ? to_string(best_ask) : "N/A") << endl;
    
    // Display last closing price
    cout << "Last Closing Price: $" << exchange->LastClosingPrice(stock_symbol) << endl;
}

// Run the automated simulation
void TradingSimulation::runSimulation() {
    cout << "\nRunning automated simulation for " << (trading_hours / 3600) << " hours..." << endl;
    
    // Run simulation for trading hours
    for (double time = 0; time < trading_hours; time += time_step) {
        cout << "\nTime: " << time << " seconds" << endl;
        performTradeStep(time);
        this_thread::sleep_for(chrono::milliseconds(10)); // Small delay for simulation
    }
    
    // Close the market
    exchange->closeMarket();
    
    // Display final results
    displayResults();
}

// Execute a trading step in automated mode
void TradingSimulation::performTradeStep(double timestamp) {
    for (auto& [trader_id, trader] : traders) {
        // Randomly choose buy or sell
        string order_type = (rand() % 2 == 0) ? "BUY" : "SELL";
        
        // Randomly choose a stock symbol
        string stock_symbol = stock_symbols[rand() % stock_symbols.size()];
        
        // Fixed quantity of 1000 as per requirements
        int quantity = 1000;
        
        // Decide price using Action's pricing logic
        Action action;
        double price = action.FindPrice(stock_symbol, exchange);
        
        // Check if the trader can afford the purchase or has stocks to sell
        if (order_type == "BUY") {
            double total_cost = price * quantity;
            if (!trader->canAffordPurchase(total_cost)) {
                cout << "Trader " << trader_id << " cannot afford to buy " << quantity 
                     << " shares of " << stock_symbol << " at $" << price << endl;
                continue;
            }
        } else { // SELL
            if (!trader->canSellStock(stock_symbol, quantity)) {
                cout << "Trader " << trader_id << " does not have enough " << stock_symbol 
                     << " shares to sell " << quantity << " units" << endl;
                continue;
            }
        }
        
        // Execute order
        cout << "Executing " << order_type << " order for " << trader_id 
             << ": " << quantity << " shares of " << stock_symbol << " at $" << price << endl;
        
        orderManager->CreateAndAddOrder(order_type, quantity, stock_symbol, 
                                        trader_id, price, timestamp, exchange);
    }
    
    // Match orders for all stocks
    for (const auto& stock : stock_symbols) {
        exchange->MatchOrders(stock);
    }
}

// Display final results with profit/loss calculations
void TradingSimulation::displayResults() {
    cout << "\n==================================" << endl;
    cout << "FINAL TRADING RESULTS" << endl;
    cout << "==================================" << endl;
    
    for (const auto& [trader_id, trader] : traders) {
        double initial_value = initial_portfolios_value[trader_id];
        double final_value = calculatePortfolioValue(trader_id);
        double profit_loss = final_value - initial_value;
        double percentage = (profit_loss / initial_value) * 100;
        
        cout << "\nTrader: " << trader_id << endl;
        cout << "-------------------------" << endl;
        cout << "Initial Portfolio Value: $" << initial_value << endl;
        cout << "Final Portfolio Value: $" << final_value << endl;
        cout << "Profit/Loss: $" << profit_loss << " (" << percentage << "%)" << endl;
        
        cout << "\nCash Balance: $" << trader->getBalance() << endl;
        cout << "Stock Holdings:" << endl;
        
        for (const auto& stock : stock_symbols) {
            int quantity = trader->getStockQuantity(stock);
            if (quantity > 0) {
                double price = exchange->LastClosingPrice(stock);
                double value = price * quantity;
                cout << "  " << stock << ": " << quantity << " shares @ $" 
                     << price << " = $" << value << endl;
            }
        }
        cout << endl;
    }
}

// Helper method to get a trader by ID
Trader* TradingSimulation::getTrader(string trader_id) {
    if (traders.find(trader_id) != traders.end()) {
        return traders[trader_id];
    }
    return nullptr;
}

// Get the list of all trader IDs
vector<string> TradingSimulation::getTraderIDs() {
    vector<string> ids;
    for (const auto& [id, _] : traders) {
        ids.push_back(id);
    }
    return ids;
}

// Get the list of all stock symbols
vector<string> TradingSimulation::getStockSymbols() {
    return stock_symbols;
}

// Destructor implementation
TradingSimulation::~TradingSimulation() {
    delete exchange;
    delete orderManager;
    
    // Clean up trader objects
    for (auto& [_, trader] : traders) {
        delete trader;
    }
}
