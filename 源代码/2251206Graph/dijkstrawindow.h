#ifndef DIJKSTRAWINDOW_H
#define DIJKSTRAWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "dijkstra.h"

namespace Ui {
class DijkstraWindow;
}

class DijkstraWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DijkstraWindow(Graph g, QWidget *parent = nullptr); // 重写构造函数以维护已建立好的图
    ~DijkstraWindow();
    void paintEvent(QPaintEvent * ev); // 重写绘图事件

private:
    Ui::DijkstraWindow *ui;
    Dijkstra * dijk = NULL;

signals:
    void back_to_menu(); // 返回菜单信号

private slots:
    void distDisplay(); // 显示最短路径
};

#endif // DIJKSTRAWINDOW_H
