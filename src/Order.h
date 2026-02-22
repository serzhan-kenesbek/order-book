#include <cstdint>

enum class OrderType { BID, ASK };

/* Order is a struct since it's simple data aggregation */
struct Order {
  int id;
  OrderType type;
  int64_t price;
  int quantity;

  /*
   * Timestamp is required for regulatory tracking/auditing.
   * However, to avoid slow OS system calls on the Critical Path,
   * the outer Network/Gateway layer populates this BEFORE
   * calling the Matching Engine kernel.
   */
  int64_t timestamp;
};