#include "kruskal.h"
#include <algorithm>
#include <QDebug>

Kruskal::Kruskal(Graph g) : ds(g.getAdjList().size())
{
    this->adjList = g.getAdjList();
    this->weights = g.getWeights();
}

/* 构造函数，初始化 n 个元素的并查集 */
DisjointSet::DisjointSet(int n) {
    parent.resize(n + 1);
    rank.resize(n + 1, 0);
    // 每个元素的父节点指向自己，形成 n 个独立的集合
    for (int i = 1; i <= n; ++i)
        parent[i] = i;
}

/* 查找元素 u 所属集合的根节点，并进行路径压缩 */
int DisjointSet::find(int u) {
    if (parent[u] != u)
        // 递归查找 u 的根节点，并将 u 直接连接到根节点
        parent[u] = find(parent[u]);
    return parent[u]; // 返回根节点
}

/* 合并元素 u 和 v 所在的集合 */
void DisjointSet::unionSets(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);
    // 根据 rank 将较小的树合并到较大的树中
    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV])
            parent[rootV] = rootU;
        else if (rank[rootU] < rank[rootV])
            parent[rootU] = rootV;
        else {
            parent[rootV] = rootU; // 如果 rank 相等，选择任意一方为根
            rank[rootU]++;
        }
    }
}

/* Kruskal 算法初始化 */
void Kruskal::initialize() {
    edges.clear();
    mst.clear();
    currentEdgeIndex = 0;
    ds = DisjointSet(adjList.size());

    // 收集图中的所有边
    for (const auto& pair : adjList) {
        int u = pair.first;
        for (int v : pair.second)
            if (u < v) {  // 防止重复添加无向边
                std::pair<int, int> p(u, v);
                edges.push_back({u, v, weights[p]});
            }
    }

    // 将边按权重排序
    std::sort(edges.begin(), edges.end());

    left = edges;
}

/* 下一步 实现 按下更新 */
void Kruskal::nextStep() {
    // 最小生成树生成完毕
    if (currentEdgeIndex >= (int)edges.size()) {
        isOver = true;
        // qDebug() << "结束了";
        return;  // 所有边已处理，算法结束
    }

    // 每次向后选一条边
    const Edge& edge = edges[currentEdgeIndex];
    currentEdgeIndex++;

    emit Kruskal::leftUpdated(left); // 发送信号，更新剩余的边

    if (ds.find(edge.u) != ds.find(edge.v)) { // 加入最小生成树
        isCircle = false; // 未成环

        left.erase(std::remove(left.begin(), left.end(), edge), left.end());

        mst.push_back(edge);
        ds.unionSets(edge.u, edge.v);
        emit Kruskal::mstUpdated(mst);  // 发送信号，更新生成树
    }
    else
        isCircle = true; // 成环不加入
    emit Kruskal::chosenUpdated(edge); // 发送信号，更新选中的边
}

/* 判断是否成环 */
bool Kruskal::judgeCircle()
{
    return isCircle;
}

/* 判断算法是否结束 */
bool Kruskal::over()
{
    return isOver;
}
