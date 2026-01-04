#pragma once
#include <cstddef>
#include <iostream>

class Stats {
public:
    // Allocation 
    size_t alloc_requests = 0;
    size_t free_requests = 0;
    size_t bytes_requested = 0;
    size_t bytes_allocated = 0;

    //Cache
    size_t cache_accesses = 0;
    size_t cache_hits = 0;
    size_t cache_misses = 0;

    //Virtual Memory 
    size_t vm_accesses = 0;
    size_t page_faults = 0;
    size_t page_hits = 0;

    size_t tlb_accesses = 0;
    size_t tlb_hits = 0;
    size_t tlb_misses = 0;

    void record_tlb_hit();
    void record_tlb_miss();

    void record_alloc(size_t requested, size_t allocated);
    void record_free();


    void record_cache_hit();
    void record_cache_miss();

    void record_page_fault();
    void record_page_hit();
    void print_summary() const;
};
