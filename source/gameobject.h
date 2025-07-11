// game_object.h
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QVector>

// 前置声明游戏类
class Game;

/* 游戏对象基类（鬼魂、Pacman等） */
class GameObject : public QGraphicsPixmapItem
{
public:
    // 对象类型枚举
    enum ObjectType {
        Ball,       // 豆子
        Wall,       // 墙壁
        Gate,       // 鬼门
        Pacman,     // Pacman
        Ghost,      // 鬼魂
        Blank,      // 空白区域
        PowerBall   // 能量豆
    };

    // 方向枚举
    enum Dir {
        Up = 0,     // 上
        Down = 1,   // 下
        Left = 2,   // 左
        Right = 3,  // 右
        Stop = 4    // 停止
    };

    static const int Width = 20;  // 游戏对象宽度（像素）

    // 构造函数
    GameObject(ObjectType t, QPixmap pixmap);
    ~GameObject();  // 析构函数

    // 基本访问方法
    ObjectType get_type();  // 获取对象类型
    int get_x();            // 获取地图X坐标
    int get_y();            // 获取地图Y坐标
    int get_score();        // 获取分数值
    void set_score(int s);  // 设置分数值

    // 方向控制方法
    void set_dir(Dir d);      // 设置当前方向
    void set_next_dir(Dir d); // 设置下一个方向
    Dir get_dir();          // 获取当前方向
    Dir get_next_dir();     // 获取下一个方向

    // 友元类
    friend class Game;
    friend class Ghost;

protected:
    int _x, _y;             // 地图坐标（网格位置）
    Dir dir;                // 当前移动方向
    Dir next_dir;           // 下一个方向（由键盘输入决定）
    ObjectType type;        // 对象类型
    int score;              // 分数值
};

/* Pacman类 */
class Pacman : public GameObject
{
public:
    Pacman();  // 构造函数
    void move();  // 移动函数
    Game *game;   // 指向游戏对象的指针

    friend class Game;

private:
    // 移动辅助函数
    void moveup();     // 向上移动
    void movedown();   // 向下移动
    void moveleft();   // 向左移动
    void moveright();  // 向右移动

    // 游戏逻辑函数
    void eat_ball(int y, int x);  // 吃豆子
    bool overlapable(int y, int x); // 检查是否可移动到指定位置

    QVector<QPixmap> anim[4]; // 四个方向的动画序列
    int anim_index;           // 当前动画帧索引
};

/* 鬼魂类 */
class Ghost : public GameObject
{
public:
    // 颜色枚举
    enum Color {
        Red = 0,     // 红色鬼魂
        Yellow = 1,  // 黄色鬼魂
        Pink = 2,    // 粉色鬼魂
        Green = 3    // 绿色鬼魂
    };

    // 状态枚举
    enum Status {
        Normal,  // 正常状态（追逐Pacman）
        Panic,   // 惊慌状态（被Pacman追逐）
        Running  // 逃跑状态（返回笼子）
    };

    const static int GhostNum = 4;  // 鬼魂数量

    Game *game;  // 指向游戏对象的指针

    Ghost(int color);  // 构造函数（参数为颜色）
    void move();       // 移动函数
    Color get_color(); // 获取鬼魂颜色

    friend class Game;
    friend class Pacman;

private:
    // 移动辅助函数
    void moveup();     // 向上移动
    void movedown();   // 向下移动
    void moveleft();   // 向左移动
    void moveright();  // 向右移动

    // AI行为函数
    void setdir_randomly();  // 随机设置方向
    void go_out_cage();      // 离开笼子
    void chase_pacman();     // 追逐Pacman
    void dodge_pacman();     // 躲避Pacman
    void go_to_cage();       // 返回笼子

    QPair<int, int> (*chase_strategy)(Ghost*);  // 追逐策略函数指针
    bool overlapable(int y, int x);  // 检查是否可移动到指定位置

    Color color;         // 鬼魂颜色
    Status status;       // 当前状态

    // 动画资源
    QVector<QPixmap> anim[4];     // 四个方向的动画序列
    QVector<QPixmap> panic_anim;  // 惊慌状态动画
    QVector<QPixmap> running_anim;// 逃跑状态动画

    int anim_index;      // 当前动画帧索引
    int release_time;    // 释放时间（倒计时）
    bool is_released;    // 是否已释放出笼子
    int panic_time;      // 惊慌状态剩余时间
};

/* 鬼魂追逐策略函数声明 */
QPair<int, int> strategy1(Ghost* ghost);  // 策略1：直接追逐Pacman当前位置
QPair<int, int> strategy2(Ghost* ghost);  // 策略2：预测Pacman移动方向
QPair<int, int> strategy3(Ghost* ghost);  // 策略3：基于红鬼位置计算目标
QPair<int, int> strategy4(Ghost* ghost);  // 策略4：近距离时逃跑

#endif // GAMEOBJECT_H
