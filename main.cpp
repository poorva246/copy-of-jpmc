#include "TradingSimulation.h"
#include <iostream>
#include <string>
using namespace std;

void displayMenu() {
    cout << "\n===== STOCK EXCHANGE SIMULATION MENU =====" << endl;
    cout << "1. Execute single trade" << endl;
    cout << "2. View top 5 bids and offers for a stock" << endl;
    cout << "3. Run automated simulation" << endl;
    cout << "4. View trader info" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice (1-5): ";
}

void displayTraderInfo(TradingSimulation& simulation) {
    cout << "\n===== TRADER INFORMATION =====" << endl;
    vector<string> trader_ids = simulation.getTraderIDs();
    
    cout << "Available traders:" << endl;
    for (const auto& id : trader_ids) {
        cout << "- " << id << endl;
    }
    
    cout << "\nEnter trader ID to view details: ";
    string trader_id;
    cin >> trader_id;
    
    Trader* trader = simulation.getTrader(trader_id);
    if (!trader) {
        cout << "Trader not found." << endl;
        return;
    }
    
    cout << "\n--- Trader: " << trader_id << " ---" << endl;
    cout << "Cash Balance: $" << trader->getBalance() << endl;
    cout << "Portfolio Value: $" << simulation.calculatePortfolioValue(trader_id) << endl;
    cout << "Stock Holdings:" << endl;
    
    for (const auto& stock : simulation.getStockSymbols()) {
        int quantity = trader->getStockQuantity(stock);
        if (quantity > 0) {
            cout << "  " << stock << ": " << quantity << " shares" << endl;
        }
    }
}

int main() {
    // Seed random number generator
    srand(time(nullptr));
    
    // Configuration parameters
    double trading_hours = 6.5 * 3600;  // 6.5 hours in seconds
    double time_step = 1;               // 1 second time step
    int stock_count = 5;
    int trader_count = 5;
    vector<string> stock_symbols = {"AAPL", "GOOG", "AMZN", "MSFT", "TSLA"};
    
    // Initialize the simulation
    cout << "Initializing stock exchange simulation..." << endl;
    TradingSimulation simulation(trading_hours, time_step, stock_count, trader_count, stock_symbols);
    
    int choice;
    bool exit_program = false;
    
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                // Execute single trade
                cout << "\nAvailable traders:" << endl;
                for (const auto& id : simulation.getTraderIDs()) {
                    cout << "- " << id << endl;
                }
                
                cout << "Enter trader ID: ";
                string trader_id;
                cin >> trader_id;
                
                simulation.executeSingleTrade(trader_id);
                break;
            }
            case 2: {
                // View top 5 bids and offers
                cout << "\nAvailable stocks:" << endl;
                for (const auto& stock : stock_symbols) {
                    cout << "- " << stock << endl;
                }
                
                cout << "Enter stock symbol: ";
                string stock_symbol;
                cin >> stock_symbol;
                
                simulation.displayTop5Orders(stock_symbol);
                break;
            }
            case 3: {
                // Run automated simulation
                simulation.runSimulation();
                break;
            }
            case 4: {
                // View trader info
                displayTraderInfo(simulation);
                break;
            }
            case 5: {
                // Exit
                exit_program = true;
                cout << "Exiting program. Thank you!" << endl;
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    } while (!exit_program);
    
    return 0;
}
