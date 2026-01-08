# Test cases for memory management simulator

### Test case 1
### Demonstrates First Fit allocation
```cpp
init memory 256
set allocator first_fit
malloc 64
malloc 32
malloc 48
dump memory 
free 2
dump memory
stats
```

### Test case 2
### Demonstrates coalescing of adjacent free blocks

```cpp
init memory 256
set allocator first_fit
malloc 64
malloc 32
malloc 48
free 2
free 3
dump memory
stats
```

### Test case 3
### Demonstrates external fragmentation

```cpp
init memory 128
set allocator first_fit
malloc 32
malloc 32
malloc 32
free 2
malloc 40
dump memory
stats
```

### Test case 4
### Demonstrates buddy allocation

```cpp
init memory 256
set allocator buddy
malloc 20
malloc 60
malloc 100
dump memory
free 2
free 1
dump memory
stats
```

### Test case 5
### cache hit and miss behavior

```cpp
init memory 256
cache_init L1 64 16 1
cache_access 0x100
cache_access 0x110
cache_access 0x100
cache_access 0x120
cache_access 0x110
stats
```

### Test case 6
###  cache access and virtual memory translation

```cpp
init memory 512
set allocator best_fit
malloc 64
malloc 128
free 1
malloc 32
cache_init L1 128 32 1
cache_access 0x400
cache_access 0x410
cache_access 0x410
vm_init 16 4096 32768 LRU
vm_access 0 0x0000
vm_access 0 0x2000
vm_access 0 0x4000
dump memory
stats
```
