#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <QPoint>
#include <map>
#include <vector>
#include <set>

/* 绘制的圆圈 */
struct Circle {
    QPoint position;
    int id;
};

/* 每个活动的时间信息 */
struct ActTime {
    int e; // 活动的最早开始时间
    int l; // 活动的最晚开始时间
    bool isCritical; // 是否关键
};

/* 有向无环图 */
class Graph
{
public:
    void addEdge(int u, int v, int w); // 新增边
    void removeEdge(int u, int v);     // 删除边
    void addVertex(int vt);    // 添加顶点
    void removeVertex(int vt); // 删除顶点
    void calculateEarlyLateTimes(); // 计算事件时间
    void calculateActTimes();       // 计算活动时间

    std::map<int, std::list<int>> getAdjList();      // 获取当前邻接链表
    std::map<std::pair<int, int>, int> getWeights(); // 获取当前权重
    std::map<int, int> getVei(); // 获取每个事件的最早开始时间 Ve(i)
    std::map<int, int> getVli(); // 获取每个事件的最晚开始时间 Vl(i)
    std::map<std::pair<int, int>, ActTime> geteli(); // 获取每个活动的时间信息
    std::set<int> getcriticalActs(); // 获取所有关键活动

    bool topologicalSort(std::vector<int>& result); // 拓扑排序函数
    bool ready = false; // 是否准备好显示关键路径和关键活动

    int getearliestTime(); // 获取整个工程完成的最早时间

protected:
    std::map<int, std::list<int>> adjList;      // 邻接链表
    std::map<std::pair<int, int>, int> weights; // 权重表
    std::map<std::pair<int, int>, ActTime> eliStart; // 每个活动的时间信息
    std::map<int, int> VeiStart; // 每个事件的最早开始时间 Ve(i)
    std::map<int, int> VliStart; // 每个事件的最晚开始时间 Vl(i)
    std::set<int> criticalActs; // 所有关键活动

    int earliestCompletionTime; // 整个工程完成的最早时间
};

#endif // GRAPH_H
