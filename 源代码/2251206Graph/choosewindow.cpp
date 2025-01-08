#include "choosewindow.h"
#include "ui_choosewindow.h"
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QPainter>

ChooseWindow::ChooseWindow(Graph g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChooseWindow)
{
    ui->setupUi(this);

    // 状态栏右侧信息
    QLabel * label = new QLabel("请在上方菜单栏中选择算法", this);
    ui->statusbar->addPermanentWidget(label);

    connect(ui->recreate, &QPushButton::clicked, this, [=](){
        emit this->recreate();
    });

    graph = g;
    setWindowTitle("菜单");

    // 信号槽连接 进行窗口的切换
    connect(ui->actionkruskal, &QAction::triggered, this, &ChooseWindow::startKruskal);
    connect(ui->actionprim, &QAction::triggered, this, &ChooseWindow::startPrim);
    connect(ui->actionone, &QAction::triggered, this, &ChooseWindow::startDijkstra);
    connect(ui->actionpair, &QAction::triggered, this, &ChooseWindow::startFloyd);
}

ChooseWindow::~ChooseWindow()
{
    delete ui;
}

/* 创建克鲁斯卡尔算法窗口 */
void ChooseWindow::startKruskal()
{
    this->hide();
    k_win = new KruskalWindow(graph);
    // 设置 k_win 场景的位置
    k_win->setGeometry(this->geometry());
    k_win->setFixedSize(700, 500);
    k_win->show();

    // 返回菜单窗口
    connect(k_win, &KruskalWindow::back_to_menu, this, [=](){
        this->setGeometry(k_win->geometry());
        this->show();
        delete k_win;
        k_win = NULL;
    });
}

/* 创建普里姆算法窗口 */
void ChooseWindow::startPrim()
{
    this->hide();

    // 确定 Prim 算法的初始顶点
    int result;
    while (true) {
        result = QInputDialog::getInt(this, "确定初始顶点", "请输入顶点编号");
        auto adjList = graph.getAdjList();
        if (adjList.find(result) != adjList.end())
            break;
        else
            QMessageBox::critical(this, "非法操作", "不存在该顶点");
    }

    p_win = new PrimWindow(graph, result);
    // 设置 p_win 场景的位置
    p_win->setGeometry(this->geometry());
    p_win->setFixedSize(700, 600);
    p_win->show();

    // 返回菜单窗口
    connect(p_win, &PrimWindow::back_to_menu, this, [=](){
        this->setGeometry(p_win->geometry());
        this->show();
        delete p_win;
        p_win = NULL;
    });
}

/* 创建迪杰斯特拉算法窗口 */
void ChooseWindow::startDijkstra()
{
    this->hide();
    // 设置场景的位置
    d_win = new DijkstraWindow(graph);
    d_win->setGeometry(this->geometry());
    d_win->setFixedSize(500, 400);
    d_win->show();

    // 返回菜单窗口
    connect(d_win, &DijkstraWindow::back_to_menu, this, [=](){
        this->setGeometry(d_win->geometry());
        this->show();
        delete d_win;
        d_win = NULL;
    });
}

/* 创建弗洛伊德算法窗口 */
void ChooseWindow::startFloyd()
{
    this->hide();

    // 设置场景的位置
    f_win = new FloydWindow(graph);
    f_win->setGeometry(this->geometry());
    f_win->setFixedSize(500, 400);
    f_win->show();

    // 返回菜单窗口
    connect(f_win, &FloydWindow::back_to_menu, this, [=](){
        this->setGeometry(f_win->geometry());
        this->show();
        delete f_win;
        f_win = NULL;
    });
}

/* 重写绘图事件 实现背景绘制 */
void ChooseWindow::paintEvent(QPaintEvent * ev)
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
