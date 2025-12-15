#pragma once

#include <vector>

using graph = std::vector<std::vector<int>>;

std::vector<int> seq_bfs(const graph& g, const int root = 0);

std::vector<int> par_bfs(const graph& g, const int root = 0);