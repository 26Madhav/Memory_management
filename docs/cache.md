# Cache Simulation Design

This document describes the design of the CPU cache simulation.
The goal is to model how caches store recently accessed data and
how replacement policies affect cache behavior.

## Cache Model

The simulator models a CPU cache as a collection of cache lines.
Each cache has a fixed size and a fixed number of lines.

Each cache line stores:
- A tag
- A valid bit
- Replacement metadata

## Cache Hierarchy

The simulator supports a multi-level cache hierarchy (L1, L2, L3).
Memory accesses first check the L1 cache and then proceed to lower
levels on a miss.

Each cache level operates independently and follows the same
replacement logic.

## Cache Access Flow

When a memory access occurs:

1. The cache is checked for a matching tag.
2. If a valid line is found, it is a cache hit.
3. On a miss, the next cache level is accessed.
4. If all cache levels miss, main memory is accessed.

Cache hits avoid accessing main memory.

## Replacement Policies

The simulator supports the following cache replacement policies:

- Least Recently Used (LRU):
  The cache line that was least recently accessed is replaced.

- First-In-First-Out (FIFO):
  The cache line that was inserted earliest is replaced.

Replacement metadata is updated on each cache access.

## Cache vs Virtual Memory

Caches and virtual memory serve different purposes.

- Caches improve performance by storing recently accessed data.
- Virtual memory provides address translation and isolation.

The cache stores memory data, not page table entries.
Address translation is handled before cache access.

## Design Choices and Simplifications

- Cache timing and latency are not modeled.
- Cache coherence is not implemented.
- Write-back and write-through policies are not distinguished.

The focus is on correctness of cache behavior rather than
hardware-level accuracy.