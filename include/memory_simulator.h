#ifndef MEMORY_SIMULATOR_H
#define MEMORY_SIMULATOR_H

#include <list>
#include <unordered_map>
#include <string>
#include <cstddef>

class Stats;

// ---------- Block ---------- 
struct Block {
    size_t start;
    size_t size;
    bool free;
    int id;
};

// ---------- Allocator Type ---------- 
enum class AllocatorType {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT,
    BUDDY
};


//    ---------------- MEMORY SIMULATOR -------------------


class MemorySimulator {
private:
    Stats* stats;

    size_t total_memory;
    std::list<Block> blocks;

    std::unordered_map<size_t, std::list<Block>> buddy_free;
    std::unordered_map<int, Block> buddy_allocated;

    AllocatorType allocator;
    int next_id;

    bool isPowerOfTwo(size_t x);
    size_t nextPowerOfTwo(size_t x);

    void allocateStandard(size_t size);
    void allocateBuddy(size_t size);
    void freeBuddy(Block block);

public:
    explicit MemorySimulator(Stats* s = nullptr);

    void init(size_t size);
    void setAllocator(const std::string& type);
    void allocate(size_t size);
    void deallocate(int id);
    void dump() const;
    AllocatorType getAllocatorType() const;


    size_t totalFreeMemory() const;
    size_t largestFreeBlock() const;
    size_t totalFreeMemoryBuddy() const;
    int getNextId() const;
    size_t getTotalMemory() const;
    size_t getBlockAddress(int id) const;
};

#endif
