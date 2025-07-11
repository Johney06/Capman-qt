// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
#include <QTimer>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/* 主窗口类 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 构造函数
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();  // 析构函数

    // 初始化方法
    void initLabels();  // 初始化UI标签

    // 事件处理
    void keyPressEvent(QKeyEvent* e) override;  // 键盘事件处理

private slots:
    // 槽函数
    void update_score();  // 更新分数显示

private:
    Ui::MainWindow *ui;  // UI对象

    // UI元素
    QLabel *score_title;  // 分数标题标签
    QLabel *score;        // 分数显示标签
    QLabel *win_label;    // 胜利标签
    QLabel *lose_label;   // 失败标签

    QTimer *score_timer;  // 分数更新定时器
    Game *game;           // 游戏场景
};
#endif // MAINWINDOW_H
