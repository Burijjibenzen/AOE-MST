#ifndef FLOYD_H
#define FLOYD_H

#include <QObject>
#include "graph.h"

/* Floyd 继承 Graph 类 */
class Floyd : public QObject, public Graph
{
    Q_OBJECT
public:
    Floyd(Graph g); // 构造函数复制父类基本信息
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>> floydWarshall(); // 计算每对顶点之间的最短路径
};

#endif // FLOYD_H
