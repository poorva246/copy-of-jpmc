#include "Action.h"

    double Action::FindPrice(string stock_symbol, StockExchange* exchange) {
        auto [best_bid, best_ask] = exchange->CurrentBestBidandOffer(stock_symbol);
        if(best_bid == 0 && best_ask == 0){
            double closing_price = exchange->LastClosingPrice(stock_symbol);
            double lower_value = 0.95 * closing_price;
            double upper_value = 1.05 * closing_price;
            return (rand() % 2 == 0) ? lower_value : upper_value;
        }
        else{
            vector<double>temp;
            temp.push_back(best_bid);
            temp.push_back(best_ask);
            double mid = (best_bid + best_ask)/ 2;
            temp.push_back(mid);
            int random_index = rand() % 3;
            return temp[random_index];
        }
    }