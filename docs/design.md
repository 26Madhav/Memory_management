# OS Memory Management Simulator

This project simulates how an operating system manages memory.
It models physical memory allocation, CPU caches and virtual memory using clear data structures and algorithms.

The goal is to understand memory-management trade offs rather than build a real operating system.

## Memory Layout and Assumptions

Physical memory is represented as a contiguous block of fixed size.
Memory is divided into logical blocks, each having a start address, size and allocation status.

Since it is simulation, addresses are simulated offsets not real memory pointers.
The total memory size is kept fixed after initialization.

## Allocation Strategies

The simulator supports multiple dynamic memory allocation strategies:

- First Fit
- Best Fit
- Worst Fit
- Buddy Allocation

These strategies differ in how free memory blocks are selected
and help demonstrate fragmentation and performance trade-offs.

## Buddy System

The buddy allocator manages memory in power-of-two sized blocks.
When a request arrives, a larger block may be split until the
required size is reached.

On deallocation, adjacent buddy blocks are merged using XOR-based
address calculation.

## Cache Hierarchy

The simulator models a multi-level CPU cache (L1, L2, L3).
Each cache contains a fixed number of cache lines with tags
and valid bits.

Cache replacement policies supported include LRU and FIFO.
Caches store recently accessed memory data, not page table entries.

## Virtual Memory Model

Virtual memory is implemented using paging.
Virtual addresses are split into a page number and an offset.

A page table maps virtual pages to physical frames.
Each entry tracks validity and replacement information.

## Address Translation Flow

Address translation follows these steps:

1. A virtual address is generated.
2. The page table is checked for a valid mapping.
3. A physical address is formed using the frame number and offset.
4. The physical address is sent to the cache hierarchy.
5. On a cache miss, main memory is accessed.

Virtual Address → Page Table → Physical Address → Cache → Main Memory

## Limitations and Simplifications

This simulator focuses on correctness and clarity rather than realism.
Several real operating system features are intentionally simplified
or omitted.

- Timing, latency, and performance costs are not modeled.
  All operations are treated as logical steps.

- Translation Lookaside Buffers (TLBs) are not implemented.

These simplifications help keep the design easy to understand
while still demonstrating core memory-management concepts.
