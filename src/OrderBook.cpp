#include "OrderBook.h"
#include <chrono>
#include <iomanip>
#include <iostream>

long long getSteadyTimeNanoseconds() {
  auto now = std::chrono::steady_clock::now();
  auto duration_since_epoch = now.time_since_epoch();

  auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(
      duration_since_epoch);

  return nanoseconds.count();
}

void OrderBook::execute_trade(Order &incoming_order, Limit &limit,
                              double match_price) {
  auto &orders = limit.orders;
  auto &best_match = orders.front();

  int trade_quantity = std::min(best_match.quantity, incoming_order.quantity);
  incoming_order.quantity -= trade_quantity;
  best_match.quantity -= trade_quantity;
  limit.total_volume -= trade_quantity;

  std::cout << "Trade Executed: " << trade_quantity << " @ " << match_price
            << "\n";

  /* Cleanup if the current order is filled */
  if (best_match.quantity == 0) {
    orderMap.erase(best_match.id);
    orders.pop_front();
  }
}

bool OrderBook::add_order(int id, OrderType type, double price, int quantity) {
  /* Error checks  and Initialization */
  if (orderMap.find(id) != orderMap.end()) {
    std::cerr << "Error: Order ID " << id << " already exists\n";
    return false;
  }
  if (price <= 0 || quantity <= 0) {
    std::cerr << "Error: Invalid input\n";
    return false;
  }

  Order new_order;
  new_order.id = id;
  new_order.type = type;
  new_order.price = price;
  new_order.quantity = quantity;
  /* Avoid std::chrono due to the possibility of the time jumping backward due
   * to OS updates */
  new_order.timestamp = getSteadyTimeNanoseconds();

  /* Matching */
  if (type == OrderType::BID) { /* Buy */
    while (new_order.quantity > 0 && asks.empty() == false) {
      auto best_ask_it = asks.begin();
      double best_ask_price = best_ask_it->first;

      /* is Spread crossed or not */
      if (best_ask_price > new_order.price) {
        break;
      }

      auto &limit = best_ask_it->second;
      execute_trade(new_order, limit, best_ask_price);

      if (limit.orders.empty()) {
        asks.erase(best_ask_it);
      }
    }
  } else { /* Sell */
    while (new_order.quantity > 0 && bids.empty() == false) {
      auto best_bid_it = bids.begin();
      double best_bid_price = best_bid_it->first;

      if (best_bid_price < new_order.price) {
        break;
      }

      auto &limit = best_bid_it->second;
      execute_trade(new_order, limit, best_bid_price);

      if (limit.orders.empty()) {
        bids.erase(best_bid_it);
      }
    }
  }

  /* Add to Book if quantity left */
  if (new_order.quantity > 0) {
    if (type == OrderType::BID) {
      bids[price].orders.push_back(new_order);
      bids[price].total_volume += new_order.quantity;
      auto it = --bids[price].orders.end();
      orderMap[id] = it;
    } else {
      asks[price].orders.push_back(new_order);
      asks[price].total_volume += new_order.quantity;
      auto it = --asks[price].orders.end();
      orderMap[id] = it;
    }
  }

  /* Explicitly update the cached best bid and best ask values */
  if (bids.empty()) {
    best_bid = 0.0;
  } else {
    best_bid = bids.begin()->first;
  }

  if (asks.empty()) {
    best_ask = 0.0;
  } else {
    best_ask = asks.begin()->first;
  }

  return true;
}

bool OrderBook::cancel_order(int id) {
  auto it = orderMap.find(id);

  if (it == orderMap.end()) {
    std::cerr << "Error: Order under that ID doesn't exist" << std::endl;
    return false;
  }

  auto list_it = it->second;
  double price = list_it->price;

  if (list_it->type == OrderType::BID) {
    bids.at(price).total_volume -= list_it->quantity;
    bids.at(price).orders.erase(list_it);
    if (bids.at(price).orders.empty()) {
      bids.erase(price);
    }
  } else {
    asks.at(price).total_volume -= list_it->quantity;
    asks.at(price).orders.erase(list_it);

    if (asks.at(price).orders.empty()) {
      asks.erase(price);
    }
  }

  orderMap.erase(id);

  /* Explicitly update the cached best bid and best ask values after a
   * cancellation */
  if (bids.empty()) {
    best_bid = 0.0;
  } else {
    best_bid = bids.begin()->first;
  }

  if (asks.empty()) {
    best_ask = 0.0;
  } else {
    best_ask = asks.begin()->first;
  }

  return true;
}

void OrderBook::print_book() {
  std::cout << "\n--- Market Check ---\n";
  std::cout << "ASKS:\n";
  for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
    double price = it->first;
    auto &limit = it->second;

    std::cout << "  $" << std::fixed << std::setprecision(2) << price
              << "\tVolume: " << limit.total_volume << "\n";
  }

  std::cout << "--------------------\n";

  std::cout << "BIDS:\n";
  for (auto it = bids.begin(); it != bids.end(); ++it) {
    double price = it->first;
    auto &limit = it->second;

    std::cout << "  $" << std::fixed << std::setprecision(2) << price
              << "\tVolume: " << limit.total_volume << "\n";
  }
}