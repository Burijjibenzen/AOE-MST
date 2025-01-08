#include "prim.h"

Prim::Prim(Graph g)
{
    this->adjList = g.getAdjList();
    this->weights = g.getWeights();
}

/* Prim 算法初始化 */
void Prim::initialize(int start)
{
    mst.clear();
    inMST.clear();
    edges = {};
    totalVertices = adjList.size();

    inMST.insert(start);
    currentVertex = start;

    for (int v : adjList[start])
        edges.push({start, v, weights[{start, v}]});
}

/* 下一步 实现 按下更新 */
void Prim::nextStep()
{
    if ((int)inMST.size() == totalVertices) {
        isOver = true;
        return; // 所有顶点都包含在MST中，算法结束
    }

    while (!edges.empty()) {
        Edge edge = edges.top();
        emit leftUpdated(edges); // 发送信号，更新剩余的边
        edges.pop();

        emit chosenUpdated(edge); // 发送信号，更新选中的边

        if (inMST.find(edge.v) == inMST.end()) { // 是尚未访问的顶点
            mst.push_back(edge);
            currentVertex = edge.v;
            inMST.insert(edge.v);
            emit visitedUpdated(inMST); // 发送信号，更新访问过的顶点

            for (int neighbor : adjList[edge.v])
                if (inMST.find(neighbor) == inMST.end())
                    edges.push({edge.v, neighbor, weights[{edge.v, neighbor}]});

            emit mstUpdated(mst); // 发送信号，更新生成树
            break;
        }
    }
}

/* 是否结束 */
bool Prim::over()
{
    return isOver;
}
