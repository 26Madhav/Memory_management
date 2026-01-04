# Cache Simulation Test Cases

## Test 1: Cache Hit and Miss

Configuration:
- Cache size: 3 lines
- Replacement policy: LRU (default)
- Associativity: 1 (direct-mapped or equivalent)

Access Pattern:
A, B, C, A

Expected Behavior:
- First access to A, B, C results in cache misses
- Second access to A results in a cache hit
- Cache hit and miss counters update correctly

---

## Test 2: LRU Replacement Policy

Configuration:
- Cache size: 3 lines
- Replacement policy: LRU

Access Pattern:
A, B, C, D

Expected Behavior:
- Cache fills with A, B, C
- Accessing D evicts A (least recently used)
- Final cache contents: B, C, D

---

## Test 3: FIFO Replacement Policy

Configuration:
- Cache size: 3 lines
- Replacement policy: FIFO

Access Pattern:
A, B, C, D

Expected Behavior:
- Cache fills with A, B, C
- Accessing D evicts A (first inserted)
- Final cache contents: B, C, D
