#include "circlewindow.h"
#include "ui_circlewindow.h"
#include <QMessageBox>
#include <QPainterPath>

CircleWindow::CircleWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CircleWindow)
{
    ui->setupUi(this);
    setWindowTitle("AOE 网络绘制"); // 设置标题

    // 信号槽连接 按钮按下
    connect(ui->undoBtn, &QPushButton::clicked, this, &CircleWindow::undoLastCircle);
    connect(ui->addEdgeButton, &QPushButton::clicked, this, &CircleWindow::addEdgeButton_clicked);
    connect(ui->removeEdgeButton, &QPushButton::clicked, this, &CircleWindow::removeEdgeButton_clicked);
    connect(ui->judge, &QPushButton::clicked, this, &CircleWindow::judge_clicked);
}

CircleWindow::~CircleWindow()
{
    delete ui;
}

/* 重写鼠标按下事件，实现单击绘制顶点 */
void CircleWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint p = event->pos();

        // 单击区域合法
        if (p.x() >= 70 && p.y() >= 70 && p.x() <= width() - 70 && p.y() <= height() - 360) {
            // 创建一个新的圆
            Circle newCircle;
            newCircle.position = event->pos();
            newCircle.id = nextId++;

            // 将圆添加到列表中
            circles.push_back(newCircle);

            // 增加顶点
            graph.addVertex(newCircle.id);
            updateAdjListDisplay();

            // 更新画布，触发重绘事件
            update();
        }
        else if (p.x() >= 50 && p.y() >= 50 && p.x() <= width() - 50 && p.y() <= height() - 340)
            ui->statusbar->showMessage(tr("非法操作：请画在方框内"),2000); // 单击区域非法
    }
}

/* 重写绘图事件，实现单击绘制顶点 */
void CircleWindow::paintEvent(QPaintEvent *event)
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

        // 绘制圆
        painter.setPen(Qt::white);
        painter.drawEllipse(circle.position, radius, radius);

        // 创建编号轮廓，美化效果
        QPainterPath path;
        QPoint textPos;

        // 使编号在圆中央
        if (circle.id < 10)
            textPos = circle.position + QPoint(-radius / 4, radius / 4);
        else
            textPos = circle.position + QPoint(-radius / 2, radius / 4);
        path.addText(textPos, painter.font(), QString::number(circle.id));

        // 先绘制轮廓
        painter.setPen(QPen(Qt::blue, 2));
        painter.drawPath(path);

        // 再绘制文字，使编号在圆中央
        painter.setPen(Qt::darkCyan);
        if (circle.id < 10)
            painter.drawText(circle.position + QPoint(-radius / 4, radius / 4), QString::number(circle.id));
        else
            painter.drawText(circle.position + QPoint(-radius / 2, radius / 4), QString::number(circle.id));
    }
}

/* 撤销操作 */
void CircleWindow::undoLastCircle()
{
    if (!circles.empty()) {
        circles.pop_back();  // 移除最新的顶点
        nextId--;  // 使得编号连续
        graph.removeVertex(nextId); // 删除顶点
        update();  // 重新绘制 AOE 网 更新图案
        updateAdjListDisplay(); // 更新邻接链表
    }
}

/* 添加边 */
void CircleWindow::addEdgeButton_clicked()
{
    int u = ui->vertexUInput->text().toInt();
    int v = ui->vertexVInput->text().toInt();
    int w = ui->weightInput->text().toInt();

    if (u <= 0 || v <= 0) { // 非法输入
        ui->statusbar->showMessage(tr("非法操作：顶点名称应为大于 0 的整数"),2000);
        return;
    }
    else if (u == v) { // 不能成环
        ui->statusbar->showMessage(tr("非法操作：不能添加从自身到自身的边"),2000);
        return;
    }

    auto adjList = graph.getAdjList();
    if (adjList.find(u) == adjList.end()) { // 不存在顶点
        ui->statusbar->showMessage(tr("非法操作：请先创建顶点"),2000);
        return;
    }
    if (adjList.find(v) == adjList.end()) { // 不存在顶点
        ui->statusbar->showMessage(tr("非法操作：请先创建顶点"),2000);
        return;
    }

    std::list<int>::iterator it = std::find(adjList[u].begin(), adjList[u].end(), v);
    if (it != adjList[u].end()) { // 已经有边了
        ui->statusbar->showMessage(tr("非法操作：已存在边，不能重复添加边"),2000);
        return;
    }

    if (w <= 0) { // 权重错误
        ui->statusbar->showMessage(tr("非法操作：权重应为大于 0 的整数"),2000);
        return;
    }

    graph.addEdge(u, v, w);
    updateAdjListDisplay();

    // 更新绘制
    update();
}

/* 删除边 */
void CircleWindow::removeEdgeButton_clicked()
{
    int u = ui->vertexUInput->text().toInt();
    int v = ui->vertexVInput->text().toInt();
    ui->weightInput->clear();

    if (u <= 0 || v <= 0) { // 非法输入
        ui->statusbar->showMessage(tr("非法操作：顶点名称应为大于 0 的整数"),2000);
        return;
    }

    auto adjList = graph.getAdjList();
    std::list<int>::iterator it = std::find(adjList[u].begin(), adjList[u].end(), v);
    if (it == adjList[u].end()) { // 已经没边了
        ui->statusbar->showMessage(tr("非法操作：不能删除不存在的边"),2000);
        return;
    }

    graph.removeEdge(u, v);
    updateAdjListDisplay();

    // 更新绘制
    update();
}

/* 输出邻接链表 */
void CircleWindow::updateAdjListDisplay()
{
    QString adjListText;
    auto adjList = graph.getAdjList();

    for (const auto &pair : adjList) {
        adjListText += QString::number(pair.first) + ": ";
        for (int vertex : pair.second)
            adjListText += QString::number(vertex) + " -> ";
        adjListText += "^\n";
    }

    ui->adjListDisplay->setText(adjListText);
}

/* 判断工程是否可行 */
void CircleWindow::judge_clicked()
{
    auto adjList = graph.getAdjList();
    if (adjList.empty()) { // 空图
        QMessageBox::critical(this, "非法操作", "该图是空图");
        return;
    }

    std::vector<int> result = {};
    bool isOK = graph.topologicalSort(result); // 进行拓扑排序
    if (isOK == false) { // 不可行
        QMessageBox::critical(this, "非法操作", "该图存在有向环，该工程不可行");
        return;
    }

    // 统计源点和汇点
    int sourceCount = 0;
    int sinkCount = 0;
    std::map<int, int> inDegree; // 入度
    std::map<int, int> outDegree; // 出度

    // 初始化入度和出度
    for (const auto& pair : adjList) {
        int u = pair.first;
        inDegree[u] = 0;
        outDegree[u] = 0;
    }
    for (const auto& pair : adjList) {
        int u = pair.first;
        for (int v : pair.second) {
            inDegree[v]++;
            outDegree[u]++;
        }
    }

    // 统计入度为 0 和出度为 0 的节点数量
    for (const auto& pair : adjList) {
        int u = pair.first;
        if (inDegree[u] == 0) sourceCount++;
        if (outDegree[u] == 0) sinkCount++;
    }

    // 判断工程是否只有一个源点和一个汇点
    if (sourceCount != 1 || sinkCount != 1) {
        QMessageBox::critical(this, "非法操作", "该工程没有唯一的源点和汇点，不可行");
        return;
    }

    // 如果工程可行且有唯一源点和汇点
    QMessageBox::information(this, "可行性判断", "该图不存在有向环，该工程可行，并且有唯一的源点和汇点");
    this->hide();
    t_win = new TimeWindow(graph, circles);
    // 设置 t_win 场景的位置
    t_win->setGeometry(this->geometry());
    t_win->setFixedSize(800, 800);
    t_win->show();
}
