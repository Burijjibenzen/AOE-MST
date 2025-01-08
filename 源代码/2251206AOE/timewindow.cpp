#include "timewindow.h"
#include "ui_timewindow.h"
#include "graph.h"
#include <QPainter>
#include <QPainterPath>

TimeWindow::TimeWindow(Graph g, std::vector<Circle> cir, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeWindow)
{
    ui->setupUi(this);
    setWindowTitle("关键路径计算");
    graph = g;
    circles = cir;

    // 计算事件时间
    graph.calculateEarlyLateTimes();

    // 表格绘制
    ui->tableWidget->setRowCount(3);
    QStringList headers1;
    headers1 << "事件" << "最早开始时间 / 全工程最早完成时间" << "最迟开始时间";
    ui->tableWidget->setVerticalHeaderLabels(headers1);
    ui->tableWidget->horizontalHeader()->hide(); // 取消列号
    VertexTableWidget(graph.getVei(), graph.getVli(), graph.getearliestTime());

    // 计算活动时间
    graph.calculateActTimes();

    // 表格绘制
    ui->ActTableWidget->setRowCount(4);
    QStringList headers2;
    headers2 << "活动" << "最早开始时间" << "最迟开始时间" << "是否关键";
    ui->ActTableWidget->setVerticalHeaderLabels(headers2);
    ui->ActTableWidget->horizontalHeader()->hide(); // 取消列号
    EdgeTableWidget(graph.geteli());

    // 信号槽连接 关键活动 / 路径的显示
    connect(ui->pushButton, &QPushButton::clicked, this, [=](){
        graph.ready = true;
        if (graph.getWeights().empty() == true)
            ui->statusbar->showMessage(tr("无关键路径 / 活动"),2000);
        update();
    });
}

TimeWindow::~TimeWindow()
{
    delete ui;
}

void TimeWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // 创建线性渐变，从左上角到右下角
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, Qt::darkGray);     // 渐变起点颜色
    gradient.setColorAt(1, Qt::darkBlue);     // 渐变终点颜色

    // 用渐变填充整个窗口
    painter.fillRect(rect(), gradient);

    painter.setRenderHint(QPainter::Antialiasing); // 消除锯齿，使图形更加平滑

    // 设置绘图区域
    QRect drawingArea(50, 50, width() - 100, height() - 390);
    painter.setClipRect(drawingArea);

    // 设置字体
    QFont font = painter.font();

    // 设置圆的大小
    int radius = 20;

    // 黑色填充背景
    painter.setBrush(Qt::black);
    painter.drawRect(drawingArea);

    // 绘制所有的边（箭头）
    auto adjList = graph.getAdjList();
    auto weights = graph.getWeights();  // 获取边的权重
    for (const auto& pair : adjList) {  // 每一条边画一条箭头
        int u = pair.first;
        QPoint uPos;

        for (int v : pair.second) {
            QPoint vPos;

            for (const Circle& circle : circles)
                if (circle.id == u)
                    uPos = circle.position;
                else if (circle.id == v)
                    vPos = circle.position;

            // 计算线的中点
            QPoint midPoint = (uPos + vPos) / 2;

            // 获取边的权重
            int weight = weights[{u, v}];

            // 设置字体和颜色
            font.setPointSize(9);
            font.setBold(false);
            painter.setFont(font);

            // 关键路径高亮
            auto eli = graph.geteli();
            if (eli[{u, v}].isCritical == true && graph.ready == true)
                painter.setPen(Qt::yellow);
            else
                painter.setPen(Qt::lightGray);

            // 在中点位置绘制权重值
            painter.drawText(midPoint, QString::number(weight));

            // 画线
            painter.drawLine(uPos, vPos);

            // 画箭头，三角函数计算角度
            QLineF line(uPos, vPos);
            double angle = std::atan2(-line.dy(), line.dx());
            QPointF arrowP1 = vPos + QPointF(-std::sin(M_PI / 3 - angle) * 10,
                                             std::cos(M_PI / 3 - angle) * 10);
            QPointF arrowP2 = vPos + QPointF(-std::sin(2 * M_PI / 3 - angle) * 10,
                                             std::cos(2 * M_PI / 3 - angle) * 10);
            painter.drawLine(vPos, arrowP1);
            painter.drawLine(vPos, arrowP2);
        }
    }

    // 绘制所有的圆
    for (const Circle& circle : circles) {

        // 设置字体和填充色，使得箭头对数字没有遮挡
        painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
        font.setPointSize(12);
        font.setBold(true);
        painter.setFont(font);

        // 关键路径高亮
        std::set<int> criticalActs = graph.getcriticalActs();
        if (criticalActs.find(circle.id) != criticalActs.end() && graph.ready == true)
            painter.setPen(Qt::cyan);
        else
            painter.setPen(Qt::white);

        // 绘制圆
        painter.drawEllipse(circle.position, radius, radius);

        // 创建编号轮廓，美化效果
        QPainterPath path;
        QPoint textPos;
        if (circle.id < 10)
            textPos = circle.position + QPoint(-radius / 4, radius / 4);
        else
            textPos = circle.position + QPoint(-radius / 2, radius / 4);

        path.addText(textPos, painter.font(), QString::number(circle.id));

        // 先绘制轮廓
        painter.setPen(QPen(Qt::blue, 2));
        painter.drawPath(path);

        // 绘制圆的编号，使编号在圆中央
        if (criticalActs.find(circle.id) != criticalActs.end() && graph.ready == true)
            painter.setPen(Qt::cyan);
        else
            painter.setPen(Qt::darkCyan);

        if (circle.id < 10)
            painter.drawText(circle.position + QPoint(-radius / 4, radius / 4), QString::number(circle.id));
        else
            painter.drawText(circle.position + QPoint(-radius / 2, radius / 4), QString::number(circle.id));
    }
}

/* 计算事件时间表 */
void TimeWindow::VertexTableWidget(const std::map<int, int>& VeiTimes,
                                   const std::map<int, int>& VliTimes,
                                   int earliestCompletionTime)
{
    int col = 0;
    for (const auto& pair : VeiTimes) {
        int vertex = pair.first;
        int Ve_i = pair.second;
        int Vl_i = VliTimes.at(vertex);

        ui->tableWidget->insertColumn(col); // 插入新列

        // 添加事件编号到表格
        QTableWidgetItem* vertexItem = new QTableWidgetItem(QString::number(vertex));
        vertexItem->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
        ui->tableWidget->setItem(0, col, vertexItem);

        // 添加最早开始时间到表格
        QTableWidgetItem* Ve_i_Item = new QTableWidgetItem(QString::number(Ve_i));
        Ve_i_Item->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
        ui->tableWidget->setItem(1, col, Ve_i_Item);

        // 添加最晚开始时间到表格
        QTableWidgetItem* Vl_i_Item = new QTableWidgetItem(QString::number(Vl_i));
        Vl_i_Item->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
        ui->tableWidget->setItem(2, col, Vl_i_Item);

        col++; // 移动到下一列
    }

    // 添加表示整个工程最早完成时间的额外列
    ui->tableWidget->insertColumn(col);

    QTableWidgetItem* projectItem = new QTableWidgetItem("全工程");
    projectItem->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
    ui->tableWidget->setItem(0, col, projectItem);

    // 添加最早完成时间到表格
    QTableWidgetItem* completionTimeItem = new QTableWidgetItem(QString::number(earliestCompletionTime));
    completionTimeItem->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
    ui->tableWidget->setItem(1, col, completionTimeItem);

    QTableWidgetItem* dashItem = new QTableWidgetItem("-");
    dashItem->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
    ui->tableWidget->setItem(2, col, dashItem);
}

/* 计算活动时间表 */
void TimeWindow::EdgeTableWidget(const std::map<std::pair<int, int>, ActTime>& eliTimes)
{
    int col = 0;
    for (const auto& pair : eliTimes) {
        int u = pair.first.first;
        int v = pair.first.second;
        int e = pair.second.e;
        int l = pair.second.l;
        bool critical = pair.second.isCritical;

        ui->ActTableWidget->insertColumn(col); // 插入新列

        // 添加活动编号到表格（格式为 "<u, v>"）
        QTableWidgetItem* ActItem = new QTableWidgetItem(QString("<%1, %2>").arg(u).arg(v));
        ActItem->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
        ui->ActTableWidget->setItem(0, col, ActItem);

        // 添加最早开始时间到表格
        QTableWidgetItem* e_i_Item = new QTableWidgetItem(QString::number(e));
        e_i_Item->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
        ui->ActTableWidget->setItem(1, col, e_i_Item);

        // 添加最晚开始时间到表格
        QTableWidgetItem* l_i_Item = new QTableWidgetItem(QString::number(l));
        l_i_Item->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
        ui->ActTableWidget->setItem(2, col, l_i_Item);

        // 添加关键活动标识到表格
        QTableWidgetItem* critical_Item;
        if (critical == true)
            critical_Item = new QTableWidgetItem(QString("是"));
        else
            critical_Item = new QTableWidgetItem(QString("否"));
        critical_Item->setTextAlignment(Qt::AlignCenter); // 文本居中对齐
        ui->ActTableWidget->setItem(3, col, critical_Item);

        col++; // 移动到下一列
    }
}
