# Manual Test Execution

1. Build the simulator using `make`
2. Run the simulator
3. Execute test commands listed in memory_tests.md

# Correctness Criteria

The simulator is considered correct if:

- No allocated memory blocks overlap
- Total allocated + free memory equals total memory size
- Free blocks are merged correctly when adjacent
- Buddy blocks merge only when valid buddies are free
- Cache hits do not access lower memory levels
- Address translation occurs before cache access
