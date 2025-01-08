#ifndef TIMEWINDOW_H
#define TIMEWINDOW_H

#include <QMainWindow>
#include "graph.h"

namespace Ui {
class TimeWindow;
}

class TimeWindow : public QMainWindow
{
    Q_OBJECT

public:
    TimeWindow(Graph g, std::vector<Circle> circles, QWidget *parent = nullptr); // 时间计算窗口构造函数传参初始化
    ~TimeWindow();

protected:
    void paintEvent(QPaintEvent *event); // 重写绘图事件
    void VertexTableWidget(const std::map<int, int>& VeiTimes,
                           const std::map<int, int>& VliTimes,
                           int earliestCompletionTime); // 计算事件时间表
    void EdgeTableWidget(const std::map<std::pair<int, int>, ActTime>& eliTimes); // 计算活动时间表

private:
    Ui::TimeWindow *ui;
    Graph graph;                 // 有向无环图
    std::vector<Circle> circles; // 存储圆的列表
};

#endif // TIMEWINDOW_H
