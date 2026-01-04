# Memory Allocation Test Cases

## Test 1: Basic Allocation and Free

Input:
init memory 1024
set allocator first_fit
malloc 200
malloc 300
free 1
dump memory

Expected Behavior:
- Block with ID 1 is freed
- Remaining blocks stay unchanged
- No overlapping blocks

## Test 2: Coalescing on Free

Input:
init memory 1024
set allocator first_fit
malloc 200
malloc 300
free 1
free 2
dump memory

Expected Behavior:
- Two adjacent free blocks are merged
- One large free block is created

## Test 3: Buddy Allocation Split and Merge

Input:
init memory 1024
set allocator buddy
malloc 100
malloc 200
free 1
free 2
dump

Expected Behavior:
- Blocks split to nearest power of two
- Freed buddy blocks are merged recursively
