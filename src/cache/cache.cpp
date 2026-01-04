#include "cache.h"
#include "stats.h"

#include <algorithm>

Cache::Cache(size_t c, size_t b, size_t a, Stats* s)
    : stats(s),
      cache_size(c),
      block_size(b),
      assoc(a),
      timer(0) {
    sets = cache_size / (block_size * assoc);
    data.resize(sets, std::vector<CacheLine>(assoc));
}

bool Cache::access(size_t addr) {
    timer++;

    size_t block = addr / block_size;
    size_t idx   = block % sets;
    size_t tag   = block / sets;


    for (auto& line : data[idx]) {
        if (line.valid && line.tag == tag) {
            if (stats) {
                stats->record_cache_hit();
            }
            line.time = timer; 
            return true;
        }
    }

    if (stats) {
        stats->record_cache_miss();
    }

    auto& set = data[idx];
    auto victim = std::min_element(
        set.begin(), set.end(),
        [](const CacheLine& a, const CacheLine& b) {
            if (!a.valid) return true;
            if (!b.valid) return false;
            return a.time < b.time;
        });

    *victim = {true, tag, timer};
    return false;
}
