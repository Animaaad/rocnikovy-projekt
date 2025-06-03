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
*/
int depth;
int start;
int outer_layer;
int end;
int n;
bool odd_mode;
int odd_end;
int extra;
int extra_start;
int tree_leaves;
std::vector<std::vector<int>> graph;
std::vector<std::vector<int>> time_saving_vec;

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
    for (int layer = 0; layer <= depth; layer++)
    {
        int layer_total = std::pow(2, layer) * k;
        int layers_total = (std::pow(2, (layer + 1)) - 1) * k;
        for (int i = 0; i < layer_total; i++)
        {
            std::vector<int> vec;
            if (layer != depth)
            { // special case, this layer has no children
                int child1 = layers_total + (i) * 2;
                int child2 = child1 + 1;
                vec.push_back(child1);
                vec.push_back(child2);
            }
            if (layer != 0)
            { // special case, this layer has no parents
                /*this calculates the start of the previous level,
                 and then adds i/2 to get from the start of the previous layer
                 to the correct vertex*/
                int parent = (std::pow(2, layer) - 1) * k -
                             (std::pow(2, (layer - 1)) * k - i / 2);
                vec.push_back(parent);
            }
            graf.push_back(vec);
        }
    }
    if (odd_mode)
    {
        int res = outer_layer * 2 % 3;
        switch (res)
        {
        case 1:
        {
            std::vector<int> vec;
            vec.push_back(start + outer_layer + 1);
            graf.push_back(vec);
            std::vector<int> vec2;
            vec2.push_back(start + outer_layer);
            graf.push_back(vec2);
            break;
        }
        case 2:
        {
            std::vector<int> vec;
            vec.push_back(start + outer_layer + 1);
            graf.push_back(vec);
            std::vector<int> vec2;
            vec2.push_back(start + outer_layer);
            graf.push_back(vec2);
            std::vector<int> vec3;
            vec3.push_back(start + outer_layer + 3);
            graf.push_back(vec3);
            std::vector<int> vec4;
            vec4.push_back(start + outer_layer + 2);
            graf.push_back(vec4);
            break;
        }
        }
        for (int i = 0; i < extra - res * 2; i++)
        {
            std::vector<int> vec;
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
    std::vector<int> parents(n, -1);
    std::vector<bool> visited(n, false);
    std::vector<int> distance(n, 0);
    q.push(s);
    visited[s] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            // if (true) {std::cerr<<u<<" b "<<v<<" distance "<<distance[u] + 1<<std::endl;}
            if (parents[u] != v && visited[v]) {
                return distance[u] + distance[v] + 1;
            }
            else if (!visited[v]) {
                q.push(v);
                parents[v] = u;
                distance[v] = distance[u] + 1;
                visited[v] = true;
                if (distance[v] >= g)
                    return 100;
            }
        }
    }
    return 100;
}

int check_cycle() {
    int min = 100;
    int tmp;
    for (int i = 0; i < n; i++) {
        tmp = bfs(i);
        if (tmp < min) {
            min = tmp;
        }
    }
    return min;
}

void prep() {
    for (int i = 0; i < k; i++) {
        std::vector<int> vec;
        for (int j = start; j < tree_leaves + start; j++) {
            vec.push_back(i * tree_leaves + j);
        }
        time_saving_vec.push_back(vec);
    }
}

int tree_index(int value) {
    for (int i = 0; true; i++) {
        if (contains(time_saving_vec[i], value)) {
            return i;
        }
    }
}

/*
main function of the code
the
@return whether the minimal possible graph for the parameters k, d, g exists
if k is even, tests all the possible subsets of vertices of the complete graph
on vertices {start, ..., end}, that are viable,
i.e. no vertex has more than 3 edges incident with it, and
no cycle is shorter than g
if k is odd, tests all the possible subsets of vertices of the complete graph
on vertices {odd_start, ..., end}, that are viable,
i.e. no vertex has more than 3 edges incident with it, and
no cycle is shorter than g*/
bool search(int i, int j) {
    if (i == j && j != end - 1)
        return search(i, j + 1);
    // std::cout<<i<<" j "<<j<<" "<<tree_index(i)<<" tj "<<tree_index(j)<<std::endl;
    if (!odd_mode && abs(tree_index(i) - tree_index(j)) > 1 &&
        !(tree_index(i) == 0 && tree_index(j) == k - 1)) {
        if (j == end - 1) {
            if (search(i + 1, start))
                return true;
        }
        else {
            if (search(i, j + 1))
                return true;
        }
        return false;
    }
    if (i == j) {
        if (odd_mode) {
            for (int i = k; i < end; i++) { // we can start from the variable start if k > 5
                if (graph[i].size() != 3)
                    return false;
            }
            for (int i = 0; i < k; i++) {
                if (graph[i].size() != 2)
                    return false; // only needed for k = 5
            }
        }
        else {
            for (int i = start; i < end; i++) {
                if (graph[i].size() != 3)
                    return false;
            }
        }
        if (check_cycle() >= g)
            return true;
        return false;
    }
    //std::cout << i << " j " << j << std::endl;
    if ((j < i && !(j - i > outer_layer - (outer_layer / k * 2))) || 
    (odd_mode && ((i < start + outer_layer && j < start + outer_layer) ||
    (i >= start + outer_layer && j >= start + outer_layer)))) {
        if (j == end - 1) {
            if (search(i + 1, start)) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            if (search(i, j + 1)) {
                return true;
            }
            else {
                return false;
            }
        }
        return false;
    }
    if (!contains(graph[i], j) && graph[i].size() < 3 && graph[j].size() < 3) {
        // std::cout<<i<<" "<<j<<std::endl;
        graph[i].push_back(j);
        graph[j].push_back(i);
        // the cycle can appear only using the vertex i, so we can use bfs
        if (bfs(i) >= g) {
            if (j == end - 1) {
                if (search(i + 1, start))
                    return true;
            }
            else {
                if (search(i, j + 1))
                    return true;
            }
        }
        auto it = std::find(graph[i].begin(), graph[i].end(), j);
        graph[i].erase(it);
        it = std::find(graph[j].begin(), graph[j].end(), i);
        graph[j].erase(it);
    }
    if (j == end - 1) {
        if (search(i + 1, start))
            return true;
    }
    else {
        if (search(i, j + 1))
            return true;
    }
    return false;
}

void edge_case_function() {
    for (int i = start; i < start + tree_leaves; i++) {
        for (int j = start + tree_leaves; j < start + 2 * tree_leaves; j++) {
            if (contains(graph[i], j)) {
                auto it = std::find(graph[i].begin(), graph[i].end(), j);
                graph[i].erase(it);
                it = std::find(graph[j].begin(), graph[j].end(), i);
                graph[j].erase(it);
            }
        }
    }
}

bool edge_case_function2(int i, int j, int istart, int jstart) {
    if (j == jstart + tree_leaves) {
        //std::cout<<"god damn... "<< i << " "<< j <<std::endl;
        return false;
    }
    graph[i].push_back(j);
    graph[j].push_back(i);
    // the cycle can appear only using the vertex i, so we can use bfs
    if (bfs(i) >= g) {
        if (i == istart + tree_leaves - 1) return true;
        if (edge_case_function2(i + 1, jstart, istart, jstart)) return true;
    }
    auto it = std::find(graph[i].begin(), graph[i].end(), j);
    graph[i].erase(it);
    it = std::find(graph[j].begin(), graph[j].end(), i);
    graph[j].erase(it);
    if (edge_case_function2(i, j + 1, istart, jstart)) return true;
    return false;
}

bool search2() {
    int i = start, j = start;
    while (!(i == end && j == start)) {
        while (j != end && graph[i].size() < 3) {
            if ((abs(tree_index(i) - tree_index(j)) <= 1) && i <= j &&
                !contains(graph[i], j)  && graph[j].size() < 3) {
                graph[i].push_back(j);
                graph[j].push_back(i);
                // the cycle can appear only using the vertex i, so we can use bfs
                if (bfs(i) < g) {
                    auto it = std::find(graph[i].begin(), graph[i].end(), j);
                    graph[i].erase(it);
                    it = std::find(graph[j].begin(), graph[j].end(), i);
                    graph[j].erase(it);
                } 
            }
            j++;
        }
        i++;
        j = start;
    }
    //edge_case_function();
    edge_case_function2(start, end - tree_leaves, start, end - tree_leaves);
    //edge_case_function2(start + tree_leaves, start, start + tree_leaves, start);
    return true;
}

int main() {
    std::cin >> k >> g >> d;
    depth = d / 2;
    start = k * (std::pow(2, depth) - 1);
    outer_layer = (std::pow(2, (depth))) * k;
    end = start + outer_layer;
    n = end;
    tree_leaves = outer_layer / k;
    prep();
    if (k % 2 == 1) {
        depth--;
        odd_mode = true;
        outer_layer = (std::pow(2, (depth))) * k;
        start = k * (std::pow(2, depth) - 1);
        extra_start = start + outer_layer;
        extra = (outer_layer * 2 + 3) / 3;
        end = start + outer_layer + extra;
        n = end;
    }
    graph = initialize_graph();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            // std::cout << i << ' ' << graph[i][j] << " g" << std::endl;
        }
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < time_saving_vec[i].size(); j++) {
            // std::cout << i << ' ' << time_saving_vec[i][j] << " t" << std::endl;
        }
    }
    int a = 0;
    if (odd_mode) {
        a = search(start, start);
    }
    else {
        a = search2();
    }

    if (a == 0) std::cout << "false" << std::endl;
    /*will print the graph that is compatible with our criteria
    if such graph exists*/
    else {  
        std::cout << "true" << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < graph[i].size(); j++) {
                //std::cout << i << ' ' << graph[i][j] << std::endl;
                if ((!odd_mode && graph[i].size() != 3 && i > start)) {
                    std::cout <<"wrong edges..." << i << " " << tree_index(i) << " " << j << std::endl;
                }
            }
        }
        if (check_cycle() < g) {
            std::cout << "wrong cycle..." << start << " " << end;
        }
    }
}