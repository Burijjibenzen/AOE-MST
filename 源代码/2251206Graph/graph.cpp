#include "Graph.h"
#include <list>
#include <QDebug>

/* 添加边 */
void Graph::addEdge(int u, int v, int w)
{
    // qDebug() << "Adding edge: " << u << " -> " << v;
    adjList[u].push_back(v); // 加入邻接链表
    adjList[v].push_back(u); // 如果是无向图

    std::pair<int, int> edge1(u, v);
    std::pair<int, int> edge2(v, u);
    weights[edge1] = w;
    weights[edge2] = w;
    // qDebug() << weights[edge1];
}

/* 删除边 */
void Graph::removeEdge(int u, int v)
{
    adjList[u].remove(v); // 从邻接链表删除
    adjList[v].remove(u); // 如果是无向图

    std::pair<int, int> edge1(u, v);
    std::pair<int, int> edge2(v, u);

    std::map<std::pair<int, int>, int>::iterator pos;

    pos = weights.find(edge1);
    if (pos != weights.end())
        weights.erase(pos);

    pos = weights.find(edge2);
    if (pos != weights.end())
        weights.erase(pos);
}

/* 添加顶点 */
void Graph::addVertex(int vt)
{
    std::list<int> lst = {};
    adjList.insert(std::map<int, std::list<int>>::value_type(vt, lst));
}

/* 删除顶点 */
void Graph::removeVertex(int vt)
{
    for (int neighbor : adjList[vt]) {
        // 从邻居的邻接表中删除该顶点
        adjList[neighbor].remove(vt);

        // 删除与该顶点相关的边权重
        std::pair<int, int> edge1(vt, neighbor);
        std::pair<int, int> edge2(neighbor, vt);

        if (weights.find(edge1) != weights.end())
            weights.erase(edge1);
        if (weights.find(edge2) != weights.end())
            weights.erase(edge2);
    }

    // 删除该顶点的邻接表
    adjList.erase(vt);
}

/* 获取邻接链表 */
std::map<int, std::list<int>> Graph::getAdjList()
{
    // 进行升序排序
    for (auto &pair : adjList)
        pair.second.sort();
    return adjList;
}

/* 深度优先搜索图 */
void Graph::DFS(int v, std::map<int, bool>& visited)
{
    visited[v] = true;

    // 遍历与 v 相邻的所有顶点
    for (int neighbor : adjList.at(v))
        if (!visited[neighbor])
            DFS(neighbor, visited);
}

/* 判断是否是连通图 */
bool Graph::isConnected()
{
    std::map<int, bool> visited;

    // 初始化所有顶点为未访问
    for (const auto& pair : adjList)
        visited[pair.first] = false;

    // 从任意一个顶点开始 DFS
    int startVertex = adjList.begin()->first;
    DFS(startVertex, visited);

    // 检查是否所有顶点都被访问过
    for (const auto& pair : visited)
        if (!pair.second)
            return false;  // 有未访问的顶点，图不连通

    return true;  // 所有顶点都被访问过，图是连通的
}

/* 获取权重表 */
std::map<std::pair<int, int>, int> Graph::getWeights()
{
    return weights;
}
