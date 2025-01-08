#ifndef KRUSKALWINDOW_H
#define KRUSKALWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "kruskal.h"

namespace Ui {
class KruskalWindow;
}

class KruskalWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit KruskalWindow(Graph g, QWidget *parent = nullptr); // 重写构造函数以维护已建立好的图
    ~KruskalWindow();
    void paintEvent(QPaintEvent * ev); // 重写绘图事件

private slots:
    void MSTUpdate_slot(const std::vector<Edge>& mst); // 更新生成树槽函数
    void LeftUpdate_slot(std::vector<Edge> left);      // 更新剩余的边槽函数
    void ChosenUpdate_slot(Edge chosen);               // 更新选中的边槽函数
    void judgeover();                                  // 判断算法是否完成

signals:
    void back_to_menu(); // 返回菜单信号

private:
    Ui::KruskalWindow *ui;
    Kruskal * kru = NULL;
};

#endif // KRUSKALWINDOW_H
