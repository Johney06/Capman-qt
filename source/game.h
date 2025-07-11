// game.h
#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QPair>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QGraphicsTextItem>
#include "gameobject.h"
#include <QAudioOutput>

// 游戏常量定义
#define BALL_SCORE      10      // 普通豆子分数
#define POWERBALL_SCORE 30      // 能量豆分数
#define GHOST_SCORE     50      // 鬼魂分数
#define INTERVAL        10      // Pacman移动时间间隔(ms)
#define NORMAL_INTERVAL 10      // 鬼魂正常状态移动间隔(ms)
#define PANNIC_INTERVAL 15      // 鬼魂惊慌状态移动间隔(ms)
#define RUNNING_INTERVAL 5      // 鬼魂逃跑状态移动间隔(ms)
#define PANNIC_TIME     1000    // 惊慌状态持续时间(ms)
#define FLASH_INTERVAL 200      // 能量豆闪烁间隔(ms)

/* 游戏主场景类 */
class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    // 游戏状态枚举
    enum GameStatus {
        Playing,  // 进行中
        Win,      // 胜利
        Lose,     // 失败
        Pause     // 暂停
    };

    // 构造函数
    Game(int x, int y, int map_w, int map_h, QString map_src);
    ~Game();  // 析构函数

    // 游戏控制方法
    void start();        // 开始游戏
    void stop();         // 停止游戏
    void togglePause();  // 切换暂停状态

    // Pacman控制
    void pacman_next_direction(GameObject::Dir d);  // 设置Pacman下一个方向

    int get_score();  // 获取当前分数

    // 游戏对象
    GameObject ***map;        // 地图二维数组
    GameObject *gate;         // 鬼门对象
    Pacman *pacman;           // Pacman对象
    Ghost *ghost[Ghost::GhostNum];  // 鬼魂数组
    QVector<GameObject*> powerball;  // 能量豆列表

    GameStatus stat;  // 当前游戏状态

    // 音效资源
    QSoundEffect *powerball_effect;  // 能量豆音效
    QMediaPlayer *bgm_player;        // 背景音乐播放器

    // 地图信息
    int map_height, map_width;  // 地图尺寸（格子数）
    int map_size;               // 地图总格子数
    int ball_num, eat_num, score;  // 豆子计数、已吃豆子数、分数
    int geo_x, geo_y;           // 场景在窗口中的位置

    // 定时器
    QTimer *ghost_timer[Ghost::GhostNum];  // 鬼魂移动定时器数组
    QTimer *pacman_timer;                  // Pacman移动定时器
    QTimer *powerball_flash_timer;         // 能量豆闪烁定时器

    QAudioOutput *bgm_audio;  // 背景音乐音频输出

protected:
    // 事件处理
    void keyPressEvent(QKeyEvent *event) override;  // 键盘事件处理

private slots:
    // 槽函数
    void pacman_handler();          // Pacman处理
    void powerball_flash();         // 能量豆闪烁
    void ghost_handler(int ghost_id); // 鬼魂处理

private:
    // 私有方法
    void updateScoreText();  // 更新分数显示

    // 私有变量
    bool flash_tick;    // 能量豆闪烁状态
    bool is_paused;     // 游戏是否暂停

    // 游戏状态文本
    QGraphicsTextItem *winText;   // 胜利文本
    QGraphicsTextItem *loseText;  // 失败文本
    QGraphicsTextItem *scoreText; // 分数文本
};

#endif // GAME_H
