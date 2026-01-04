#include <iostream>
#include <string>
#include <iomanip>

#include "memory_simulator.h"
#include "cache.h"
#include "virtual_memory.h"
#include "stats.h"

using namespace std;

void printHelp() {
    cout << "\n===== Available Commands =====\n\n";

    cout << "[Memory]\n";
    cout << "  init memory <size>\n";
    cout << "  set allocator <FIRST_FIT | BEST_FIT | WORST_FIT | BUDDY>\n";
    cout << "  malloc <bytes>\n";
    cout << "  free <block_id>\n";
    cout << "  dump memory\n";
    cout << "  stats\n\n";

    cout << "[Cache]\n";
    cout << "  cache_init <L1|L2|L3> <cache_size> <block_size> <associativity>\n";
    cout << "  cache_access <hex_address>\n\n";

    cout << "[Virtual Memory]\n";
    cout << "  vm_init <vbits> <page_size> <phys_mem> <LRU|FIFO>\n";
    cout << "  vm_access <pid> <hex_virtual_address>\n\n";

    cout << "[Utility]\n";
    cout << "  help\n";
    cout << "  exit\n\n";
}

int main() {
    Stats stats;
    MemorySimulator mem(&stats);

    Cache *L1 = nullptr, *L2 = nullptr, *L3 = nullptr;
    VirtualMemory *vm = nullptr;

    string cmd;
    cout << "OS Memory + Cache Simulator\n";
    cout<< "Type 'help' for a list of commands\n";

    while (true) {
        cout << "> ";
        cin >> cmd;

        if (cmd == "init") {
            string what;
            size_t size;
            cin >> what >> size;  
            mem.init(size);
        }

        else if (cmd == "set") {
            string what, type;
            cin >> what >> type;   
            mem.setAllocator(type);
        }

        else if (cmd == "malloc") {
            size_t size;
            cin >> size;

            int before = mem.getNextId();
            mem.allocate(size);
            int after = mem.getNextId();

            if (after > before) {
                cout << "Allocated block id=" << before
                     << " at address=0x"
                     << hex << setw(4) << setfill('0')
                     << mem.getBlockAddress(before)
                     << dec << "\n";
            }
        }

        else if (cmd == "free") {
            int id;
            cin >> id;
            mem.deallocate(id);
            cout << "Block " << id << " freed and merged\n";
        }

        else if (cmd == "dump") {
            string what;
            cin >> what;   
            mem.dump();
        }

        else if (cmd == "stats") {
            cout << "Total memory: " << mem.getTotalMemory() << "\n";
            size_t free_mem;

            if (mem.getAllocatorType() == AllocatorType::BUDDY)
                free_mem = mem.totalFreeMemoryBuddy();
            else
                free_mem = mem.totalFreeMemory();

            cout << "Free memory : " << free_mem << "\n";
            cout << "Used memory : " << mem.getTotalMemory() - free_mem << "\n";

            stats.print_summary();
        }

        else if (cmd == "cache_init") {
            size_t c, b, a;
            string lvl;
            cin >> lvl >> c >> b >> a;

            if (lvl == "L1") L1 = new Cache(c, b, a, &stats);
            else if (lvl == "L2") L2 = new Cache(c, b, a, &stats);
            else if (lvl == "L3") L3 = new Cache(c, b, a, &stats);

            cout << lvl << " cache initialized\n";
        }

        else if (cmd == "cache_access") {
            size_t addr;
            cin >> hex >> addr >> dec;

            if (L1 && L1->access(addr)) continue;
            if (L2 && L2->access(addr)) continue;
            if (L3) L3->access(addr);
        }

        else if (cmd == "vm_init") {
            size_t vbits, psize, pmem;
            string pol;
            cin >> vbits >> psize >> pmem >> pol;

            PagePolicy p = (pol == "LRU") ? PagePolicy::LRU : PagePolicy::FIFO;
            vm = new VirtualMemory(vbits, psize, pmem, p, &stats);

            cout << "Virtual Memory initialized\n";
        }

        else if (cmd == "vm_access") {
            int pid;
            size_t vaddr;
            cin >> pid >> hex >> vaddr >> dec;

            if (!vm) {
                cout << "VM not initialized\n";
                continue;
            }

            size_t paddr = vm->translate(pid, vaddr);

            if (L1 && L1->access(paddr)) continue;
            if (L2 && L2->access(paddr)) continue;
            if (L3) L3->access(paddr);
        }

        else if (cmd == "exit") {
            break;
        }
        else if (cmd == "help") {
            printHelp();
        }

        else {
            cout << "Unknown command\n";
        }
    }

    return 0;
}
