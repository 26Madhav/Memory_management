#pragma once
#include <cstddef>
#include <unordered_map>
#include <vector>


//    Page Table Structures

struct PageTableEntry {
    bool present = false;
    size_t frame = 0;
    size_t last_used = 0;   
};


//    TLB Entry

struct TLBEntry {
    int pid;
    size_t vpn;
    size_t pfn;
    bool valid;
    size_t last_used;     
};


//    Physical Frame

struct Frame {
    bool occupied = false;
    int pid = -1;
    size_t vpn = 0;
    size_t loaded_time = 0; 
};


enum class PagePolicy {
    FIFO,
    LRU
};


class Stats;

class VirtualMemory {
public:
    VirtualMemory(size_t vbits,
                  size_t psize,
                  size_t pmem,
                  PagePolicy policy,
                  Stats* stats);

    size_t translate(int pid, size_t virtual_addr);

private:
    Stats* stats; 

    size_t tlb_size;
    std::vector<TLBEntry> tlb;

    size_t page_size;
    size_t virtual_bits;   
    size_t physical_mem;
    size_t frames_count;

    size_t timer = 0;


    std::vector<Frame> frames;
    std::unordered_map<int, std::vector<PageTableEntry>> page_tables;

    PagePolicy policy;

    void handlePageFault(int pid, size_t vpn);
    size_t selectVictim();
    void evict(size_t frame_id);
    void loadFrame(size_t frame_id, int pid, size_t vpn);

    bool tlbLookup(int pid, size_t vpn, size_t& pfn);
    void tlbInsert(int pid, size_t vpn, size_t pfn);
};
