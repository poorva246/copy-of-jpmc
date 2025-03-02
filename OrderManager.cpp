#include "OrderManager.h"


void OrderManager::updateTraderValue(Trader* buyer, Trader* seller, double price, int quantity, string stock_symbol) {
    double total_cost = price * quantity;
    buyer->deductCash(total_cost);
    seller->addCash(total_cost);
    buyer->addStock(stock_symbol, quantity);
    seller->removeStock(stock_symbol, quantity);
    cout<<buyer->getBalance()<<endl;
    cout<<seller->getBalance()<<endl;
    cout<<buyer->getStockQuantity(stock_symbol)<<endl;
}

    // Create and add an order (Private)
void OrderManager::CreateAndAddOrder(string order_type, int quantity, string stock_symbol, string trader_id, double price, double timestamp, StockExchange* exchange) {
    Order new_order = {order_type, quantity, price, timestamp, stock_symbol, trader_id};
    cout<<"Order created for trader with price "<<price<<endl;
    exchange->addOrder(new_order);
}
