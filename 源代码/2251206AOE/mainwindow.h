#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "circlewindow.h"

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
    CircleWindow * c_win;                // 创建绘图窗口
    void paintEvent(QPaintEvent *event); // 重写绘图事件

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
