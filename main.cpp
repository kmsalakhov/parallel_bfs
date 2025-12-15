#include <iostream>
#include <vector>

#include "bfs.h"

int main() {
    graph g(5);
    g[0] = {1, 2, 3, 4};
    const std::vector<int> result = seq_bfs(g);
    for (int x : result) {
        std::cout << x << std::endl;
    }

    return 0;
}