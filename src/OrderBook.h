#include "Order.h"

#include <list>
#include <map>
#include <unordered_map>
#include <functional>

/* For optimization later */
#include <memory>

class OrderBook {
    private:
        std::unordered_map<int, std::list<Order>::iterator> orderMap;
        std::map<double, std::list<Order>, std::greater<double>> bids;
        std::map<double, std::list<Order>> asks;
    public:
        bool add_order(int id, OrderType type, double price, int quantity);
        bool cancel_order(int id);
        void print_book();
};