#include "OrderBook.h"
#include <iostream>

int main() {
  OrderBook book;

  /* Test 1: Perfect Match - Buy 10 @ 100 matches Sell 10 @ 100 */
  std::cout << "[TEST 1] Perfect Match (Full Fill)...\n";
  book.add_order(1, OrderType::ASK, 100, 10);
  book.add_order(2, OrderType::BID, 100, 10);
  book.print_book();

  /* Test 2: Partial Fill - Buy 50 @ 100 matches Sell 20 @ 100 */
  std::cout << "\n[TEST 2] Partial Fill (Resting > Aggressive)...\n";
  book.add_order(3, OrderType::BID, 100, 50);
  book.add_order(4, OrderType::ASK, 100, 20);
  book.print_book();

  /* Test 3: Market Sweep - Buy 45 @ 105 eats through 101, 102, and part of 103
   */
  std::cout << "\n[TEST 3] Market Sweep (Multiple Levels)...\n";

  /* Clear previous state */
  book.cancel_order(3);

  /* Setup sellers */
  book.add_order(5, OrderType::ASK, 101, 10);
  book.add_order(6, OrderType::ASK, 102, 20);
  book.add_order(7, OrderType::ASK, 103, 30);

  /* Execute sweep */
  book.add_order(8, OrderType::BID, 105, 45);
  book.print_book();

  return 0;
}