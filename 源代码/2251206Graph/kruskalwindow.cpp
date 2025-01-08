#include "kruskalwindow.h"
#include "ui_kruskalwindow.h"
#include <QPainter>

KruskalWindow::KruskalWindow(Graph g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::KruskalWindow)
{
    ui->setupUi(this);

    setWindowTitle("克鲁斯卡尔算法实现最小生成树");

    kru = new Kruskal(g); // 构造函数
    kru->initialize(); // 初始化

    // 信号槽连接 按下按钮每次实现一个步骤
    connect(ui->nextBtn, &QPushButton::clicked, kru, &Kruskal::nextStep);
    connect(kru, &Kruskal::mstUpdated, this, &KruskalWindow::MSTUpdate_slot);
    connect(kru, &Kruskal::chosenUpdated, this, &KruskalWindow::ChosenUpdate_slot);
    connect(kru, &Kruskal::leftUpdated, this, &KruskalWindow::LeftUpdate_slot);
    connect(ui->nextBtn, &QPushButton::clicked, this, &KruskalWindow::judgeover);
    connect(ui->backBtn, &QPushButton::clicked, this, [=](){ // 返回
        emit back_to_menu();
    });
}

KruskalWindow::~KruskalWindow()
{
    delete ui;
}

/* 更新生成树槽函数 */
void KruskalWindow::MSTUpdate_slot(const std::vector<Edge>& mst)
{
    QString MSTText;
    for (const Edge& edge : mst) {
        MSTText += QString::number(edge.u) + "--" + QString::number(edge.v) + " weight: " + QString::number(edge.weight);
        MSTText += "\n";
    }
    ui->MSTEdge->setText(MSTText);
}

/* 更新剩余的边槽函数 */
void KruskalWindow::LeftUpdate_slot(std::vector<Edge> left)
{
    QString LeftText;
    for (Edge edge : left) {
        LeftText += QString::number(edge.u) + "--" + QString::number(edge.v) + " weight: " + QString::number(edge.weight);
        LeftText += "\n";
    }
    ui->leftEdge->setText(LeftText);
}

/* 更新选中的边槽函数 */
void KruskalWindow::ChosenUpdate_slot(Edge chosen)
{
    QString ChosenText;
    ChosenText += QString::number(chosen.u) + "--" + QString::number(chosen.v) + " weight: " + QString::number(chosen.weight);
    ui->chosenEdge->setText(ChosenText);

    if (kru->judgeCircle() == true) {
        ui->statusbar->showMessage(tr("成环，不加入所选边"),2000);
        return;
    }
}

/* 判断算法是否完成 */
void KruskalWindow::judgeover()
{
    if (kru->over() == true) {
        ui->statusbar->showMessage(tr("最小生成树生成完毕，请单击“返回”跳转至菜单"),2000);
        return;
    }
}

/* 重写绘图事件 实现背景绘制 */
void KruskalWindow::paintEvent(QPaintEvent * ev)
{
    Q_UNUSED(ev);

    QPainter painter(this);

    // 创建线性渐变，从左上角到右下角
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, Qt::darkGray);         // 渐变起点颜色
    gradient.setColorAt(1, Qt::darkBlue);     // 渐变终点颜色

    // 用渐变填充整个窗口
    painter.fillRect(rect(), gradient);
}
