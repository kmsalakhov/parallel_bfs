#include <gtest/gtest.h>

#include "bfs.h"

TEST(SequenceBfs, EmptyGraph) {
    graph g(0);

    const std::vector<int> result = seq_bfs(g, 0);

    EXPECT_EQ(result.empty(), true);
}

TEST(SequenceBfs, StarGraph) {
    graph g(5);
    g[0] = {1, 2, 3, 4};
    std::vector<int> expected(5, 1);
    expected[0] = 0;

    const std::vector<int> result = seq_bfs(g, 0);

    EXPECT_EQ(result, expected);
}

TEST(SequenceBfs, PathGraph) {
    const int n = 100;
    graph g(n);
    std::vector<int> expected(n);

    for (int i = 0; i < n; ++i) {
        if (i != n - 1) {
            g[i].push_back(i + 1);
            g[i + 1].push_back(i);
        }
        expected[i] = i;
    }

    const std::vector<int> result = seq_bfs(g, 0);

    EXPECT_EQ(result, expected);
}

TEST(SequenceBfs, CycleGraph) {
    const int n = 5;
    graph g(n);
    for (int i = 0; i < n; ++i) {
        g[i].push_back((i + 1) % n);
        g[(i + 1) % n].push_back(i);
    }
    const std::vector<int> expected = {0, 1, 2, 2, 1};

    const std::vector<int> result = seq_bfs(g, 0);

    EXPECT_EQ(result, expected);
}