#include <algorithm>
#include <random>
#include <vector>
#include <cassert>

#include "bfs.h"

#include "parlay/primitives.h"
#include "parlay/parallel.h"
#include <benchmark/benchmark.h>

int xyz_to_id(const int x, const int y, const int z, const int size) {
    // size = 5
    // (1, 0, 0) = 1 * 25 + 0 + 0 = 25
    // (0, 0, 1) = 0 + 0 + 1 = 1
    // (0, 1, 1) = 0 + 5 + 1 = 6
    // (0, 4, 4) = 0 + 20 + 4 = 24
    // 
    return x * size * size + y * size + z;
}

std::vector<int> generate_neighbours(int x, int y, int z, int size) {
    std::vector<int> res;

    // Соседи по X
    if (x > 0) res.push_back(xyz_to_id(x-1, y, z, size));
    if (x < size-1) res.push_back(xyz_to_id(x+1, y, z, size));

    // Соседи по Y
    if (y > 0) res.push_back(xyz_to_id(x, y-1, z, size));
    if (y < size-1) res.push_back(xyz_to_id(x, y+1, z, size));

    // Соседи по Z
    if (z > 0) res.push_back(xyz_to_id(x, y, z-1, size));
    if (z < size-1) res.push_back(xyz_to_id(x, y, z+1, size));
    
    return res;
}

graph generate_graph(size_t size) {
    graph g(size * size * size);

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            for (int z = 0; z < size; ++z) {
                const int id = xyz_to_id(x, y, z, size);
                g[id] = generate_neighbours(x, y, z, size);
            }
        }
    }
    
    return g;
}

void BM_SequentialBfs(benchmark::State& state) {
    const size_t size = state.range(0);
    auto g = generate_graph(size);
    
    for (auto _ : state) {
        auto result = seq_bfs(g);
        benchmark::DoNotOptimize(result);
    }
    
    state.SetBytesProcessed(state.iterations() * size * size * size * sizeof(int));
}

void BM_ParallelBfs(benchmark::State& state) {
    const int num_threads = parlay::num_workers();
    
    const size_t size = state.range(0);
    auto g = generate_graph(size);
    
    for (auto _ : state) {
        auto result = par_bfs(g);
        benchmark::DoNotOptimize(result);
    }
    
    state.SetBytesProcessed(state.iterations() * size * size * size * sizeof(int));
    state.SetLabel(std::string("Threads: ") + std::to_string(num_threads));
}

BENCHMARK(BM_SequentialBfs)->Arg(10);
BENCHMARK(BM_ParallelBfs)->Arg(10);

BENCHMARK_MAIN();