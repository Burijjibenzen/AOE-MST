#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "choosewindow.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();
    void paintEvent(QPaintEvent * ev); // 重写绘图事件

private slots:
    void addEdgeButton_clicked();      // 添加边槽函数
    void removeEdgeButton_clicked();   // 删除边槽函数
    void addVertexButton_clicked();    // 添加顶点槽函数
    void removeVertexButton_clicked(); // 删除顶点槽函数
    void updateAdjListDisplay();       // 显示邻接链表
    void judge_clicked();              // 判断图是否连通

private:
    Ui::GraphWindow *ui_g;
    Graph graph; // 构建图
    ChooseWindow * choose_win = NULL; // 算法选择窗口
};

#endif // GRAPHWINDOW_H
