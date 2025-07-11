// startscene.h
#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QObject>

/* 开始场景类 */
class StartScene : public QGraphicsScene {
    Q_OBJECT
public:
    // 构造函数
    StartScene(QObject *parent = nullptr);

signals:
    // 信号
    void startGame();  // 开始游戏信号
};

#endif // STARTSCENE_H
