#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "graphwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 400);
    setWindowTitle("数据结构课程设计");

    // 状态栏右侧信息
    QLabel * label = new QLabel("2251206 冯羽芯", this);
    ui->statusbar->addPermanentWidget(label);

    // 信号槽连接 实现界面切换
    c_win = new CircleWindow;
    connect(ui->pushButton, &QPushButton::clicked, this, [=](){
        c_win->setFixedSize(800, 800);

        this->hide();
        c_win->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 重写绘图事件 实现背景绘制 */
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // 创建线性渐变，从左上角到右下角
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, Qt::darkGray);     // 渐变起点颜色
    gradient.setColorAt(1, Qt::darkBlue);     // 渐变终点颜色

    // 用渐变填充整个窗口
    painter.fillRect(rect(), gradient);
}
