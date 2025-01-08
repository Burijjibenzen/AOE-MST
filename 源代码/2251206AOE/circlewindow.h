#ifndef CIRCLEWINDOW_H
#define CIRCLEWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include "graph.h"
#include "timewindow.h"

namespace Ui {
class CircleWindow;
}

class CircleWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CircleWindow(QWidget *parent = nullptr);
    ~CircleWindow();

protected:
    void paintEvent(QPaintEvent *event) override;      // 重写绘图事件
    void mousePressEvent(QMouseEvent *event) override; // 重写鼠标按下事件

private:
    Ui::CircleWindow *ui;
    std::vector<Circle> circles; // 存储圆的列表
    int nextId = 1;              // 下一个圆的编号
    Graph graph;                 // 有向无环图
    TimeWindow * t_win;          // 计算各事件和活动时间的窗口

private slots:
    void undoLastCircle();           // 撤销
    void addEdgeButton_clicked();    // 添加边
    void removeEdgeButton_clicked(); // 删除边
    void updateAdjListDisplay();     // 更新邻接链表
    void judge_clicked();            // 判断工程是否可行
};

#endif // CIRCLEWINDOW_H
