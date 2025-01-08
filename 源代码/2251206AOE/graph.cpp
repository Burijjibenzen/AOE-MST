#include "graph.h"
#include <queue>
#include <QDebug>

/* 添加边 */
void Graph::addEdge(int u, int v, int w)
{
    // qDebug() << "Adding edge: " << u << " -> " << v;
    adjList[u].push_back(v); // 加入邻接链表

    std::pair<int, int> edge(u, v);
    weights[edge] = w;
}

/* 删除边 */
void Graph::removeEdge(int u, int v)
{
    adjList[u].remove(v); // 从邻接链表删除

    std::pair<int, int> edge(u, v);

    std::map<std::pair<int, int>, int>::iterator pos;

    pos = weights.find(edge);
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
    // 删除顶点 vt 所有的出边
    for (int neighbor : adjList[vt]) {
        std::pair<int, int> edge(vt, neighbor);
        if (weights.find(edge) != weights.end())
            weights.erase(edge);
    }

    // 删除所有指向顶点 vt 的入边
    for (auto& pair : adjList) {
        int u = pair.first;
        std::list<int>& neighbors = pair.second;

        neighbors.remove(vt);
        std::pair<int, int> edge(u, vt);
        if (weights.find(edge) != weights.end())
            weights.erase(edge);
    }

    // 从邻接表中删除顶点 vt
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

/* 获取权重表 */
std::map<std::pair<int, int>, int> Graph::getWeights()
{
    return weights;
}

/* 拓扑排序 */
bool Graph::topologicalSort(std::vector<int>& result)
{
    std::map<int, int> inDegree; // 记录入度
    std::queue<int> zeroInDegreeQueue; // 存储入度为 0 的节点

    // 初始化入度
    for (const auto& pair : adjList) {
        int u = pair.first;
        inDegree[u] = 0; // 所有节点初始入度为 0
    }
    for (const auto& pair : adjList)
        for (int v : pair.second)
            inDegree[v]++; // 计算每个节点的入度

    // 将入度为 0 的节点加入队列
    for (const auto& pair : inDegree)
        if (pair.second == 0)
            zeroInDegreeQueue.push(pair.first);

    while (!zeroInDegreeQueue.empty()) {
        int u = zeroInDegreeQueue.front();
        zeroInDegreeQueue.pop();
        result.push_back(u);

        // 更新邻接节点的入度
        for (int v : adjList[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0)
                zeroInDegreeQueue.push(v);
        }
    }

    // 返回是否有有向环
    return result.size() == adjList.size();
}

/* 计算事件的最早最晚发生时间 */
void Graph::calculateEarlyLateTimes()
{
    std::vector<int> topoOrder;
    // 如果拓扑排序失败，直接返回
    if (!topologicalSort(topoOrder))
        return;

    // 初始化最早开始时间 Ve(i)，将所有顶点的最早开始时间设为0
    for (const auto& vertex : adjList)
        VeiStart[vertex.first] = 0;

    // 计算最早开始时间 Ve(i)
    for (int i = 0; i < (int)topoOrder.size(); ++i) {
        int u = topoOrder[i];
        for (int v : adjList[u]) {
            int weight = weights[{u, v}];
            // 如果 v 的当前 Ve(v) 小于 Ve(u) + 边 (u, v) 的权重，则更新 Ve(v)
            if (VeiStart[v] < VeiStart[u] + weight)
                VeiStart[v] = VeiStart[u] + weight;
        }
    }

    // 计算整个工程的最早完成时间
    earliestCompletionTime = 0;
    for (const auto& vertex : VeiStart)
        // 找到最大的 Ve(i) 作为最早完成时间
        if (vertex.second > earliestCompletionTime)
            earliestCompletionTime = vertex.second;

    // 初始化最晚开始时间 Vl(i)，将所有顶点的最晚开始时间设为整个工程的最早完成时间
    for (const auto& vertex : adjList)
        VliStart[vertex.first] = earliestCompletionTime;

    // 按照拓扑排序的逆序计算最晚开始时间 Vl(i)
    for (int i = topoOrder.size() - 1; i >= 0; --i) {
        int v = topoOrder[i];
        for (int u : adjList[v]) {
            int weight = weights[{v, u}];
            // 如果 v 的当前 Vl(v) 大于 Vl(u) - 边 (v, u) 的权重，则更新 Vl(v)
            if (VliStart[v] > VliStart[u] - weight)
                VliStart[v] = VliStart[u] - weight;
        }
    }
}

/* 获取事件最早开始的时间表 */
std::map<int, int> Graph::getVei()
{
    return VeiStart;
}

/* 获取事件最晚开始的时间表 */
std::map<int, int> Graph::getVli()
{
    return VliStart;
}

/* 获取工程最早完成的时间 */
int Graph::getearliestTime()
{
    return earliestCompletionTime;
}

/* 计算活动的最早最晚完成时间 */
void Graph::calculateActTimes()
{
    for (const auto& edge : weights) {
        int u = edge.first.first;
        int v = edge.first.second;
        int w = edge.second;

        // 计算松弛时间
        int slack = VliStart[v] - VeiStart[u] - w;

        eliStart[{u, v}].e = VeiStart[u];
        eliStart[{u, v}].l = VliStart[v] - w;

        // 如果松弛时间为0，则该活动是关键活动
        if (slack == 0) {
            eliStart[{u, v}].isCritical = true;
            criticalActs.insert(u);
            criticalActs.insert(v);
        }
        else
            eliStart[{u, v}].isCritical = false;
    }
}

/* 获取活动的完成时间信息 */
std::map<std::pair<int, int>, ActTime> Graph::geteli()
{
    return eliStart;
}

/* 获取所有的关键活动 */
std::set<int> Graph::getcriticalActs()
{
    return criticalActs;
}
