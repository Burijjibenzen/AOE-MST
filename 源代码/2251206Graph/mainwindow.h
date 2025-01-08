#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GraphWindow * graph_win; // 邻接链表窗口
    void paintEvent(QPaintEvent * ev); // 重写绘图事件

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
