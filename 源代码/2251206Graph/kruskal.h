#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include "graph.h"
#include <QObject>

class DisjointSet { // 并查集
public:
    DisjointSet(int n); // 构造函数，初始化 n 个元素的并查集
    int find(int u); // 查找元素 u 所属集合的根节点，并进行路径压缩
    void unionSets(int u, int v); // 合并元素 u 和 v 所在的集合

private:
    std::vector<int> parent; // 存储每个元素的父节点
    std::vector<int> rank;   // 存储每个集合的秩，用于优化合并操作
};

class Kruskal : public QObject, public Graph // 继承 QObject 来发送信号
{
    Q_OBJECT //需要添加这个
public:
    Kruskal(Graph g);
    void initialize(); // Kruskal 算法初始化
    void nextStep(); // 下一步 实现 按下更新
    bool judgeCircle(); // 判断是否成环
    bool over(); // 是否结束

private:
    std::vector<Edge> edges; // 所有的边
    std::vector<Edge> mst;   // 最小生成树中的边
    std::vector<Edge> left;  // 剩余的边

    DisjointSet ds; // 并查集

    int currentEdgeIndex; // 当前边号

    bool isCircle; // 是否成环
    bool isOver = false; // 是否结束

signals:
    void mstUpdated(std::vector<Edge> mst);  // 生成树中的边
    void chosenUpdated(Edge chosen); // 当前选中的边
    void leftUpdated(std::vector<Edge> left); // 剩余的边
};

#endif // KRUSKAL_H
