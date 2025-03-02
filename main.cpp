#include "TradingSimulation.h"

#include <iostream>
using namespace std;

void Fast_IO(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    #ifndef ONLINE_JUDGE
    freopen("z_input.txt","r",stdin);
    freopen("z_output.txt","w",stdout);
    #endif
} 

int main (){
    Fast_IO();

    double trading_hours = 6.5 * 3600; 
    double time_step = 1;              
    int stock_count = 5;              
    int trader_count = 5;  
    vector<string> stock_symbols = {"AAPL", "GOOG", "AMZN", "MSFT", "TSLA"};
    TradingSimulation simulation(trading_hours, time_step, stock_count, trader_count, stock_symbols);


    simulation.runSimulation();

    simulation.displayResults();
    return 0;
}