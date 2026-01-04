# OS Memory Management Simulator

A user space simulator that models how an operating system manages memory.
It includes physical memory allocation, CPU cache simulation, and virtual
memory using paging.

The project focuses on correctness and clarity rather than low-level
hardware or kernel implementation.

---

## Features

### Physical Memory Allocation
- First Fit, Best Fit, and Worst Fit allocation strategies
- Buddy allocation using power-of-two blocks
- Block splitting and coalescing
- Fragmentation tracking and statistics

### Cache Simulation
- Multi-level cache hierarchy (L1, L2, L3)
- Cache lines with tags and valid bits
- Replacement policies: LRU and FIFO
- Cache hit and miss tracking

### Virtual Memory
- Paging-based virtual memory model
- Page tables with valid bits and frame numbers
- Logical page replacement
- Clear separation from cache simulation

---

## Project Structure

```txt
memory-simulator/
├── src/
│   ├── allocator/
│   ├── buddy/
│   ├── cache/
│   ├── virtual_memory/
│   └── main.cpp
├── include/
├── docs/
│   ├── design.md
│   ├── allocator.md
│   ├── cache.md
│   └── virtual_memory.md
├── tests/
├── Makefile
└── README.md
```
---

## Build and Run

### Build
make

./memory_sim


## Example command
The simulator provides a simple command-based interface to interact with memory allocation and inspection.
init memory 1024
set allocator first_fit
malloc 128
malloc 256
free 1
dump memory
stats

init initializes physical memory
set selects the allocation strategy
malloc allocates memory
free deallocates memory using allocation ID
dump prints current memory layout
stats shows fragmentation statistics

## Testing

The simulator was validated using structured behavioral test cases covering
memory allocation, cache replacement, and virtual memory translation.

Test specifications:
- `tests/memory_test.md` — Validates allocation strategies, coalescing, and buddy allocator behavior
- `tests/cache_test.md` — Validates cache hit/miss logic and LRU/FIFO replacement policies

Tests are executed manually via the interactive CLI to verify correctness
and internal invariants.

## Limitations
No timing or latency modeling
No TLB or cache coherence simulation
Single-process execution
No concurrency support
