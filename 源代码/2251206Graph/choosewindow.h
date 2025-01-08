#ifndef CHOOSEWINDOW_H
#define CHOOSEWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "kruskalwindow.h"
#include "primwindow.h"
#include "dijkstrawindow.h"
#include "floydwindow.h"

namespace Ui {
class ChooseWindow;
}

class ChooseWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChooseWindow(Graph g, QWidget *parent = nullptr); // 重写构造函数
    ~ChooseWindow();
    void paintEvent(QPaintEvent * ev); // 重写绘图事件

signals:
    void recreate(); // 重新构建图的信号

private slots:
    void startKruskal();  // 创建克鲁斯卡尔算法窗口
    void startPrim();     // 创建普里姆算法窗口
    void startDijkstra(); // 创建迪杰斯特拉算法窗口
    void startFloyd();    // 创建弗洛伊德算法窗口

private:
    Ui::ChooseWindow *ui;
    Graph graph;            // 无向连通图
    KruskalWindow * k_win;  // 克鲁斯卡尔算法窗口
    PrimWindow * p_win;     // 普里姆算法窗口
    DijkstraWindow * d_win; // 迪杰斯特拉算法窗口
    FloydWindow * f_win;    // 弗洛伊德算法窗口
};

#endif // CHOOSEWINDOW_H
