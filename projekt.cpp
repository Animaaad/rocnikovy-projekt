#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
int k;
int d;
int g;
/*start is the vertex that is the start of the "outer layer",
which is all the leaves of the k trees that
are required to have minimal distance d
depth is the height of each individual tree
outer_layer is the number of vertices that are on the "outer_layer"
end is the last vertex in the graph
odd_mode indicates if k is odd
odd_start is the first vertex of the layer that is right before the outer layer
*/ 
int depth;
int start;                
int outer_layer;
int end;
int n;
bool odd_mode;
int odd_start;
std::vector<std::vector<int>> graph;

bool contains(const std::vector<int> vec, int value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}
/*
function that initializes the graph.
initializes it layer by layer of k trees
the first layer will have assigned the smallest numbers, 
the outer level will have assigned the biggest numbers.
if k is even, then the trees will have d/2 layers, and
we will only need to test edge connections between vertices on the outer layer
if k is odd, then the trees will have also d/2 layers,
however the outer layer won't be connected to anything initially and
whether vertices connect to anything will have to be tested 
in the search function
*/
std::vector<std::vector<int>> initialize_graph() {
    std::vector<std::vector<int>> graf;
    for (int layer = 0; layer <= depth; layer++) {
        int layer_total = std::pow(2, layer) * k;
        int layers_total = (std::pow(2, (layer + 1)) - 1) * k ;
        for (int i = 0; i < layer_total; i++) {
            std::vector<int> vec;
            if (layer == depth) { //special case, this layer has no children
                if (!odd_mode) {
                    int parent = odd_start + i / 2;
                    vec.push_back(parent);
                }
            } else {
                if (!odd_mode || layer != depth - 1) {
                    int child1 = layers_total + (i) * 2;
                    int child2 = child1 + 1;
                    vec.push_back(child1);
                    vec.push_back(child2);
                }
                if (layer != 0) { //special case, this layer has no parents
                /*this calculates the start of the previous level,
                 and then adds i/2 to get from the start of the previous layer
                 to the correct vertex*/
                   int parent = std::pow(2, (layer)- 1) * k -
                    (std::pow(2, (layer - 1)) * k - i / 2);
                    
                    vec.push_back(parent);
                }
            }
            graf.push_back(vec);
        }
    }
    return graf;
}
/*
standard way of finding the minimal cycle in a graph
*/
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

/*
main function of the code
the minimal possible graph with those parameteres has to have at least
k trees of height d/2 because the distance between the vertices that have 2
edges indecent with them has to be at least d, and every vertex on the path
between them has to be incedent with at least 3 edges
@return whether the minimal possible graph for the parameters k, d, g exists
if k is even, tests all the possible subsets of vertices of the complete graph
on vertices {start, ..., end}, that are viable,
i.e. no vertex has more than 3 edges incident with it, and 
no cycle is shorter than g
if k is odd, tests all the possible subsets of vertices of the complete graph
on vertices {odd_start, ..., end}, that are viable,
i.e. no vertex has more than 3 edges incident with it, and 
no cycle is shorter than g
if the graph exists, it will find it, but the graph might have some vertices
that aren't incedent with any edges, we can just disregard them*/
bool search(int i, int j) {
    if (i == j && j != end - 1) return search(i, j + 1);
    if (i < start && j < start) {
        if (j == end - 1) {
            if (search(i + 1, odd_start)) return true;
        } else {
            if (search(i, j + 1)) return true;
        }
    }
    if (i == j && j == end - 1) {
        if (odd_mode) {
            for (int i = start; i < end; i++) {
                if (graph[i].size() != 3 && graph[i].size() != 0) return false;
            }
            for (int i = odd_start; i < start; i++) {
                if (graph[i].size() != 2) return false; //3 if k > 5
            }
        } else {
            for (int i = start; i < end; i++) {
                if (graph[i].size() != 3) return false;
            }
        }
       if (check_cycle() >= g) return true;
    }
    if (!contains(graph[i], j) && graph[i].size() < 3 && graph[j].size() < 3 &&
    /*if k == 5, then the layer that comes before the outer layer is the 
    first layer, and vertices on it can't be incident with more than 2 edges*/
    !(((i < start && graph[i].size() == 2) ||
    (j < start && graph[j].size() == 2)) && (k == 5))) {
        graph[i].push_back(j);
        graph[j].push_back(i);
        //the cycle can appear only using the vertex i, so we can use bfs
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
    std::cin >> k;
    g = k;
    d = k - 3;
    depth = d / 2;
    start = k * (std::pow(2, depth) - 1);   
    outer_layer = (std::pow(2, (depth))) * k;
    end = start + outer_layer;
    n = end;
    odd_start = start - outer_layer / 2;
    if (k % 2 == 1) {
        odd_mode = true; 
    }
    graph = initialize_graph();
    int a;
    if (odd_mode) {
        a = search(odd_start, odd_start);
    } else {
        a = search(start, start);
    }
    if (a == 0) std::cout << "false" << std::endl;
    /*will print the graph that is compatible with our criteria
    if such graph exists*/
    else {
        std::cout << "true" << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < graph[i].size(); j++) {
                std::cout << i << ' ' << graph[i][j] << std::endl;
            }
        }
    }
}
