#pragma once
#include <cstddef>
#include <vector>

struct CacheLine {
    bool valid = false;
    size_t tag = 0;
    size_t time = 0;
};

// Forward declaration
class Stats;

class Cache {
public:
    Cache(size_t c, size_t b, size_t a, Stats* stats);
    bool access(size_t addr);

private:
    Stats* stats;   // telemetry sink

    size_t cache_size, block_size, assoc;
    size_t sets;
    size_t timer = 0;
    std::vector<std::vector<CacheLine>> data;
};
