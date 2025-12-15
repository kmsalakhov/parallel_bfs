#include <queue>
#include <cassert>
#include <atomic>
#include <algorithm>

#include "parlay/primitives.h"
#include "parlay/parallel.h"
#include "parlay/sequence.h"
#include "parlay/utilities.h"

#include "bfs.h"

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
        // TODO(kasalakhov): no new struct
        return std::vector<int>();
    }

    std::vector<std::atomic<int>> dist(n);
    for (int i = 0; i < n; ++i) {
        dist[i].store(-1);
    }

    std::vector<parlay::sequence<int>> f;
    f.push_back({root});
    dist[root].store(0);

    for (int i = 0; !f[i].empty(); ++i) {
        // TODO(kasalakhov): optimize memory usage
        parlay::sequence<int> degs(f[i].size());
        parlay::parallel_for(0, f[i].size(), [i, &f, &g, &degs](const size_t j) {
            degs[j] = g[f[i][j]].size();
        });
        const int degs_sum = parlay::scan_inplace(degs);

        parlay::sequence<int> new_f(degs_sum, -1);
        // TODO(kasalakhov): as_const references?
        parlay::parallel_for(0, f[i].size(), [i, &g, &f, &degs, &dist, &new_f](const size_t j) {
            const int new_dist = dist[f[i][j]].load() + 1;
            assert(new_dist != 0);

            int expected = -1;
            // TODO(kasalakhov): parallel for?
            for (int k = 0; k < g[f[i][j]].size(); ++k) {
                const int to = g[f[i][j]][k];
                if (dist[to].compare_exchange_strong(expected, new_dist)) {
                    new_f[degs[j] + k] = to;
                }
            }
        });

        parlay::filter(new_f, [](const int x) {
            return x != -1;
        });

        f.push_back(new_f);
    }

    std::vector<int> result_dist(n);
    for (int i = 0; i < n; ++i) {
        result_dist[i] = dist[i].load();
    }

    return result_dist;
}
