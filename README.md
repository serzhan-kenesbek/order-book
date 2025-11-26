# Why C++ for the Limit Order Book

* C++ provides deterministic, ultra-low latency execution.
* It allows manual direct control over memory/hardware resources preventing unpredictable pauses common in languages with a garbage collector (GC) that can halt the entire system for milliseconds. This is detrimental in High-Frequency Trading (HFT).
* Python, Java, and JS use a GC, Rust uses the Ownership and Borrow Checker System, and C++ relies on manual management and smart pointers.
* A GC automatically runs periodically to find and reclaim memory that is no longer actively used by the application. This frees from manual tracking, but introduces unpredictable latency spikes.
* Rust uses an Ownership System, which enforces memory cleanup at compile time. It uses strict rules to ensure that memory is automatically and deterministically freed when its owner goes out of scope, guaranteeing safety without runtime overhead or latency.
* While Rust offers strong safety guarantees, C++ is prioritized because its decades-long dominance ensures vast, battle-tested library ecosystems and direct compatibility with the existing infrastructure of HFT.