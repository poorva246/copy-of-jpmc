#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <string>
#include "StockExchange.h"
using namespace std;

class Action {
    public:
    double FindPrice(string stock_symbol, StockExchange* exchange);
};

#endif