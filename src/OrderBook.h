#include "Order.h"

#include <functional>
#include <list>
#include <map>
#include <unordered_map>

/* For optimization later */
#include <memory>

class OrderBook {
private:
  std::unordered_map<int, std::list<Order>::iterator> orderMap;
  std::map<double, std::list<Order>, std::greater<double>> bids;
  std::map<double, std::list<Order>> asks;

  /* Explicit Best Bid/Ask tracking */
  double best_bid = 0.0;
  double best_ask = 0.0;

public:
  bool add_order(int id, OrderType type, double price, int quantity);
  bool cancel_order(int id);
  void print_book();

  double get_best_bid() const { return best_bid; }
  double get_best_ask() const { return best_ask; }
};