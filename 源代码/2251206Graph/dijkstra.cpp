#include "dijkstra.h"
#include <queue>

/* 构造函数复制父类基本信息 */
Dijkstra::Dijkstra(Graph g)
{
    this->adjList = g.getAdjList();
    this->weights = g.getWeights();
}

std::pair<std::map<int, int>, std::map<int, std::vector<int>>> Dijkstra::dijkstra(int start) {
    // 存储最短路径的距离
    std::map<int, int> dist;
    // 存储最短路径的前驱顶点
    std::map<int, int> prev;

    // 初始化所有顶点的距离为无穷大，前驱顶点设为 -1
    for (auto& pair : adjList) {
        dist[pair.first] = std::numeric_limits<int>::max();
        prev[pair.first] = -1;
    }
    dist[start] = 0; // 起点的距离为0

    // 优先队列存储 <距离, 顶点>
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        // 遍历邻接点
        for (int v : adjList[u]) {
            int weight = weights[{u, v}];
            // 如果通过 u 到达 v 的路径比当前已知的最短路径更短，则更新最短路径
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
                prev[v] = u; // 更新前驱顶点
            }
        }
    }

    // 构建每个顶点的最短路径
    std::map<int, std::vector<int>> paths;
    for (const auto& pair : dist) {
        int v = pair.first;
        std::vector<int> path;
        // 从目标顶点回溯到起点
        for (int at = v; at != -1; at = prev[at])
            path.push_back(at);
        std::reverse(path.begin(), path.end()); // 反转路径
        paths[v] = path; // 存储路径
    }

    return {dist, paths};
}

