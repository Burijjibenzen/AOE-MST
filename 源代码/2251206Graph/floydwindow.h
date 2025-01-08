#ifndef FLOYDWINDOW_H
#define FLOYDWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "floyd.h"

namespace Ui {
class FloydWindow;
}

class FloydWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FloydWindow(Graph g, QWidget *parent = nullptr); // 重写构造函数以维护已建立好的图
    ~FloydWindow();
    void showTable(); // 显示最短路径表
    void paintEvent(QPaintEvent * ev); // 重写绘图事件

private:
    Ui::FloydWindow *ui;
    Floyd * floyd = NULL;

signals:
    void back_to_menu(); // 返回菜单信号
};

#endif // FLOYDWINDOW_H
