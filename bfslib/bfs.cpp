#include <queue>
#include <cassert>

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
    // TODO(kasalakhov): changed to parallel
    return seq_bfs(g, root);
}
