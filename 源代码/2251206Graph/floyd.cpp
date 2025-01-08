#include "floyd.h"

Floyd::Floyd(Graph g)
{
    this->adjList = g.getAdjList();
    this->weights = g.getWeights();
}

/* 计算每对顶点之间的最短路径 */
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>> Floyd::floydWarshall()
{
    int V = adjList.size();  // 顶点数

    std::map<int, int> vertexMap;
    std::vector<int> reverseMap(V + 1); // 反向映射以恢复原始顶点编号
    int index = 1;

    // 构建映射
    for (const auto& pair : adjList) {
        vertexMap[pair.first] = index;
        reverseMap[index] = pair.first;
        index++;
    }

    // 初始化距离矩阵，所有顶点对之间的距离初始为无穷大（表示不可达）
    std::vector<std::vector<int>> dist(V + 1, std::vector<int>(V + 1, std::numeric_limits<int>::max()));
    std::vector<std::vector<int>> next(V + 1, std::vector<int>(V + 1, -1));  // 路径矩阵

    for (const auto& pair : adjList) {
        int u = vertexMap[pair.first];
        dist[u][u] = 0;  // 自己到自己距离为0
        for (int v : pair.second) {
            dist[u][vertexMap[v]] = weights[{pair.first, v}];  // 邻接顶点的距离
            next[u][vertexMap[v]] = vertexMap[v];  // 直接相连的路径
        }
    }

    // Floyd-Warshall
    for (int k = 1; k <= V; ++k) // k 为中间顶点
        for (int i = 1; i <= V; ++i) // i 为起始顶点
            for (int j = 1; j <= V; ++j) // j 为终止顶点
                // 如果通过顶点 k 可以缩短从 i 到 j 的距离，则更新 dist[i][j]
                if (dist[i][k] != std::numeric_limits<int>::max() &&
                    dist[k][j] != std::numeric_limits<int>::max() &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k]; // 更新路径矩阵
                }

    // 构建所有最短路径
    std::vector<std::vector<std::vector<int>>> paths(V + 1, std::vector<std::vector<int>>(V + 1));

    for (int i = 1; i <= V; ++i) {
        for (int j = 1; j <= V; ++j) {
            if (dist[i][j] != std::numeric_limits<int>::max()) {
                int u = i;
                while (u != j) {
                    paths[i][j].push_back(reverseMap[u]);
                    u = next[u][j];
                }
                paths[i][j].push_back(reverseMap[j]); // 加上目标顶点
            }
        }
    }

    return {dist, paths};
}

