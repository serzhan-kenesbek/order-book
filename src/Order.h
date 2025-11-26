enum class OrderType {
    BID,
    ASK
};

/* Order is a struct since it's simple data aggregation */
struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;

    /* long long for required high time precision for Time Priority wihtout risking integer overflow */
    long long timestamp;

    /* Add Constructor Later */
};