// startscene.cpp
#include "startscene.h"
#include <QGraphicsProxyWidget>
#include <QFont>
#include <QPixmap>
#include <QGraphicsPixmapItem>

// 开始场景构造函数
StartScene::StartScene(QObject *parent)
    : QGraphicsScene(parent)
{
    // 设置场景大小为图片大小 (600x600)
    setSceneRect(0, 0, 600, 600);

    // 加载背景图片
    QPixmap background(":/game_objects/map_objects/start.jpg");
    if (!background.isNull()) {
        QGraphicsPixmapItem *bgItem = addPixmap(background);  // 添加背景图片
        bgItem->setPos(0, 0);  // 设置位置
    }

    // 添加开始按钮
    QPushButton *btn = new QPushButton("开始游戏");
    btn->setStyleSheet("QPushButton { font-size: 18px; padding: 10px; }");  // 按钮样式
    QGraphicsProxyWidget *proxy = addWidget(btn);  // 添加按钮到场景
    proxy->setPos(250, 200);  // 按钮位置

    // 添加玩法说明
    QGraphicsTextItem *desc = addText(
        "玩法说明：\n"
        "1. 方向键控制吃豆人移动\n"
        "2. 吃完所有豆子获胜，被鬼抓到失败\n"
        "3. 吃大能量豆时鬼进入惊慌状态\n"
        "4. 按P键可暂停/继续游戏",
        QFont("Arial", 16));  // 字体设置
    desc->setDefaultTextColor(Qt::white);  // 白色文字
    desc->setPos(140, 280);  // 位置

    // 连接按钮点击信号到开始游戏信号
    connect(btn, &QPushButton::clicked, this, &StartScene::startGame);
}
