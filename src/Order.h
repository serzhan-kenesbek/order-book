#include <cstdint>

enum class OrderType { BID, ASK };

/* Order is a struct since it's simple data aggregation */
struct Order {
  int id;
  OrderType type;
  int64_t price;
  int quantity;

  /* long long for required high time precision for Time Priority wihtout
   * risking integer overflow */
  long long timestamp;
};