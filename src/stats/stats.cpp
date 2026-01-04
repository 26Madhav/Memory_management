#include "stats.h"

// -------- Allocation --------
void Stats::record_alloc(size_t requested, size_t allocated) {
    alloc_requests++;
    bytes_requested += requested;
    bytes_allocated += allocated;
}

void Stats::record_free() {
    free_requests++;
}

// -------- Cache --------
void Stats::record_cache_hit() {
    cache_accesses++;
    cache_hits++;
}

void Stats::record_cache_miss() {
    cache_accesses++;
    cache_misses++;
}

// -------- Virtual Memory --------
void Stats::record_page_fault() {
    page_faults++;
}

void Stats::record_page_hit() {
    page_hits++;
}

// -------- Reporting --------
void Stats::print_summary() const {
    std::cout << "\n===== Simulation Statistics =====\n";

    std::cout << "\n[Allocation]\n";
    std::cout << "Alloc requests      : " << alloc_requests << "\n";
    std::cout << "Free requests       : " << free_requests << "\n";
    std::cout << "Bytes requested     : " << bytes_requested << "\n";
    std::cout << "Bytes allocated     : " << bytes_allocated << "\n";
    std::cout << "Internal fragmentation : "
              << (bytes_allocated - bytes_requested) << "\n";

    std::cout << "\n[Cache]\n";
    std::cout << "Accesses : " << cache_accesses << "\n";
    std::cout << "Hits     : " << cache_hits << "\n";
    std::cout << "Misses   : " << cache_misses << "\n";
    if (cache_accesses)
        std::cout << "Hit rate : "
                  << (double)cache_hits / cache_accesses << "\n";

    std::cout << "\n[Virtual Memory]\n";
    std::cout << "VM accesses : " << vm_accesses << "\n";
    std::cout << "Page hits   : " << page_hits << "\n";
    std::cout << "Page faults : " << page_faults << "\n";

    std::cout << "\n[TLB]\n";
    std::cout << "TLB accesses : " << tlb_accesses << "\n";
    std::cout << "TLB hits     : " << tlb_hits << "\n";
    std::cout << "TLB misses   : " << tlb_misses << "\n";
    std::cout << "=================================\n";
}
// -------- TLB --------
void Stats::record_tlb_hit() {
    tlb_accesses++;
    tlb_hits++;
}

void Stats::record_tlb_miss() {
    tlb_accesses++;
    tlb_misses++;
}
