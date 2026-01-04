#include "memory_simulator.h"
#include "stats.h"

#include <iostream>
#include <iomanip>
#include <algorithm>


MemorySimulator::MemorySimulator(Stats* s)
    : stats(s),
      total_memory(0),
      allocator(AllocatorType::FIRST_FIT),
      next_id(1) {}


bool MemorySimulator::isPowerOfTwo(size_t x) {
    return x && !(x & (x - 1));
}

size_t MemorySimulator::nextPowerOfTwo(size_t x) {
    size_t p = 1;
    while (p < x) p <<= 1;
    return p;
}


void MemorySimulator::init(size_t size) {
    total_memory = size;
    blocks.clear();
    buddy_free.clear();
    buddy_allocated.clear();
    next_id = 1;

    blocks.push_back({0, size, true, -1});

    if (isPowerOfTwo(size)) {
        buddy_free[size].push_back({0, size, true, -1});
    }
}


void MemorySimulator::setAllocator(const std::string& type) {
    if (type == "first_fit") allocator = AllocatorType::FIRST_FIT;
    else if (type == "best_fit") allocator = AllocatorType::BEST_FIT;
    else if (type == "worst_fit") allocator = AllocatorType::WORST_FIT;
    else if (type == "buddy") allocator = AllocatorType::BUDDY;
}


void MemorySimulator::allocateStandard(size_t size) {
    auto chosen = blocks.end();

    if (allocator == AllocatorType::FIRST_FIT) {
        for (auto it = blocks.begin(); it != blocks.end(); ++it)
            if (it->free && it->size >= size) { chosen = it; break; }
    }
    else if (allocator == AllocatorType::BEST_FIT) {
        size_t best = SIZE_MAX;
        for (auto it = blocks.begin(); it != blocks.end(); ++it)
            if (it->free && it->size >= size && it->size < best) {
                best = it->size;
                chosen = it;
            }
    }
    else if (allocator == AllocatorType::WORST_FIT) {
        size_t worst = 0;
        for (auto it = blocks.begin(); it != blocks.end(); ++it)
            if (it->free && it->size >= size && it->size > worst) {
                worst = it->size;
                chosen = it;
            }
    }

    if (chosen == blocks.end())
        return;

    Block alloc = {chosen->start, size, false, next_id++};

    if (chosen->size > size) {
        Block rem = {chosen->start + size, chosen->size - size, true, -1};
        *chosen = alloc;
        blocks.insert(std::next(chosen), rem);
    } else {
        *chosen = alloc;
    }

    if (stats)
        stats->record_alloc(size, size);
}



void MemorySimulator::allocateBuddy(size_t size) {
    if (!isPowerOfTwo(total_memory))
        return;

    size_t req = nextPowerOfTwo(size);
    size_t curr = req;

    while (curr <= total_memory && buddy_free[curr].empty())
        curr <<= 1;

    if (curr > total_memory)
        return;

    Block block = buddy_free[curr].front();
    buddy_free[curr].pop_front();

    while (curr > req) {
        curr >>= 1;
        Block right = {block.start + curr, curr, true, -1};
        buddy_free[curr].push_back(right);
        block.size = curr;
    }

    block.free = false;
    block.id = next_id++;
    buddy_allocated[block.id] = block;

    if (stats)
        stats->record_alloc(size, block.size);
}

void MemorySimulator::freeBuddy(Block block) {
    size_t size = block.size;
    size_t addr = block.start;

    while (size < total_memory) {
        size_t buddy_addr = addr ^ size;

        auto& freelist = buddy_free[size];
        auto it = std::find_if(
            freelist.begin(), freelist.end(),
            [&](const Block& b) { return b.start == buddy_addr; }
        );

        if (it == freelist.end())
            break;

        addr = std::min(addr, buddy_addr);
        freelist.erase(it);
        size <<= 1;
    }

    buddy_free[size].push_back({addr, size, true, -1});
}

void MemorySimulator::allocate(size_t size) {
    if (allocator == AllocatorType::BUDDY)
        allocateBuddy(size);
    else
        allocateStandard(size);
}

void MemorySimulator::deallocate(int id) {

    if (allocator == AllocatorType::BUDDY) {
        auto it = buddy_allocated.find(id);
        if (it == buddy_allocated.end())
            return;

        Block block = it->second;
        buddy_allocated.erase(it);

        block.free = true;
        block.id = -1;
        freeBuddy(block);

        if (stats)
            stats->record_free();
        return;
    }

    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        if (!it->free && it->id == id) {
            it->free = true;
            it->id = -1;

            auto next = std::next(it);
            if (next != blocks.end() && next->free) {
                it->size += next->size;
                blocks.erase(next);
            }

            if (it != blocks.begin()) {
                auto prev = std::prev(it);
                if (prev->free) {
                    prev->size += it->size;
                    blocks.erase(it);
                }
            }

            if (stats)
                stats->record_free();
            return;
        }
    }
}


void MemorySimulator::dump() const {
    std::cout << "\n================ MEMORY DUMP ================\n";

    if (allocator != AllocatorType::BUDDY) {
        // Reset formatting defensively
        std::cout << std::dec << std::setfill(' ');

        std::cout << std::left
                  << std::setw(12) << "Start"
                  << std::setw(12) << "Size"
                  << std::setw(12) << "Status"
                  << std::setw(12) << "ID"
                  << "\n";

        std::cout << std::string(48, '-') << "\n";

        for (const auto& b : blocks) {
            std::cout << std::left
                      << std::setw(12) << b.start
                      << std::setw(12) << b.size
                      << std::setw(12) << (b.free ? "FREE" : "USED");

            if (b.free)
                std::cout << std::setw(12) << "-";
            else
                std::cout << std::setw(12) << b.id;

            std::cout << "\n";
        }
    }
    else {
        std::cout << "[Buddy Free Lists]\n";
        for (const auto& [sz, lst] : buddy_free) {
            std::cout << "Size " << sz << " : ";
            for (const auto& b : lst)
                std::cout << "[" << b.start << "] ";
            std::cout << "\n";
        }
    }

    std::cout << "============================================\n";
}


size_t MemorySimulator::totalFreeMemory() const {
    if (allocator == AllocatorType::BUDDY)
        return 0;

    size_t total = 0;
    for (const auto& b : blocks)
        if (b.free) total += b.size;
    return total;
}

size_t MemorySimulator::largestFreeBlock() const {
    if (allocator == AllocatorType::BUDDY)
        return 0;

    size_t largest = 0;
    for (const auto& b : blocks)
        if (b.free) largest = std::max(largest, b.size);
    return largest;
}

size_t MemorySimulator::totalFreeMemoryBuddy() const {
    size_t total = 0;
    for (const auto& [sz, lst] : buddy_free) {
        total += sz * lst.size();
    }
    return total;
}


int MemorySimulator::getNextId() const {
    return next_id;
}

size_t MemorySimulator::getTotalMemory() const {
    return total_memory;
}

AllocatorType MemorySimulator::getAllocatorType() const {
    return allocator;
}

size_t MemorySimulator::getBlockAddress(int id) const {

    if (allocator == AllocatorType::BUDDY) {
        auto it = buddy_allocated.find(id);
        if (it != buddy_allocated.end())
            return it->second.start;
        return static_cast<size_t>(-1);
    }

    for (const auto& b : blocks) {
        if (!b.free && b.id == id)
            return b.start;
    }

    return static_cast<size_t>(-1);
}
