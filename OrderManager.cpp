#include "OrderManager.h"

// Add implementation of missing methods
double OrderManager::FindCashBalance(Trader* t) {
    return t->getBalance();
}

void OrderManager::addCash(Trader* t, double amount) {
    t->addCash(amount);
}

void OrderManager::withdrawCash(Trader* t, double amount) {
    t->deductCash(amount);
}

void OrderManager::updateTraderValue(Trader* buyer, Trader* seller, double price, int quantity, string stock_symbol) {
    double total_cost = price * quantity;
    buyer->deductCash(total_cost);
    seller->addCash(total_cost);
    buyer->addStock(stock_symbol, quantity);
    seller->removeStock(stock_symbol, quantity);
    cout << "Buyer balance: $" << buyer->getBalance() << endl;
    cout << "Seller balance: $" << seller->getBalance() << endl;
    cout << "Buyer now has " << buyer->getStockQuantity(stock_symbol) << " shares of " << stock_symbol << endl;
}

// Create and add an order
void OrderManager::CreateAndAddOrder(string order_type, int quantity, string stock_symbol, string trader_id, double price, double timestamp, StockExchange* exchange) {
    Order new_order = {order_type, quantity, price, timestamp, stock_symbol, trader_id};
    cout << "Order created for trader " << trader_id << " - " << order_type << " " << quantity << " " << stock_symbol << " @ $" << price << endl;
    exchange->addOrder(new_order);
}
