// ghost.cpp
#include "game.h"
#include "chasing_strategy.cpp"
#include <QRandomGenerator>
#define W (GameObject::Width)  // 游戏对象宽度

// 鬼魂构造函数
Ghost::Ghost(int clr) : GameObject(GameObject::Ghost, QPixmap())
{
    color = (Color)clr;  // 设置鬼魂颜色
    anim_index = 0;      // 动画索引
    is_released = false; // 是否已释放
    dir = Dir(QRandomGenerator::global()->generate() % 4);  // 随机初始方向
    status = Normal;     // 初始状态为正常

    // 根据颜色加载不同的动画
    switch (color) {
    case Red:  // 红色鬼魂
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/redright1.png"));
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/redright2.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/redup1.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/redup2.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/redleft1.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/redleft2.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/reddown1.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/reddown2.png"));
        break;
    case Yellow:  // 黄色鬼魂
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/yellowright1.png"));
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/yellowright2.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/yellowup1.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/yellowup2.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/yellowleft1.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/yellowleft2.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/yellowdown1.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/yellowdown2.png"));
        break;
    case Green:  // 绿色鬼魂
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/greenright1.png"));
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/greenright2.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/greenup1.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/greenup2.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/greenleft1.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/greenleft2.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/greendown1.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/greendown2.png"));
        break;
    case Pink:  // 粉色鬼魂
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/pinkright1.png"));
        anim[Right].push_back(QPixmap(":/game_objects/ghosts/pinkright2.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/pinkup1.png"));
        anim[Up].push_back(QPixmap(":/game_objects/ghosts/pinkup2.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/pinkleft1.png"));
        anim[Left].push_back(QPixmap(":/game_objects/ghosts/pinkleft2.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/pinkdown1.png"));
        anim[Down].push_back(QPixmap(":/game_objects/ghosts/pinkdown2.png"));
        break;
    }

    // 加载惊慌状态和逃跑状态的动画
    panic_anim.push_back(QPixmap(":/game_objects/ghosts/bl1.png"));
    panic_anim.push_back(QPixmap(":/game_objects/ghosts/bl2.png"));
    running_anim.push_back(QPixmap(":/game_objects/ghosts/run.png"));
    setPixmap(anim[Right][0]);  // 设置初始图像
}

// 获取鬼魂颜色
Ghost::Color Ghost::get_color()
{
    return color;
}

// 向上移动
void Ghost::moveup()
{
    QVector<QPixmap> *ptr;  // 指向当前动画序列的指针

    // 根据状态选择动画
    switch (status) {
    case Normal:
        ptr = &anim[Up];  // 正常状态使用方向动画
        break;
    case Panic:
        ptr = &panic_anim;  // 惊慌状态使用惊慌动画
        break;
    case Running:
        ptr = &running_anim;  // 逃跑状态使用逃跑动画
        break;
    }

    // 更新动画帧
    anim_index++;
    if (anim_index >= ptr->size()) {
        anim_index = 0;  // 循环动画
    }
    setPixmap((*ptr)[anim_index]);  // 设置当前帧
    setY(static_cast<int>(y()) - 1);  // 向上移动一个像素
}

// 向左移动（类似moveup）
void Ghost::moveleft()
{
    QVector<QPixmap> *ptr;
    switch (status) {
    case Normal:
        ptr = &anim[Left];
        break;
    case Panic:
        ptr = &panic_anim;
        break;
    case Running:
        ptr = &running_anim;
        break;
    }

    anim_index++;
    if (anim_index >= ptr->size()) {
        anim_index = 0;
    }
    setPixmap((*ptr)[anim_index]);
    setX(static_cast<int>(x()) - 1);
}

// 向下移动（类似moveup）
void Ghost::movedown()
{
    QVector<QPixmap> *ptr;
    switch (status) {
    case Normal:
        ptr = &anim[Down];
        break;
    case Panic:
        ptr = &panic_anim;
        break;
    case Running:
        ptr = &running_anim;
        break;
    }

    anim_index++;
    if (anim_index >= ptr->size()) {
        anim_index = 0;
    }
    setPixmap((*ptr)[anim_index]);
    setY(static_cast<int>(y()) + 1);
}

// 向右移动（类似moveup）
void Ghost::moveright()
{
    QVector<QPixmap> *ptr;
    switch (status) {
    case Normal:
        ptr = &anim[Right];
        break;
    case Panic:
        ptr = &panic_anim;
        break;
    case Running:
        ptr = &running_anim;
        break;
    }

    anim_index++;
    if (anim_index >= ptr->size()) {
        anim_index = 0;
    }
    setPixmap((*ptr)[anim_index]);
    setX(static_cast<int>(x()) + 1);
}

// 检查是否可以移动到指定位置
bool Ghost::overlapable(int i, int j)
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
    case Wall:  // 墙壁不可移动
        return false;
    case Gate:  // 鬼门特殊处理
        if (is_released == false && release_time == 0) {
            return true;  // 未释放且到释放时间可通过
        } else if (status == Running) {
            return true;  // 逃跑状态可通过
        } else {
            return false; // 其他情况不可通过
        }
    default:  // 其他类型可通过
        return true;
    }
}

// 随机设置方向
void Ghost::setdir_randomly()
{
    QVector<Dir> oklist;  // 可行方向列表

    // 检查四个方向是否可行
    if (overlapable(_y, _x + 1)) {
        oklist.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        oklist.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        oklist.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        oklist.push_back(Up);
    }

    // 随机选择一个可行方向
    dir = oklist.at(QRandomGenerator::global()->generate() % oklist.size());
}

// 离开笼子
void Ghost::go_out_cage()
{
    // 计算到鬼门的距离
    int x_dist_to_gate = game->gate->_x - _x;
    int y_dist_to_gate = game->gate->_y - _y;

    // 根据距离设置方向
    if (x_dist_to_gate > 0) {
        set_dir(GameObject::Right);  // 向右移动
    } else if (x_dist_to_gate < 0) {
        set_dir(GameObject::Left);   // 向左移动
    } else {
        if (y_dist_to_gate > 0) {
            set_dir(GameObject::Down);  // 向下移动
        } else {
            set_dir(GameObject::Up);    // 向上移动
        }
    }
}

// 追逐Pacman
void Ghost::chase_pacman()
{
    // 初始化可行方向数组
    bool okdir[5] = {false, false, false, false, false};
    QVector<Dir> oklist;

    // 检查四个方向是否可行
    if (overlapable(_y, _x + 1)) {
        okdir[Right] = true;
        oklist.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        okdir[Left] = true;
        oklist.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        okdir[Down] = true;
        oklist.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        okdir[Up] = true;
        oklist.push_back(Up);
    }

    // 确定反方向（避免180度转向）
    Dir backward_dir;
    switch (dir) {
    case Up:
        backward_dir = Down;
        break;
    case Down:
        backward_dir = Up;
        break;
    case Left:
        backward_dir = Right;
        break;
    case Right:
        backward_dir = Left;
        break;
    case Stop:
        backward_dir = Stop;
        break;
    }

    // 在交叉路口改变方向（避免振荡）
    if (oklist.size() > 2) {
        // 获取追逐策略向量
        QPair<int, int> vector = chase_strategy(this);
        int dist_x = vector.first;   // X方向距离
        int dist_y = vector.second;  // Y方向距离

        // 根据向量选择最佳方向
        if (dist_y > 0) {
            if (okdir[Down] && dir != Up) {
                dir = Down;  // 优先向下
            } else {
                // 其次选择左右方向
                if (dist_x >= 0) {
                    if (okdir[Right]) dir = Right;
                    else if (okdir[Left]) dir = Left;
                } else {
                    if (okdir[Left]) dir = Left;
                    else if (okdir[Right]) dir = Right;
                }
            }
        } else if (dist_y < 0) {
            if (okdir[Up] && dir != Down) {
                dir = Up;  // 优先向上
            } else {
                if (dist_x >= 0) {
                    if (okdir[Right]) dir = Right;
                    else if (okdir[Left]) dir = Left;
                } else {
                    if (okdir[Left]) dir = Left;
                    else if (okdir[Right]) dir = Right;
                }
            }
        } else if (dist_x > 0) {
            if (okdir[Right] && dir != Left) {
                dir = Right;  // 优先向右
            } else {
                if (okdir[Up]) dir = Up;
                else if(okdir[Down]) dir = Down;
            }
        } else if (dist_x < 0) {
            if (okdir[Left] && dir != Right) {
                dir = Left;  // 优先向左
            } else {
                if (okdir[Up]) dir = Up;
                else if(okdir[Down]) dir = Down;
            }
        }
    } else if (oklist.size() == 2) {
        // 在角落处理
        if (okdir[dir] == false) {
            if (oklist[0] == backward_dir) {
                dir = oklist[1];  // 避免走回头路
            } else {
                dir = oklist[0];  // 选择另一个方向
            }
        }
    }
}

// 躲避Pacman（类似chase_pacman，但方向相反）
void Ghost::dodge_pacman()
{
    bool okdir[5] = {false, false, false, false, false};
    QVector<Dir> oklist;
    if (overlapable(_y, _x + 1)) {
        okdir[Right] = true;
        oklist.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        okdir[Left] = true;
        oklist.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        okdir[Down] = true;
        oklist.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        okdir[Up] = true;
        oklist.push_back(Up);
    }

    Dir backward_dir;
    switch (dir) {
    case Up:
        backward_dir = Down;
        break;
    case Down:
        backward_dir = Up;
        break;
    case Left:
        backward_dir = Right;
        break;
    case Right:
        backward_dir = Left;
        break;
    case Stop:
        backward_dir = Stop;
        break;
    }

    if (oklist.size() > 2) {
        int dist_x = game->pacman->get_x() - _x;
        int dist_y = game->pacman->get_y() - _y;
        // 躲避逻辑：选择远离Pacman的方向
        if (dist_y < 0) {
            if (okdir[Down] && dir != Up) {
                dir = Down;  // 远离向上移动的Pacman
            } else {
                if (dist_x >= 0) {
                    if (okdir[Left]) dir = Left;
                    else if (okdir[Right]) dir = Right;
                } else {
                    if (okdir[Right]) dir = Right;
                    else if (okdir[Left]) dir = Left;
                }
            }
        } else if (dist_y > 0) {
            if (okdir[Up] && dir != Down) {
                dir = Up;  // 远离向下移动的Pacman
            } else {
                if (dist_x >= 0) {
                    if (okdir[Left]) dir = Left;
                    else if (okdir[Right]) dir = Right;
                } else {
                    if (okdir[Right]) dir = Right;
                    else if (okdir[Left]) dir = Left;
                }
            }
        } else if (dist_x < 0) {
            if(okdir[Right] && dir != Left) {
                dir = Right;  // 远离向左移动的Pacman
            } else {
                if (okdir[Up]) dir = Up;
                else if(okdir[Down]) dir = Down;
            }
        } else if (dist_x > 0) {
            if(okdir[Left] && dir != Right) {
                dir = Left;  // 远离向右移动的Pacman
            } else {
                if (okdir[Up]) dir = Up;
                else if(okdir[Down]) dir = Down;
            }
        }
    } else if (oklist.size() == 2) {
        if (okdir[dir] == false) {
            if (oklist[0] == backward_dir) {
                dir = oklist[1];
            } else {
                dir = oklist[0];
            }
        }
    }
}

// 返回笼子
void Ghost::go_to_cage()
{
    bool okdir[5] = {false, false, false, false, false};
    QVector<Dir> oklist;
    if (overlapable(_y, _x + 1)) {
        okdir[Right] = true;
        oklist.push_back(Right);
    }
    if (overlapable(_y, _x - 1)) {
        okdir[Left] = true;
        oklist.push_back(Left);
    }
    if (overlapable(_y + 1, _x)) {
        okdir[Down] = true;
        oklist.push_back(Down);
    }
    if (overlapable(_y - 1, _x)) {
        okdir[Up] = true;
        oklist.push_back(Up);
    }

    Dir backward_dir;
    switch (dir) {
    case Up:
        backward_dir = Down;
        break;
    case Down:
        backward_dir = Up;
        break;
    case Left:
        backward_dir = Right;
        break;
    case Right:
        backward_dir = Left;
        break;
    case Stop:
        backward_dir = Stop;
        break;
    }

    if (oklist.size() > 2) {
        // 计算到鬼门的距离
        int dist_x = game->gate->get_x() - _x;
        int dist_y = (game->gate->get_y() + 1) - _y;

        // 根据距离选择方向
        if (dist_y > 0) {
            if (okdir[Down] && dir != Up) {
                dir = Down;  // 向下移动
            } else {
                if (dist_x >= 0) {
                    if (okdir[Right]) dir = Right;
                    else if (okdir[Left]) dir = Left;
                } else {
                    if (okdir[Left]) dir = Left;
                    else if (okdir[Right]) dir = Right;
                }
            }
        } else if (dist_y < 0) {
            if (okdir[Up] && dir != Down) {
                dir = Up;  // 向上移动
            } else {
                if (dist_x >= 0) {
                    if (okdir[Right]) dir = Right;
                    else if (okdir[Left]) dir = Left;
                } else {
                    if (okdir[Left]) dir = Left;
                    else if (okdir[Right]) dir = Right;
                }
            }
        } else if (dist_x > 0) {
            if(okdir[Right] && dir != Left) {
                dir = Right;  // 向右移动
            } else {
                if (okdir[Up]) dir = Up;
                else if (okdir[Down]) dir = Down;
            }
        } else if (dist_x < 0) {
            if(okdir[Left] && dir != Right) {
                dir = Left;  // 向左移动
            } else {
                if (okdir[Up]) dir = Up;
                else if (okdir[Down]) dir = Down;
            }
        }
    } else if (oklist.size() == 2) {
        if (okdir[dir] == false) {
            if (oklist[0] == backward_dir) {
                dir = oklist[1];
            } else {
                dir = oklist[0];
            }
        }
    }
}

// 鬼魂移动主函数
void Ghost::move()
{
    // 处理状态计时器
    if (status != Running && release_time > 0) {
        release_time--;  // 减少释放时间
    } else if (status == Panic) {
        panic_time--;  // 减少惊慌时间
        if (panic_time <= 0) {
            status = Normal;  // 恢复正常状态
            game->ghost_timer[color]->setInterval(NORMAL_INTERVAL);  // 恢复正常移动速度
        }
    }

    // 计算鬼魂在地图中的位置
    int gh_x = static_cast<int>(x());
    int gh_y = static_cast<int>(y());
    int __x = (gh_x - game->geo_x) / W;           // 地图X坐标
    int __y = (gh_y - game->geo_y) / W;           // 地图Y坐标
    int x_remainder = (gh_x - game->geo_x) % W;   // X方向像素偏移
    int y_remainder = (gh_y - game->geo_y) % W;   // Y方向像素偏移

    // 当鬼魂完全对齐地图格子时决定方向
    if (x_remainder == 0 && y_remainder == 0) {
        // 更新鬼魂地图坐标
        _x = __x;
        _y = __y;

        if (is_released) {
            // 已释放状态：根据状态选择行为
            switch (status) {
            case Normal:
                chase_pacman();  // 正常状态追逐Pacman
                break;
            case Panic:
                dodge_pacman();  // 惊慌状态躲避Pacman
                break;
            default:
                break;
            }
        } else {
            // 未释放状态
            if (release_time == 0) {
                // 到释放时间：离开笼子
                go_out_cage();
                if (game->map[_y][_x]->get_type() == Gate) {
                    // 到达鬼门：标记为已释放
                    is_released = true;
                }
            } else {
                if (status == Running) {
                    // 逃跑状态：返回笼子
                    if (_x == game->gate->get_x() && _y == game->gate->get_y() + 1) {
                        // 到达笼子：恢复正常状态
                        status = Normal;
                        game->ghost_timer[color]->setInterval(NORMAL_INTERVAL);
                    } else {
                        // 继续返回笼子
                        go_to_cage();
                    }
                } else {
                    // 在笼子内：随机移动
                    setdir_randomly();
                }
            }
        }
    }

    // 根据方向移动
    switch (dir) {
    case Stop:
        break;
    case Up:
        moveup();
        break;
    case Down:
        movedown();
        break;
    case Left:
        moveleft();
        break;
    case Right:
        moveright();
        break;
    }

    // 检测是否碰到Pacman
    if (collidesWithItem(game->pacman, Qt::IntersectsItemShape)){
        if (status == Normal) {
            // 正常状态碰到Pacman：游戏失败
            game->stat = Game::Lose;
        } else if (status == Panic) {
            // 惊慌状态碰到Pacman：进入逃跑状态
            setPixmap(QPixmap(":/rsc2/run.png"));
            game->score += score;  // 增加分数
            status = Running;      // 进入逃跑状态
            release_time = 200;    // 设置返回笼子时间
            is_released = false;   // 标记为未释放
            game->ghost_timer[color]->setInterval(RUNNING_INTERVAL);  // 设置逃跑速度
        }
    }
}
