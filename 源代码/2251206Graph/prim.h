#ifndef PRIM_H
#define PRIM_H

#include <QObject>
#include "graph.h"
#include <set>
#include <queue>

class Prim : public QObject, public Graph
{
    Q_OBJECT
public:
    Prim(Graph g);
    void initialize(int start); // Prim 算法初始化
    void nextStep(); // 下一步 实现 按下更新
    bool over(); // 是否结束

private:
    std::vector<Edge> mst; // 最小生成树边集
    std::set<int> inMST; // 最小生成树包含的点集
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> edges; // 优先级队列

    int currentVertex; // 当前顶点
    int totalVertices; // 总顶点数

    bool isOver = false; // 是否结束

signals:
    void mstUpdated(std::vector<Edge> mst);  // 更新生成树中的边
    void chosenUpdated(Edge chosen); // 更新当前选中的边
    void leftUpdated(std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> left); // 更新剩余的边
    void visitedUpdated(std::set<int> mst_set); // 更新已访问过的顶点
};

#endif // PRIM_H
