#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>

/* 边 */
struct Edge {
    int u, v, weight;

    bool operator<(const Edge& other) const { // 重载运算符，边权按照升序排列
        return weight < other.weight;
    }

    bool operator>(const Edge& other) const { // 重载运算符，边权按照升序排列
        return weight > other.weight;
    }

    // 重载运算符，用于比较两个边是否相等
    bool operator==(const Edge& other) const {
        return (u == other.u && v == other.v && weight == other.weight) ||
               (u == other.v && v == other.u && weight == other.weight);
    }
};

/* 无向图 */
class Graph {
public:
    void addEdge(int u, int v, int w); // 新增边
    void removeEdge(int u, int v);     // 删除边
    void addVertex(int vt);    // 添加顶点
    void removeVertex(int vt); // 删除顶点
    void DFS(int v, std::map<int, bool>& visited); // 深度优先搜索图

    bool isConnected(); // 判断是否是连通图

    std::map<int, std::list<int>> getAdjList(); // 获取当前邻接链表
    std::map<std::pair<int, int>, int> getWeights(); // 获取当前权重

protected:
    std::map<int, std::list<int>> adjList;      // 邻接链表
    std::map<std::pair<int, int>, int> weights; // 权重表
};

#endif // GRAPH_H
