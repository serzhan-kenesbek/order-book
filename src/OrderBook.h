#include "Order.h"

#include <functional>
#include <list>
#include <map>
#include <unordered_map>

struct Limit {
  int total_volume = 0;
  std::list<Order> orders;
};

class OrderBook {
private:
  /*
   * execute_trade is private to enforce encapsulation.
   * Only internal matching logic should trigger trades.
   * If this were public, anyone could execute illegal/fake trades.
   */
  void execute_trade(Order &incoming_order, Limit &limit, int64_t match_price);

  std::unordered_map<int, std::list<Order>::iterator> orderMap;
  std::map<int64_t, Limit, std::greater<int64_t>> bids;
  std::map<int64_t, Limit> asks;

  /* Explicit Best Bid/Ask tracking */
  int64_t best_bid = 0;
  int64_t best_ask = 0;

public:
  bool add_order(int id, OrderType type, int64_t price, int quantity);
  bool cancel_order(int id);
  void print_book();

  int64_t get_best_bid() const { return best_bid; }
  int64_t get_best_ask() const { return best_ask; }
};