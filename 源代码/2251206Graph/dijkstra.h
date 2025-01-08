#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <QObject>
#include "graph.h"

/* Dijkstra 继承 Graph 类 */
class Dijkstra : public QObject, public Graph
{
    Q_OBJECT
public:
    Dijkstra(Graph g); // 构造函数复制父类基本信息
    std::pair<std::map<int, int>, std::map<int, std::vector<int>>> dijkstra(int start); // 计算单源最短路径
};

#endif // DIJKSTRA_H
