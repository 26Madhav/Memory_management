#include "virtual_memory.h"
#include "stats.h"

#include <climits>
#include <algorithm>

VirtualMemory::VirtualMemory(size_t vbits,
                             size_t psize,
                             size_t pmem,
                             PagePolicy p,
                             Stats* s)
    : stats(s),
      tlb_size(16),
      page_size(psize),
      virtual_bits(vbits),
      physical_mem(pmem),
      frames_count(0),
      timer(0),
      policy(p)
{
    (void)virtual_bits;
    frames_count = physical_mem / page_size;
    frames.resize(frames_count);
    tlb.resize(tlb_size, {0, 0, 0, false, 0});
}

size_t VirtualMemory::translate(int pid, size_t virtual_addr) {
    timer++;
    if (stats) {
        stats->vm_accesses++;
    }
    size_t vpn = virtual_addr / page_size;
    size_t offset = virtual_addr % page_size;
    size_t pfn;

    if (tlbLookup(pid, vpn, pfn)) {
        return pfn * page_size + offset;
    }

    auto &pt = page_tables[pid];
    if (pt.size() <= vpn)
        pt.resize(vpn + 1);

    if (pt[vpn].present) {
        if (stats) {
            stats->record_page_hit();
        }
        pt[vpn].last_used = timer;

        tlbInsert(pid, vpn, pt[vpn].frame);

        return pt[vpn].frame * page_size + offset;
    }

    if (stats) {
        stats->record_page_fault();
    }

    handlePageFault(pid, vpn);
    pt[vpn].last_used = timer;

    tlbInsert(pid, vpn, pt[vpn].frame);

    return pt[vpn].frame * page_size + offset;
}

void VirtualMemory::handlePageFault(int pid, size_t vpn) {
    for (size_t i = 0; i < frames.size(); i++) {
        if (!frames[i].occupied) {
            loadFrame(i, pid, vpn);
            return;
        }
    }

    size_t victim = selectVictim();
    evict(victim);
    loadFrame(victim, pid, vpn);
}

size_t VirtualMemory::selectVictim() {
    size_t victim = 0;

    if (policy == PagePolicy::FIFO) {
        size_t oldest = SIZE_MAX;
        for (size_t i = 0; i < frames.size(); i++) {
            if (frames[i].loaded_time < oldest) {
                oldest = frames[i].loaded_time;
                victim = i;
            }
        }
    }
    else { // LRU
        size_t least = SIZE_MAX;
        for (size_t i = 0; i < frames.size(); i++) {
            auto &pte =
                page_tables[frames[i].pid][frames[i].vpn];
            if (pte.last_used < least) {
                least = pte.last_used;
                victim = i;
            }
        }
    }

    return victim;
}

void VirtualMemory::evict(size_t frame_id) {
    auto &f = frames[frame_id];
    page_tables[f.pid][f.vpn].present = false;
}

void VirtualMemory::loadFrame(size_t frame_id, int pid, size_t vpn) {
    frames[frame_id] = {true, pid, vpn, timer};

    auto &pte = page_tables[pid][vpn];
    pte.present = true;
    pte.frame = frame_id;
}

bool VirtualMemory::tlbLookup(int pid, size_t vpn, size_t& pfn) {
    for (auto& entry : tlb) {
        if (entry.valid && entry.pid == pid && entry.vpn == vpn) {
            entry.last_used = timer;
            pfn = entry.pfn;
            if (stats) stats->record_tlb_hit();
            return true;
        }
    }

    if (stats) stats->record_tlb_miss();
    return false;
}

void VirtualMemory::tlbInsert(int pid, size_t vpn, size_t pfn) {
    for (auto& entry : tlb) {
        if (!entry.valid) {
            entry = {pid, vpn, pfn, true, timer};
            return;
        }
    }

    auto victim = std::min_element(
        tlb.begin(), tlb.end(),
        [](const TLBEntry& a, const TLBEntry& b) {
            return a.last_used < b.last_used;
        });

    *victim = {pid, vpn, pfn, true, timer};
}
