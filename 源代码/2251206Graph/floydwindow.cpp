#include "floydwindow.h"
#include "ui_floydwindow.h"
#include <QPainter>

FloydWindow::FloydWindow(Graph g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FloydWindow)
{
    ui->setupUi(this);
    setWindowTitle("每一对顶点之间的最短路径");
    floyd = new Floyd(g); // 构造函数

    connect(ui->backBtn, &QPushButton::clicked, this, [=](){ // 返回
        emit back_to_menu();
    });

    showTable(); // 显示最短路径表
}

FloydWindow::~FloydWindow()
{
    delete ui;
}

/* 显示最短路径表 */
void FloydWindow::showTable()
{
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<std::vector<int>>>> dist = floyd->floydWarshall();

    int V = dist.first.size() - 1;

    std::vector<std::vector<std::vector<int>>> paths = dist.second;

    // 设置表格的行数和列数
    ui->tableWidget->setRowCount(V);
    ui->tableWidget->setColumnCount(V);

    ui->pathWidget->setRowCount(V);
    ui->pathWidget->setColumnCount(V);

    // 设置表头
    QStringList headers;
    auto adjList = floyd->getAdjList();

    for (const auto& pair : adjList)
        headers << QString::number(pair.first);

    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setVerticalHeaderLabels(headers);

    ui->pathWidget->setHorizontalHeaderLabels(headers);
    ui->pathWidget->setVerticalHeaderLabels(headers);

    // 填充表格
    for (int i = 1; i <= V; ++i)
        for (int j = 1; j <= V; ++j) {
            QString text, path;
            if (dist.first[i][j] == std::numeric_limits<int>::max())
                text = "INF";  // 如果是无穷大，用 "INF" 表示
            else
                text = QString::number(dist.first[i][j]);

            QTableWidgetItem* item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter); // 文本居中
            ui->tableWidget->setItem(i - 1, j - 1, item);

            for (int v : paths[i][j])
                path += QString::number(v) + " ";

            QTableWidgetItem* item2 = new QTableWidgetItem(path);
            item2->setTextAlignment(Qt::AlignCenter); // 文本居中
            ui->pathWidget->setItem(i - 1, j - 1, item2);
        }
}

/* 重写绘图事件 实现背景绘制 */
void FloydWindow::paintEvent(QPaintEvent * ev)
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
