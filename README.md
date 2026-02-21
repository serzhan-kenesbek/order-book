# In development! Adding:
* Volume tracking over price levels (Add a Limit struct)
* Decoupled Execution Logic (currently it is in the add_order function)
* Remove Floating Point Arithmetic
* Add Benchmarking
* Eliminate Dynamic Memory Allocaiton
* Improve CPU Cache Locality
* Advanced HFT: Threading & Lock-Free Data Structures

# Goals for the project
* Brush up C++ coding skills
* Understand Optimization (on a Hardware level) critical for High Frequency Trading
* Learn C++ (in this case C++17) on a deeper level
* See if I enjoy this and decide if I want to take the related courses (currently in the Winter Semester) in the upcoming semesters for deeper understanding and to refine my knowledge

# Why C++ for the Limit Order Book

* C++ provides deterministic, ultra-low latency execution.
* It allows manual direct control over memory/hardware resources preventing unpredictable pauses common in languages with a garbage collector (GC) that can halt the entire system for milliseconds. This is detrimental in High-Frequency Trading (HFT).
* Python, Java, and JS use a GC, Rust uses the Ownership and Borrow Checker System, and C++ relies on manual management and smart pointers.
* A GC automatically runs periodically to find and reclaim memory that is no longer actively used by the application. This frees from manual tracking, but introduces unpredictable latency spikes.
* Rust uses an Ownership System, which enforces memory cleanup at compile time. It uses strict rules to ensure that memory is automatically and deterministically freed when its owner goes out of scope, guaranteeing safety without runtime overhead or latency.
* While Rust offers strong safety guarantees, C++ is prioritized because its decades-long dominance ensures vast, battle-tested library ecosystems and direct compatibility with the existing infrastructure of HFT.

# Order Book

The OrderBook must perform three distinct functions with maximum efficiency:

* The ID Lookup - given an id find an order instantly.
    * Options: Hash Table
* The Ordering - the book must maintain all Bids (Buys) sorted from highest price to lowest, and all Asks (Sells) sorted from lowest price to highest. Efficiently insert new prices and instantly find the best price.
    * Options: The Tree Family (BSTs, AVL Trees, or Red-Black Trees), The Heap Family (Min-Heaps or Max-Heaps like Priority Queues in C++)
    * Chose: RBT. O(log n) operations with fewer rotations than AVL, ideal for constant insertions/cancellations. Maintains sorted order automatically, O(1) best price access, and supports market depth traversal.
* Time Priority - at any single price orders must be executed in the order they arrived (FIFO).
    * Options: Queue implementation (Doubly Linked List, Double-Ended Queue, Vector)
    * Chose: User should be able to cancel their orders before they get matched. To make sure it's instantaneous (O(1)) doubly-linked list makes the most sense.

Features:
* Add Order
* Cancel Order
* Print Book
* Get Best Bid
* Get Best Ask
