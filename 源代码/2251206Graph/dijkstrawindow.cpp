#include "dijkstrawindow.h"
#include "ui_dijkstrawindow.h"
#include <QPainter>

DijkstraWindow::DijkstraWindow(Graph g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DijkstraWindow)
{
    ui->setupUi(this);
    setWindowTitle("某一确定顶点到所有其它顶点的最短路径");
    dijk = new Dijkstra(g); // 构造函数

    connect(ui->sourceBtn, &QPushButton::clicked, this, &DijkstraWindow::distDisplay);
    connect(ui->backBtn, &QPushButton::clicked, this, [=](){ // 返回
        emit back_to_menu();
    });
}

DijkstraWindow::~DijkstraWindow()
{
    delete ui;
}

/* 显示最短路径 */
void DijkstraWindow::distDisplay()
{
    int vt = ui->vtNumber->text().toInt();
    ui->vtNumber->clear(); // 清空输入框

    auto adjList = dijk->getAdjList();

    // 检查输入是否合法
    if (vt <= 0) { // 非法输入
        ui->statusbar->showMessage(tr("非法操作：顶点名称应为大于 0 的整数"),2000);
        return;
    }
    else if (adjList.find(vt) == adjList.end()) {
        ui->statusbar->showMessage(tr("非法操作：顶点不存在"),2000);
        return;
    }

    QString str = QString("顶点 %1 到所有其它顶点的最短路径").arg(vt);
    ui->label->setText(str);

    // 调用 Dijkstra 算法
    std::pair<std::map<int, int>, std::map<int, std::vector<int>>> ans = dijk->dijkstra(vt);

    std::map<int, int> distances = ans.first;
    std::map<int, std::vector<int>> paths = ans.second;

    // 输出结果
    QString dist;
    for (const auto& pair : distances) {
        int vertex = pair.first;
        int distance = pair.second;
        std::vector<int> path = paths[vertex];

        dist += "顶点 " + QString::number(vt) + " 到顶点 " + QString::number(vertex) + " 的最短距离为 " + QString::number(distance) + "\n";
        dist += "  路径：";
        for (int v : path)
            dist += QString::number(v) + " ";
        dist += "\n";
    }

    ui->distDisplay->setText(dist);
}

/* 重写绘图事件 实现背景绘制 */
void DijkstraWindow::paintEvent(QPaintEvent * ev)
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
