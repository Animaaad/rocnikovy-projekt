#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#define Graph std::vector<std::vector<bool>>;
int k = 6;
int d = 3;
int g = 6;
int n = 18;
int k2 = k * 2; 
std::vector<std::vector<int>> graph;

bool contains(const std::vector<int> vec, int value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

std::vector<std::vector<int>> initiate_graph() {
    std::vector<std::vector<int>> graf;
    for (int i = 0; i < k2; i++) {
        std::vector<int> vec;
        vec.push_back(i / 2 + 12);
        graf.push_back(vec);
    }
    for (int i = k2; i < n; i++) {
        std::vector<int> vec;
        vec.push_back((i - 12) * 2);
        vec.push_back((i - 12) * 2 + 1);
        graf.push_back(vec);
    }
    return graf;
}

int bfs(int s) {
    std::queue<int> q;
    std::vector<int> parents(n ,-1);
    std::vector<bool> visited(n, false);
    std::vector<int> distance(n, 0);
    q.push(s);
    visited[s] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : graph[u]) {
            if (parents[u] != v && visited[v]) {
                return distance[u] + distance [v] + 1;
            } else if (!visited[v]) {
                q.push(v);
                parents[v] = u;
                distance[v] = distance [u] + 1;
                visited[v] = true;
            }
        }
    }
    return INT_MAX;
}

int check_cycle() {
    int min = INT_MAX;
    int tmp;
    for (int i = 0; i < n; i++) {
        tmp = bfs(i);
        if (tmp < min) {
            min = tmp;
        }
    }
    return min;
}

bool recursion(int i, int j) {
    if (graph[i].size() > 3 || graph[j].size() > 3) return false;
    if (i == j && j != k2 - 1) return recursion(i, j + 1);
    if (i == k2 && j == 0) {
       for (int i = 0; i < k2; i++) {
            if (graph[i].size() != 3) return false;
       } if (check_cycle() >= g) return true;
    }
    int min_cycle = check_cycle();
    if (min_cycle >= g) {
        if (!contains(graph[i], j)) {
            graph[i].push_back(j);
            graph[j].push_back(i);
            if (j == k2 - 1) {
                if (recursion(i + 1, 0)) return true;
            } else {
                if (recursion(i, j + 1)) return true;
            }
            auto it = std::find(graph[i].begin(), graph[i].end(), j);
            graph[i].erase(it);
            it = std::find(graph[j].begin(), graph[j].end(), i);
            graph[j].erase(it);
        }
    
        if (j == k2 - 1) {
            if (recursion(i + 1, 0)) return true;
        } else {
            if (recursion(i, j + 1)) return true;
        }
    }
    return false;
}

int main() {
    graph = initiate_graph();
    int a = recursion(0, 0);
    if (a == 0) std::cout << "false" << std::endl;
    else std::cout << "true" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            std::cout << i << ' ' << graph[i][j] << std::endl;
        }
    }
}
