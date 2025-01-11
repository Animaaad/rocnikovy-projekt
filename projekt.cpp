#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
int k = 8;
int d = 5;
int g = 8;
int depth = d / 2;
int start = k * (std::pow(2, depth) - 1);
int outer_layer = (std::pow(2, (depth))) * k;
int end = start + outer_layer;
int n = end;
std::vector<std::vector<int>> graph;

bool contains(const std::vector<int> vec, int value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

std::vector<std::vector<int>> initiate_graph() {
    std::vector<std::vector<int>> graf;
    int current;
    for (int layer = 0; layer <= depth; layer++) {
        int layer_total = std::pow(2, layer) * k;
        int layers_total = (std::pow(2, (layer + 1)) - 1) * k ;
        for (int i = 0; i < layer_total; i++) {
            std::vector<int> vec;
            current++;
            if (layer == 0) {
                int child1 = layer_total + (i) * 2;
                int child2 = child1 + 1;
                vec.push_back(child1);
                vec.push_back(child2);
            } else if (layer == depth) {
                int parent = start - 
                (outer_layer / 2 - i / 2);                                
                vec.push_back(parent);                  
            } else {
                int child1 = layers_total + (i) * 2;
                int child2 = child1 + 1;
                vec.push_back(child1);
                vec.push_back(child2);
                int parent = std::pow(2, (layer)- 1) * k - 
                (std::pow(2, (layer - 1)) * k - i / 2);
                vec.push_back(parent);
            }
            graf.push_back(vec);
        }
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

bool search(int i, int j) {
    if (i == j && j != end - 1) return search(i, j + 1);
    if (i == j && j == end - 1) {
       for (int i = start; i < end; i++) {
            if (graph[i].size() != 3) return false;
       }
       if (check_cycle() >= g) return true;
    }
    if (!contains(graph[i], j) && graph[i].size() < 3 && graph[j].size() < 3) {
        graph[i].push_back(j);
        graph[j].push_back(i);
        if (bfs(i) >= g) {
           if (j == end - 1) {
           if (search(i + 1, start)) return true;
           } else {
                if (search(i, j + 1)) return true;
            }
        }
        auto it = std::find(graph[i].begin(), graph[i].end(), j);
        graph[i].erase(it);
        it = std::find(graph[j].begin(), graph[j].end(), i);
        graph[j].erase(it);
    }
    if (j == end - 1) {
        if (search(i + 1, start)) return true;
    } else {
        if (search(i, j + 1)) return true;
    }
    return false;
}

int main() {
    graph = initiate_graph();
    int a = search(start, start);
    if (a == 0) std::cout << "false" << std::endl;
    else {
        std::cout << "true" << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < graph[i].size(); j++) {
                std::cout << i << ' ' << graph[i][j] << std::endl;
            }
        }
    }
}
