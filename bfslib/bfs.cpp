#include <queue>
#include <cassert>
#include <atomic>
#include <algorithm>

#include "parlay/primitives.h"
#include "parlay/parallel.h"
#include "parlay/sequence.h"
#include "parlay/utilities.h"

#include "bfs.h"

#ifdef DEBUG
    #define debug(var) \
        do { \
            std::cout << #var << " = " << (var) << std::endl; \
        } while(0)

    template<typename T>
    void print_container(const T& container, const std::string& name) {
        std::cout << name << " = [";
        bool first = true;
        for (const auto& elem : container) {
            if (!first) std::cout << ", ";
            std::cout << elem;
            first = false;
        }
        std::cout << "]" << std::endl;
    }

    #define cdebug(var) \
        print_container(var, #var)

    #define print(var) \
        do { \
            std::cout << (var) << std::endl; \
        } while(0)

#else

    #define debug(var) do {} while(0)
    #define cdebug(var) do {} while(0)
    #define print(var) do {} while(0)

#endif

std::vector<int> seq_bfs(const graph& g, const int root) {
    const size_t n = g.size();
    if (n == 0) {
        return std::vector<int>{};
    }

    std::queue<int> q;
    std::vector<int> dist(n, -1);

    dist[root] = 0;
    q.push(root);

    while (!q.empty()) {
        const int v = q.front();
        assert(dist[v] != -1);
        q.pop();

        for (const int to : g[v]) {
            if (dist[to] == -1) {
                dist[to] = dist[v] + 1;
                q.push(to);
            }
        }
    }

    return dist;
}

std::vector<int> par_bfs(const graph& g, const int root) {
    const int n = g.size();
    if (n == 0) {
        return std::vector<int>();
    }

    auto sizes = parlay::map(g, [](const std::vector<int>& x) {
        return x.size();
    });

    const int m = parlay::reduce(sizes);

    parlay::sequence<std::atomic<int>> dist(n);
    parlay::parallel_for(0, n, [&dist](const int i) {
        dist[i].store(-1);
    });

    parlay::sequence<int> f(n), f_new(m), degs(m);
    int f_size = 1;
    f[0] = {root};

    dist[root].store(0);

    for (int i = 0; f_size != 0; ++i) {
        parlay::parallel_for(0, f_size, [i, &f, &g, &degs](const size_t j) {
            degs[j] = g[f[j]].size();
        });
        const int f_new_size = parlay::scan_inplace(degs.cut(0, f_size));

        parlay::parallel_for(0, f_new_size, [&f_new](const int j) {
            f_new[j] = -1;
        });

        parlay::parallel_for(0, f_size, [&](const size_t j) {
            const int new_dist = dist[f[j]].load() + 1;
            assert(new_dist > 0);

            // No parallel for here! x1.7 slower
            for (int k = 0; k < g[f[j]].size(); ++k) {
                const int to = g[f[j]][k];
                int expected = -1;

                if (dist[to].compare_exchange_strong(expected, new_dist)) {
                    f_new[degs[j] + k] = to;
                }
            }
        });

        f_size = parlay::filter_into_uninitialized(f_new.cut(0, f_new_size), f, [](const int x) {
            return x != -1;
        });
    }

    std::vector<int> result_dist(n);
    parlay::parallel_for(0, n, [&](size_t i) {
        result_dist[i] = dist[i].load();
    });

    return result_dist;
}
