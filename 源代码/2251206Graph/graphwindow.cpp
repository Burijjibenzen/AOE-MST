#include "graphwindow.h"
#include "ui_graphwindow.h"
#include <algorithm>
#include <list>
#include <QMessageBox>
#include <QPainter>

GraphWindow::GraphWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_g(new Ui::GraphWindow)
{
    ui_g->setupUi(this);

    setWindowTitle("图的创建与邻接表显示");

    // 按钮按下 信号槽连接
    connect(ui_g->addEdgeButton, &QPushButton::clicked, this, &GraphWindow::addEdgeButton_clicked);
    connect(ui_g->removeEdgeButton, &QPushButton::clicked, this, &GraphWindow::removeEdgeButton_clicked);
    connect(ui_g->vtAddBtn, &QPushButton::clicked, this, &GraphWindow::addVertexButton_clicked);
    connect(ui_g->vtRemoveBtn, &QPushButton::clicked, this, &GraphWindow::removeVertexButton_clicked);
    connect(ui_g->judge, &QPushButton::clicked, this, &GraphWindow::judge_clicked);
}

GraphWindow::~GraphWindow()
{
    delete ui_g;
}

/* 添加边 */
void GraphWindow::addEdgeButton_clicked()
{
    int u = ui_g->vertexUInput->text().toInt();
    int v = ui_g->vertexVInput->text().toInt();
    int w = ui_g->weightInput->text().toInt();

    if (u <= 0 || v <= 0) { // 非法输入
        ui_g->statusbar->showMessage(tr("非法操作：顶点名称应为大于 0 的整数"),2000);
        return;
    }
    else if (u == v) { // 不能成环
        ui_g->statusbar->showMessage(tr("非法操作：不能添加从自身到自身的边"),2000);
        return;
    }

    auto adjList = graph.getAdjList();
    if (adjList.find(u) == adjList.end()) { // 不存在顶点
        ui_g->statusbar->showMessage(tr("非法操作：请先创建顶点"),2000);
        return;
    }
    if (adjList.find(v) == adjList.end()) { // 不存在顶点
        ui_g->statusbar->showMessage(tr("非法操作：请先创建顶点"),2000);
        return;
    }

    std::list<int>::iterator it = std::find(adjList[u].begin(), adjList[u].end(), v);
    if (it != adjList[u].end()) { // 已经有边了
        ui_g->statusbar->showMessage(tr("非法操作：已存在边，不能重复添加边"),2000);
        return;
    }

    if (w <= 0) { // 权重错误
        ui_g->statusbar->showMessage(tr("非法操作：权重应为大于 0 的整数"),2000);
        return;
    }

    graph.addEdge(u, v, w);
    updateAdjListDisplay();
}

/* 删除边 */
void GraphWindow::removeEdgeButton_clicked()
{
    int u = ui_g->vertexUInput->text().toInt();
    int v = ui_g->vertexVInput->text().toInt();
    ui_g->weightInput->clear();

    if (u <= 0 || v <= 0) { // 非法输入
        ui_g->statusbar->showMessage(tr("非法操作：顶点名称应为大于 0 的整数"),2000);
        return;
    }

    auto adjList = graph.getAdjList();
    std::list<int>::iterator it = std::find(adjList[u].begin(), adjList[u].end(), v);
    if (it == adjList[u].end()) { // 已经没边了
        ui_g->statusbar->showMessage(tr("非法操作：不能删除不存在的边"),2000);
        return;
    }

    graph.removeEdge(u, v);
    updateAdjListDisplay();
}

/* 显示邻接链表 */
void GraphWindow::updateAdjListDisplay()
{
    QString adjListText;
    auto adjList = graph.getAdjList();

    for (const auto &pair : adjList) {
        adjListText += QString::number(pair.first) + ": ";
        for (int vertex : pair.second)
            adjListText += QString::number(vertex) + " -> ";
        adjListText += "^\n";
    }

    ui_g->adjListDisplay->setText(adjListText);
}

/* 添加顶点 */
void GraphWindow::addVertexButton_clicked()
{
    int vt = ui_g->vtNumber->text().toInt();
    ui_g->vtNumber->clear();

    if (vt <= 0) { // 非法输入
        ui_g->statusbar->showMessage(tr("非法操作：顶点名称应为大于 0 的整数"),2000);
        return;
    }

    auto adjList = graph.getAdjList();
    if (adjList.find(vt) != adjList.end()) { // 存在顶点
        ui_g->statusbar->showMessage(tr("非法操作：不能重复创建已存在的顶点"),2000);
        return;
    }

    graph.addVertex(vt);
    updateAdjListDisplay();
}

/* 删除顶点 */
void GraphWindow::removeVertexButton_clicked()
{
    int vt = ui_g->vtNumber->text().toInt();
    ui_g->vtNumber->clear();

    if (vt <= 0) { // 非法输入
        ui_g->statusbar->showMessage(tr("非法操作：顶点名称应为大于 0 的整数"),2000);
        return;
    }

    auto adjList = graph.getAdjList();
    if (adjList.find(vt) == adjList.end()) { // 不存在顶点
        ui_g->statusbar->showMessage(tr("非法操作：不能删除不存在的顶点"),2000);
        return;
    }

    graph.removeVertex(vt);
    updateAdjListDisplay();
}

/* 判断是否是连通图 */
void GraphWindow::judge_clicked()
{
    auto adjList = graph.getAdjList();
    if (adjList.empty()) { // 空图
        QMessageBox::critical(this, "非法操作", "该图是空图");
        return;
    }

    if (adjList.size() <= 1) { // 只有一个顶点
        QMessageBox::critical(this, "非法操作", "该图顶点不足");
        return;
    }

    bool isconnected = graph.isConnected();
    if (isconnected == false) {
        QMessageBox::critical(this, "非法操作", "该图不是连通图，无法进行后续求最小生成树和最短路径操作");
        return;
    }
    else { // 进入算法选择窗口
        this->hide();
        choose_win = new ChooseWindow(graph);
        // 设置 choose_win 场景的位置
        choose_win->setGeometry(this->geometry());
        choose_win->setFixedSize(400, 200);

        choose_win->show();

        connect(choose_win, &ChooseWindow::recreate, this, [=](){
            this->setGeometry(choose_win->geometry());
            this->show();
            delete choose_win;
            choose_win = NULL;
        });
    }
}

/* 重写绘图事件 实现背景绘制 */
void GraphWindow::paintEvent(QPaintEvent * ev)
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
