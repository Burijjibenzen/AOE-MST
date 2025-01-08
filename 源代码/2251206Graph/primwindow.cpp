#include "primwindow.h"
#include "ui_primwindow.h"
#include <QPainter>

PrimWindow::PrimWindow(Graph g, int startVertex, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PrimWindow)
{
    ui->setupUi(this);

    setWindowTitle("普里姆算法实现最小生成树");

    // 状态栏右侧信息
    QString str = QString("起始顶点为：%1").arg(startVertex);
    QLabel * label = new QLabel(str, this);
    ui->statusbar->addPermanentWidget(label);

    prim = new Prim(g); // 构造函数
    prim->initialize(startVertex); // 初始化

    // 信号槽连接 按下按钮每次实现一个步骤
    connect(ui->nextBtn, &QPushButton::clicked, prim, &Prim::nextStep);
    connect(prim, &Prim::mstUpdated, this, &PrimWindow::MSTUpdate_slot);
    connect(prim, &Prim::chosenUpdated, this, &PrimWindow::ChosenUpdate_slot);
    connect(prim, &Prim::leftUpdated, this, &PrimWindow::LeftUpdate_slot);
    connect(ui->nextBtn, &QPushButton::clicked, this, &PrimWindow::judgeover);
    connect(prim, &Prim::visitedUpdated, this, &PrimWindow::MSTsetUpdate_slot);
    connect(ui->backBtn, &QPushButton::clicked, this, [=](){ // 返回
        emit back_to_menu();
    });
}

PrimWindow::~PrimWindow()
{
    delete ui;
}

/* 更新生成树槽函数 */
void PrimWindow::MSTUpdate_slot(const std::vector<Edge>& mst)
{
    QString MSTText;
    for (const Edge& edge : mst) {
        MSTText += QString::number(edge.u) + "--" + QString::number(edge.v) + " weight: " + QString::number(edge.weight);
        MSTText += "\n";
    }
    ui->MSTEdge->setText(MSTText);
}

/* 更新选中的边槽函数 */
void PrimWindow::ChosenUpdate_slot(Edge chosen)
{
    QString ChosenText;
    ChosenText += QString::number(chosen.u) + "--" + QString::number(chosen.v) + " weight: " + QString::number(chosen.weight);
    ui->chosenEdge->setText(ChosenText);
}

/* 更新剩余的边槽函数 */
void PrimWindow::LeftUpdate_slot(std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> left)
{
    QString LeftText;

    // 遍历优先级队列
    while (!left.empty()) {
        Edge edge = left.top();
        left.pop();  // 弹出队列顶元素

        // 构建输出文本
        LeftText += QString::number(edge.u) + "--" + QString::number(edge.v) + " weight: " + QString::number(edge.weight);
        LeftText += "\n";
    }

    ui->leftEdge->setText(LeftText);
}

/* 判断算法是否完成 */
void PrimWindow::judgeover()
{
    if (prim->over() == true) {
        ui->statusbar->showMessage(tr("最小生成树生成完毕，请单击“返回”跳转至菜单"),2000);
        return;
    }
}

/* 更新已访问过的顶点槽函数 */
void PrimWindow::MSTsetUpdate_slot(std::set<int> mst_set)
{
    QString MSTsetText;

    // 遍历 set 中的所有元素
    for (int vertex : mst_set)
        MSTsetText += QString::number(vertex) + " ";

    ui->visited->setText(MSTsetText);
}

/* 重写绘图事件 实现背景绘制 */
void PrimWindow::paintEvent(QPaintEvent * ev)
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
