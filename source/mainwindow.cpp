// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

// 主窗口构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // 初始化UI
    setWindowTitle(tr("pac-man"));  // 设置窗口标题

    /* 初始化图形视图和游戏场景 */
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");  // 无边框样式
    ui->graphicsView->setBackgroundBrush(Qt::black);  // 黑色背景
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);    // 无焦点策略

    int map_height = 20, map_width = 29;  // 地图尺寸20x29
    int x = 50, y = 50;                   // 在主窗口中的位置
    int w = (map_width * GameObject::Width);  // 计算宽度
    int h = (map_height * GameObject::Width); // 计算高度

    // 设置图形视图的位置和大小
    ui->graphicsView->setGeometry(x, y, w, h);

    // 创建游戏场景
    game = new Game(x, y, map_width, map_height, ":/game_objects/map_objects/map.txt");
    ui->graphicsView->setScene(game);  // 设置场景

    initLabels();  // 初始化UI标签
    game->start(); // 开始游戏
}


/* 初始化UI标签 */
void MainWindow::initLabels()
{
    // 分数标题标签
    score_title = new QLabel(this);
    score_title->setText("score");  // 文本
    score_title->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");  // 样式
    score_title->setGeometry(50, 12, 60, 26);  // 位置和大小

    // 分数显示标签
    score = new QLabel(this);
    score->setIndent(-80);  // 缩进
    score->setText("0");    // 初始文本
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: white;font-size: 16px;}");  // 样式
    score->setGeometry(110, 12, 150, 26);  // 位置和大小

    // 胜利标签
    win_label = new QLabel(this);
    win_label->hide();  // 初始隐藏
    win_label->setText("You win!");  // 文本
    win_label->setStyleSheet("QLabel {font-family: Fixedsys;color: yellow;font-size: 16px;}");  // 样式
    win_label->setGeometry(310, 12, 150, 26);  // 位置和大小

    // 失败标签
    lose_label = new QLabel(this);
    lose_label->hide();  // 初始隐藏
    lose_label->setText("You lose!");  // 文本
    lose_label->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 16px;}");  // 样式
    lose_label->setGeometry(310, 12, 150, 26);  // 位置和大小

    // 分数更新定时器
    score_timer = new QTimer(this);
    score_timer->start(25);  // 每25毫秒更新一次
    connect(score_timer, SIGNAL(timeout()), this , SLOT(update_score()));  // 连接信号槽
}


/* 更新分数显示 */
void MainWindow::update_score()
{
    score->setText(QString::number(game->get_score()));  // 设置分数文本

    // 根据游戏状态显示结果
    if (game->stat == Game::Win) {
        win_label->show();   // 显示胜利标签
        score_timer->stop(); // 停止更新
    } else if (game->stat == Game::Lose) {
        lose_label->show();  // 显示失败标签
        score_timer->stop();  // 停止更新
    }
}


// 键盘事件处理
void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W:
        game->pacman_next_direction(GameObject::Up);  // 向上移动
        break;
    case Qt::Key_A:
        game->pacman_next_direction(GameObject::Left);  // 向左移动
        break;
    case Qt::Key_S:
        game->pacman_next_direction(GameObject::Down);  // 向下移动
        break;
    case Qt::Key_D:
        game->pacman_next_direction(GameObject::Right);  // 向右移动
        break;
    }
}


// 主窗口析构函数
MainWindow::~MainWindow()
{
    delete ui;  // 删除UI对象
}
