# Memory Allocation Design

This document describes how physical memory allocation is implemented
in the simulator. It explains the block structure, supported allocation
strategies, and how fragmentation is handled.

## Memory Block Structure

Physical memory is represented as a list of blocks.
Each block contains:

- Start address
- Block size
- Allocation status (free or used)
- Allocation ID

Blocks are kept in address order, which makes it easy to split
and merge blocks during allocation and deallocation.

## Standard Allocation Strategies

The simulator supports First Fit, Best Fit, and Worst Fit allocation.

- First Fit selects the first free block large enough to satisfy the request.
- Best Fit selects the smallest free block that can satisfy the request.
- Worst Fit selects the largest available free block.

All three strategies use the same underlying block list and differ
only in how the free block is chosen.

## Block Splitting

When a free block is larger than the requested size, it is split
into two blocks:

- One allocated block of the requested size
- One remaining free block with the leftover memory

This ensures that memory is used efficiently and prevents
unnecessary internal fragmentation.

## Deallocation and Coalescing

When a block is freed, it is marked as free and assigned an invalid ID.
The allocator then checks neighboring blocks.

If adjacent blocks are also free, they are merged into a single
larger free block. This process is called coalescing.

Coalescing reduces external fragmentation and helps keep
larger contiguous blocks available for future allocations.

## Fragmentation Tracking

The simulator tracks fragmentation-related statistics such as:

- Total free memory
- Size of the largest free block

These metrics help observe how different allocation strategies
affect memory usage over time.

## Buddy Allocation

The buddy allocator manages memory using power-of-two sized blocks.
All memory sizes are rounded up to the nearest power of two.

If no block of the required size is available, a larger block is
recursively split until the requested size is reached.

## Buddy Deallocation and Merging

When a buddy-allocated block is freed, the allocator checks whether
its adjacent buddy is also free.

Buddy addresses are computed using a simple XOR operation on the
block address and its size.

If the buddy is free, the two blocks are merged into a larger block.
This process repeats until no further merge is possible.

## Design Choices

- Standard allocators and the buddy allocator are implemented separately to keep the logic simple and clear.

- Coalescing is performed eagerly during deallocation to reduce fragmentation.

- The buddy allocator uses separate free lists for each block size, improving allocation efficiency.