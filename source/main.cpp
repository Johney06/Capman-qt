// main.cpp
#include <QApplication>
#include <QGraphicsView>
#include "startscene.h"
#include "game.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  // 创建Qt应用

    QGraphicsView view;  // 创建视图窗口
    view.setWindowTitle("PAC MAN");  // 设置窗口标题
    view.setFixedSize(600, 620);     // 设置固定窗口大小

    // 创建开始场景
    StartScene *startScene = new StartScene;
    Game *gameScene = nullptr;  // 游戏场景初始为空

    // 设置初始场景为开始场景
    view.setScene(startScene);
    view.show();  // 显示窗口

    // 连接开始游戏的信号
    QObject::connect(startScene, &StartScene::startGame, [&]() {
        // 如果已有游戏场景，先删除
        if (gameScene){
            delete gameScene;
            gameScene = nullptr;
        }
        // 创建新游戏场景（0,0位置，29x20地图，使用指定地图文件）
        gameScene = new Game(0, 0, 29, 20, ":/game_objects/map_objects/map.txt");
        view.setScene(gameScene);  // 设置新场景
        gameScene->start();        // 开始游戏
    });

    return app.exec();  // 运行应用
}
