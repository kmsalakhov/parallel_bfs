#include <random>

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

TEST(SequenceBfs, SingleNodeGraph) {
    graph g(1);
    const std::vector<int> result = seq_bfs(g, 0);
    EXPECT_EQ(result, std::vector<int>{0});
}

TEST(SequenceBfs, DisconnectedGraph) {
    graph g(4);
    g[0] = {1};
    g[1] = {0};
    g[2] = {3};
    g[3] = {2};
    
    const std::vector<int> expected = {0, 1, -1, -1};
    const std::vector<int> result = seq_bfs(g, 0);
    EXPECT_EQ(result, expected);
}

TEST(SequenceBfs, MultipleShortestPaths) {
    graph g(5);
    g[0] = {1, 2};
    g[1] = {0, 3, 4};
    g[2] = {0, 3};
    g[3] = {1, 2, 4};
    g[4] = {1, 3};
    
    const std::vector<int> expected = {0, 1, 1, 2, 2};
    const std::vector<int> result = seq_bfs(g, 0);
    EXPECT_EQ(result, expected);
}

TEST(SequenceBfs, SemiRandomGraph) {
    /*
       Структура графа:
          0
         / \
        1   2
       / \   \
      3   4   5
           \ /
            6
           / \
          7   8
    */
    graph g(9);
    g[0] = {1, 2};
    g[1] = {0, 3, 4};
    g[2] = {0, 5};
    g[3] = {1};
    g[4] = {1, 6};
    g[5] = {2, 6};
    g[6] = {4, 5, 7, 8};
    g[7] = {6};
    g[8] = {6};
    
    const std::vector<int> expected = {0, 1, 1, 2, 2, 2, 3, 4, 4};
    const std::vector<int> result = seq_bfs(g, 0);
    EXPECT_EQ(result, expected);
}

TEST(SequenceBfs, LargeStarGraph) {
    const int n = 10000;
    graph g(n);
    for (int i = 1; i < n; ++i) {
        g[0].push_back(i);
        g[i].push_back(0);
    }
    
    std::vector<int> expected(n, 1);
    expected[0] = 0;
    
    const std::vector<int> result = seq_bfs(g, 0);
    EXPECT_EQ(result, expected);
}



// ParallelTests
TEST(ParallelBfs, EmptyGraph) {
    graph g(0);

    const std::vector<int> result = par_bfs(g, 0);

    EXPECT_EQ(result.empty(), true);
}

TEST(ParallelBfs, StarGraph) {
    graph g(5);
    g[0] = {1, 2, 3, 4};
    std::vector<int> expected(5, 1);
    expected[0] = 0;

    const std::vector<int> result = par_bfs(g, 0);

    EXPECT_EQ(result, expected);
}

TEST(ParallelBfs, PathGraph) {
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

    const std::vector<int> result = par_bfs(g, 0);

    EXPECT_EQ(result, expected);
}

TEST(ParallelBfs, CycleGraph) {
    const int n = 5;
    graph g(n);
    for (int i = 0; i < n; ++i) {
        g[i].push_back((i + 1) % n);
        g[(i + 1) % n].push_back(i);
    }
    const std::vector<int> expected = {0, 1, 2, 2, 1};

    const std::vector<int> result = par_bfs(g, 0);

    EXPECT_EQ(result, expected);
}

TEST(ParallelBfs, SingleNodeGraph) {
    graph g(1);
    const std::vector<int> result = par_bfs(g, 0);
    EXPECT_EQ(result, std::vector<int>{0});
}

TEST(ParallelBfs, DisconnectedGraph) {
    graph g(4);
    g[0] = {1};
    g[1] = {0};
    g[2] = {3};
    g[3] = {2};
    
    const std::vector<int> expected = {0, 1, -1, -1};
    const std::vector<int> result = par_bfs(g, 0);
    EXPECT_EQ(result, expected);
}

TEST(ParallelBfs, MultipleShortestPaths) {
    graph g(5);
    g[0] = {1, 2};
    g[1] = {0, 3, 4};
    g[2] = {0, 3};
    g[3] = {1, 2, 4};
    g[4] = {1, 3};
    
    const std::vector<int> expected = {0, 1, 1, 2, 2};
    const std::vector<int> result = par_bfs(g, 0);
    EXPECT_EQ(result, expected);
}

TEST(ParallelBfs, SemiRandomGraph) {
    /*
       Структура графа:
          0
         / \
        1   2
       / \   \
      3   4   5
           \ /
            6
           / \
          7   8
    */
    graph g(9);
    g[0] = {1, 2};
    g[1] = {0, 3, 4};
    g[2] = {0, 5};
    g[3] = {1};
    g[4] = {1, 6};
    g[5] = {2, 6};
    g[6] = {4, 5, 7, 8};
    g[7] = {6};
    g[8] = {6};
    
    const std::vector<int> expected = {0, 1, 1, 2, 2, 2, 3, 4, 4};
    const std::vector<int> result = par_bfs(g, 0);
    EXPECT_EQ(result, expected);
}

TEST(ParallelBfs, LargeStarGraph) {
    const int n = 10000;
    graph g(n);
    for (int i = 1; i < n; ++i) {
        g[0].push_back(i);
        g[i].push_back(0);
    }
    
    std::vector<int> expected(n, 1);
    expected[0] = 0;
    
    const std::vector<int> result = par_bfs(g, 0);
    EXPECT_EQ(result, expected);
}



// EQUALITY TESTS
graph generate_random_graph(int n, double edge_probability, unsigned seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    graph g(n);
    
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (dist(rng) < edge_probability) {
                g[i].push_back(j);
                g[j].push_back(i);
            }
        }
    }
    return g;
}

TEST(RandomGraphConsistency, DenseGraph) {
    const int n = 200;
    const double density = 0.7;
    const int root = 0;
    
    graph g = generate_random_graph(n, density);
    
    auto seq_result = seq_bfs(g, root);
    auto par_result = par_bfs(g, root);
    
    ASSERT_EQ(seq_result, par_result);
}

TEST(RandomGraphConsistency, SparseGraph) {
    const int n = 300;
    const double density = 0.05;
    const int root = n / 2;
    
    graph g = generate_random_graph(n, density);
    
    auto seq_result = seq_bfs(g, root);
    auto par_result = par_bfs(g, root);
    
    ASSERT_EQ(seq_result, par_result);
}

TEST(RandomGraphConsistency, MediumGraph) {
    const int n = 150;
    const double density = 0.3;
    const int root = 42;
    
    graph g = generate_random_graph(n, density, 12345);
    
    auto seq_result = seq_bfs(g, root);
    auto par_result = par_bfs(g, root);
    
    ASSERT_EQ(seq_result, par_result);
}