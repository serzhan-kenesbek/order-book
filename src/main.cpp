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

/* 
    The OrderBook must perform three distinct functions with maximum efficiency:
        * The ID Lookup - given an id find an order instantly.
            * Options: Hash Table
        * The Ordering  - the book must maintain all Bids (Buys) sorted from
                          highest price to lowest, and all Asks (Sells) sorted
                          from lowest price to highest. Efficiently insert new
                          prices and instantly find the best price.
            * Options: The Tree Family (BSTs, AVL Trees, or Red-Black Trees),
                       The Heap Family (Min-Heaps or Max-Heaps like Priority
                       Queues in C++)
            * Chose:   RBT. O(log n) operations with fewer rotations than AVL, 
                       ideal for constant insertions/cancellations. Maintains 
                       sorted order automatically, O(1) best price access, and 
                       supports market depth traversal.
        * Time Priority - at any single price orders must be executed in the 
                          order they arrived (FIFO).
            * Options: Queue implementation (Doubly Linked List, Double-Ended
                       Queue, Vector)
            * Chose:   User should be able to cancel their orders before they
                       get matched. To make sure it's instantaneous (O(1))
                       doubly-linked list makes the most sense.
*/
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
  return true;
}

int main() {
  std::cout << "Hello World" << std::endl;

  return 0;
}