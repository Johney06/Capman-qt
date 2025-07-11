// pacman.cpp
#include "game.h"
#define W (GameObject::Width)  // 游戏对象宽度

// Pacman构造函数
Pacman::Pacman() : GameObject(GameObject::Pacman, QPixmap(":/game_objects/pacman/a3.png"))
{
    dir = Stop;         // 初始方向为停止
    next_dir = Stop;    // 初始下一个方向为停止
    anim_index = 2;     // 动画索引

    // 加载向右动画
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a2.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a3.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a4.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a5.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a6.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a5.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a4.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a3.png"));
    anim[Right].push_back(QPixmap(":/game_objects/pacman/a2.png"));

    // 加载向上动画
    anim[Up].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b2.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b3.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b4.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b5.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b6.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b5.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b4.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b3.png"));
    anim[Up].push_back(QPixmap(":/game_objects/pacman/b2.png"));

    // 加载向左动画
    anim[Left].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c2.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c3.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c4.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c5.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c6.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c5.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c4.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c3.png"));
    anim[Left].push_back(QPixmap(":/game_objects/pacman/c2.png"));

    // 加载向下动画
    anim[Down].push_back(QPixmap(":/game_objects/pacman/a1.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d2.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d3.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d4.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d5.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d6.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d5.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d4.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d3.png"));
    anim[Down].push_back(QPixmap(":/game_objects/pacman/d2.png"));
}

// 向上移动
void Pacman::moveup()
{
    // 更新动画帧
    anim_index++;
    if (anim_index == anim[Up].size()) {
        anim_index = 0;  // 动画循环
    }
    setPixmap(anim[Up][anim_index]);  // 设置当前帧
    setY(static_cast<int>(y()) - 1);  // 向上移动一个像素
}

// 向左移动（类似moveup）
void Pacman::moveleft()
{
    anim_index++;
    if (anim_index == anim[Left].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Left][anim_index]);
    setX(static_cast<int>(x()) - 1);
}

// 向下移动（类似moveup）
void Pacman::movedown()
{
    anim_index++;
    if (anim_index == anim[Down].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Down][anim_index]);
    setY(static_cast<int>(y()) + 1);
}

// 向右移动（类似moveup）
void Pacman::moveright()
{
    anim_index++;
    if (anim_index == anim[Right].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Right][anim_index]);
    setX(static_cast<int>(x()) + 1);
}

// 吃豆子
void Pacman::eat_ball(int __y, int __x)
{
    GameObject *obj = game->map[__y][__x];  // 获取目标位置对象

    switch (obj->get_type()) {
    case Ball:  // 普通豆子
        game->score += obj->get_score();  // 增加分数
        game->ball_num--;  // 减少豆子计数
        break;
    case PowerBall:  // 能量豆
        game->score += obj->get_score();  // 增加分数
        game->ball_num--;  // 减少豆子计数

        // 从能量豆列表中移除
        for (int i = 0; i < game->powerball.size(); i++) {
            if (game->powerball.at(i) == obj) {
                game->powerball.remove(i);
                break;
            }
        }

        // 播放能量豆音效
        if (game->powerball_effect)
            game->powerball_effect->play();

        // 设置所有鬼魂为惊慌状态
        for (int i = 0; i < Ghost::GhostNum; i++) {
            if (game->ghost[i]->status != Ghost::Running) {
                game->ghost[i]->status = Ghost::Panic;  // 设置惊慌状态
                game->ghost[i]->panic_time = PANNIC_TIME;  // 设置惊慌时间
                game->ghost_timer[i]->setInterval(PANNIC_INTERVAL);  // 设置移动速度
            }
        }
        break;
    default:  // 其他类型不处理
        return;
    }

    /* Pacman吃掉豆子后，
     * 将当前位置设为空白 */
    QPixmap blankpix;
    game->map[_y][_x] = new GameObject(GameObject::Blank, blankpix);  // 原位置设为空白
    game->map[__y][__x] = this;  // 新位置设置为Pacman
    delete obj;  // 删除豆子对象
}

// 检查是否可以移动到指定位置
bool Pacman::overlapable(int i, int j)
{
    // 检查边界
    if (i < 0 || j < 0) {
        return false;
    }
    if (i >= game->map_height || j >= game->map_width) {
        return false;
    }

    // 根据地图对象类型判断是否可移动
    switch (game->map[i][j]->get_type()) {
    case Wall:    // 墙壁不可移动
    case Gate:    // 鬼门不可移动
        return false;
    default:      // 其他类型可移动
        return true;
    }
}

// Pacman移动主函数
void Pacman::move()
{
    // 计算Pacman在地图中的位置
    int pac_x = static_cast<int>(x());
    int pac_y = static_cast<int>(y());
    int __x = (pac_x - game->geo_x) / W;            // 地图X坐标
    int __y = (pac_y - game->geo_y) / W;            // 地图Y坐标
    int x_remainder = (pac_x - game->geo_x) % W;    // X方向像素偏移
    int y_remainder = (pac_y - game->geo_y) % W;    // Y方向像素偏移
    Dir next_dir = get_next_dir();                  // 获取下一个方向

    /* 当Pacman完全对齐地图格子时，
     * 决定是否改变方向 */
    if (x_remainder == 0) {
        if (y_remainder == 0) {
            // 当Pacman完全对齐格子时，
            // 检查该位置是否有豆子可吃
            eat_ball(__y, __x);

            // 更新Pacman地图坐标
            _x = __x;
            _y = __y;

            // 如果所有豆子都被吃完，游戏胜利
            if (game->ball_num == 0) {
                game->stat = Game::Win;
                return;
            }
        }

        // 检查是否可以改变方向
        switch (next_dir) {
        case Stop:  // 停止
            dir = next_dir;
            break;
        case Up:    // 尝试向上转
            if (overlapable(_y - 1, _x)) {
                dir = next_dir;
            }
            break;
        case Down:  // 尝试向下转
            if (overlapable(_y + 1, _x)) {
                dir = next_dir;
            }
            break;
        case Left:  // 尝试向左转
            if (y_remainder == 0 && overlapable(_y, _x - 1)) {
                dir = next_dir;
            }
            break;
        case Right: // 尝试向右转
            if (y_remainder == 0 && overlapable(_y, _x + 1)) {
                dir = next_dir;
            }
            break;
        }
    } else if (y_remainder == 0) {
        switch (next_dir) {
        case Stop:
            dir = next_dir;
            break;
        case Left:  // 尝试向左转
            if (overlapable(_y, _x - 1)) {
                dir = next_dir;
            }
            break;
        case Right: // 尝试向右转
            if (overlapable(_y, _x + 1)) {
                dir = next_dir;
            }
            break;
        default:
            break;
        }
    }

    /* Pacman保持移动或在碰到墙壁时停止 */
    switch (dir) {
    case Stop:  // 停止状态
        break;
    case Up:    // 向上移动
        if (y_remainder == 0 && !overlapable(_y - 1, _x)) {
            // 碰到墙壁：停止
            dir = Stop;
            next_dir = Stop;
        } else {
            moveup();  // 可以移动
        }
        break;
    case Down:  // 向下移动
        if (y_remainder == 0 && !overlapable(_y + 1, _x)) {
            // 碰到墙壁：停止
            dir = Stop;
            next_dir = Stop;
        } else {
            movedown();  // 可以移动
        }
        break;
    case Left:  // 向左移动
        if (x_remainder == 0 && !overlapable(_y, _x - 1)) {
            // 碰到墙壁：停止
            dir = Stop;
            next_dir = Stop;
        } else {
            moveleft();  // 可以移动
        }
        break;
    case Right: // 向右移动
        if (x_remainder == 0 && !overlapable(_y, _x + 1)) {
            // 碰到墙壁：停止
            dir = Stop;
            next_dir = Stop;
        } else {
            moveright();  // 可以移动
        }
        break;
    }
}
