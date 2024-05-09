#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Function to perform BFS on the graph
void bfs(int start, const vector<vector<int>>& graph) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;

    // Start the BFS from the given starting vertex
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int vertex = q.front();
        cout << vertex << " ";
        q.pop();

        // Explore all the adjacent vertices
        for (int i : graph[vertex]) {
            if (!visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
}

int main() {
    // Create a sample graph using an adjacency list
    vector<vector<int>> graph = {
        {},          // 0 (no edges)
        {2, 3},     // 1
        {1, 4},     // 2
        {1, 5},     // 3
        {2},        // 4
        {3}         // 5
    };

    cout << "BFS starting from vertex 1: ";
    bfs(1, graph);

    return 0;
}
