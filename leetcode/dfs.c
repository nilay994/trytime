#include <stdio.h>
#include <stdbool.h>

void dfs(bool visited[], int graph[][7], int node) {
    printf("%d ", node);  // print the visited node
    visited[node] = true; // mark node as visited

    for (int i = 0; i < 7; i++) {
        // check adjacent nodes
        if (graph[node][i] == 1 && !visited[i]) {
            dfs(visited, graph, i); // recursively visit unvisited adjacent node
        }
    }
}

int main() {
    bool visited[7] = {false}; // initialize visited array
    int graph[7][7] = {
        {0, 1, 1, 0, 0, 0, 0}, // adjacency matrix representing the graph
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    printf("DFS traversal: ");
    dfs(visited, graph, 0); // start DFS from node 0

    return 0;
}
