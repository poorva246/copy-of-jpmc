#include "Trader.h"


Trader::Trader(string id, double balance, unordered_map<string,int>portfolio) : ID(id), account_balance(balance), portfolio(portfolio) {}
string Trader::getID(){ return ID; }
double Trader::getBalance(){ return account_balance; }

void Trader::addCash(double amount) { account_balance += amount; }

bool Trader::deductCash(double amount) {
    if (account_balance >= amount) {
        account_balance -= amount;
        return true;
    }
    return false;  // ❌ Not enough balance
}

int Trader::getStockQuantity(string stock_symbol) {
    if (portfolio.find(stock_symbol) != portfolio.end()) {
        return portfolio[stock_symbol];
    }
    return 0;  // ❌ Stock not found
}

// ✅ Checks if the trader can afford the purchase
bool Trader::canAffordPurchase(double total_cost) {
    return account_balance >= total_cost;
}

// ✅ Checks if the trader has enough stocks to sell
bool Trader::canSellStock(string stock_symbol, int quantity) {
    return portfolio[stock_symbol] >= quantity;
}

// ✅ Asks trader to deposit money if needed
bool Trader::depositFundsIfNeeded(double required_amount) {
    cout << "Trader " << ID << " needs $" << required_amount << " to buy. Do you want to deposit? (Y/N): ";
    char choice;
    cin >> choice;
    if (choice == 'Y') {
        cout << "Enter the amount to deposit: ";
        double amount;
        cin >> amount;
        addCash(amount);
        return true;  // ✅ Deposit successful
    }
    return false;  // ❌ Trader refused to deposit
}

void Trader::addStock(string stock_symbol, int quantity) {
    if (portfolio.find(stock_symbol) != portfolio.end()) {
        portfolio[stock_symbol] += quantity;
    } else {
        portfolio[stock_symbol] = quantity;
    }
}

void Trader::removeStock(string stock_symbol, int quantity) {
    if (portfolio.find(stock_symbol) != portfolio.end()) {
        portfolio[stock_symbol] -= quantity;
        if (portfolio[stock_symbol] == 0) {
            portfolio.erase(stock_symbol);
        }
    }
}