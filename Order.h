#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

struct Order {
    string type;
    int quantity;
    // should be > 0
    double price;
    double timestamp;
    string stock_symbol;
    string trader_id;
};
// Comparator for Buy Orders (Max Heap)
struct BuyOrderComparator {
    bool operator()(const Order& a, const Order& b) {
        if (a.price == b.price)
            return a.timestamp  > b.timestamp;  // Earlier order gets priority
        return a.price < b.price;  // Higher price gets priority
    }
};

// Comparator for Sell Orders (Min Heap)
struct SellOrderComparator {
    bool operator()(const Order& a, const Order& b) {
        if (a.price == b.price)
            return a.timestamp > b.timestamp;  // Earlier order gets priority
        return a.price > b.price;  // Lower price gets priority
    }
};

#endif
