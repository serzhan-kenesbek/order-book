#include <iostream>
#include <chrono>
#include <list>
#include <map>
#include <unordered_map>

long long getSteadyTimeNanoseconds() {
  auto now = std::chrono::steady_clock::now();
  auto duration_since_epoch = now.time_since_epoch();

  auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_epoch);

  return nanoseconds.count();
}

enum class OrderType {
    BID,
    ASK
};

// Order is a struct since it's simple data aggregation
struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;

    // long long for required high time precision for Time Priority wihtout risking integer overflow
    long long timestamp;
};

class OrderBook {
    private:
        std::unordered_map<int, std::list<Order>::iterator> orderMap;
        std::map<double, std::list<Order>, std::greater<double>> bids;
        std::map<double, std::list<Order>> asks;
    public:
        bool add_order(int id, OrderType type, double price, int quantity);
        bool cancel_order(int id);
};

bool OrderBook::add_order(int id, OrderType type, double price, int quantity) {
  // Error checks
  if (orderMap.find(id) != orderMap.end()) {
    std::cerr << "Error: Order ID " << id << " already exists\n";
    return false;
  }
  
  if (price <= 0) {
    std::cerr << "Error: Invalid price " << price << "\n";
    return false;
  }
  
  if (quantity <= 0) {
    std::cerr << "Error: Invalid quantity " << quantity << "\n";
    return false;
  }

  Order new_order;
  new_order.id = id;
  new_order.type = type;
  new_order.price = price;
  new_order.quantity = quantity;
  // Avoid std::chrono due to the possibility of the time jumping backward due to OS updates
  new_order.timestamp = getSteadyTimeNanoseconds();

  if (type == OrderType::BID) {
    bids[price].push_back(new_order);
    auto it = --bids[price].end();
    orderMap[id] = it;
  } else {
    asks[price].push_back(new_order);
    auto it = --asks[price].end();
    orderMap[id] = it;
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
    bids.at(price).erase(list_it);
    if (bids.at(price).empty()) {
      bids.erase(price);
    }
  }
  else {
    asks.at(price).erase(list_it);

    if (asks.at(price).empty()) {
      asks.erase(price);
    }
  }

  orderMap.erase(id);
  return true;
}

int main() {
  std::cout << "Hello World" << std::endl;

  return 0;
}