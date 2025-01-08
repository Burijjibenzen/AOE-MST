#ifndef PRIMWINDOW_H
#define PRIMWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "prim.h"

namespace Ui {
class PrimWindow;
}

class PrimWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrimWindow(Graph g, int startVertex, QWidget *parent = nullptr); // 重写构造函数以维护已建立好的图
    ~PrimWindow();
    void paintEvent(QPaintEvent * ev); // 重写绘图事件

private:
    Ui::PrimWindow *ui;
    Prim * prim = NULL;

private slots:
    void MSTUpdate_slot(const std::vector<Edge>& mst); // 更新生成树槽函数
    void LeftUpdate_slot(std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> left); // 更新剩余的边槽函数
    void ChosenUpdate_slot(Edge chosen); // 更新选中的边槽函数
    void judgeover(); // 判断算法是否完成
    void MSTsetUpdate_slot(std::set<int> mst_set); // 更新已访问过的顶点槽函数

signals:
    void back_to_menu(); // 返回菜单信号
};

#endif // PRIMWINDOW_H
