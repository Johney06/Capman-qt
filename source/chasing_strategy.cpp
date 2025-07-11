// chasing_strategy.cpp
#include "game.h"

// 策略1：直接追逐Pacman
QPair<int, int> strategy1(Ghost* ghost) {
    Pacman *pacman = ghost->game->pacman;
    int dist_x = pacman->get_x() - ghost->get_x();  // X方向距离
    int dist_y = pacman->get_y() - ghost->get_y();  // Y方向距离
    return QPair<int, int>(dist_x, dist_y);
}

// 策略2：预测Pacman移动方向
QPair<int, int> strategy2(Ghost* ghost) {
    Pacman *pacman = ghost->game->pacman;
    int dist_x = pacman->get_x() - ghost->get_x();  // X方向距离
    int dist_y = pacman->get_y() - ghost->get_y();  // Y方向距离

    // 根据Pacman当前方向预测位置
    switch(pacman->get_dir()) {
    case GameObject::Up:
        dist_y -= 4;  // 向上预测4个单位
        break;
    case GameObject::Down:
        dist_y += 4;  // 向下预测4个单位
        break;
    case GameObject::Left:
        dist_x -= 4;  // 向左预测4个单位
        break;
    case GameObject::Right:
        dist_x += 4;  // 向右预测4个单位
        break;
    default:
        break;
    }
    return QPair<int, int>(dist_x, dist_y);
}

// 策略3：红鬼位置作为参考点
QPair<int, int> strategy3(Ghost* ghost) {
    Pacman *pacman = ghost->game->pacman;
    Ghost *red_ghost = ghost->game->ghost[Ghost::Red];
    // 计算基于红鬼位置的向量
    int dist_x = (pacman->get_x() - red_ghost->get_x()) * 2 - ghost->get_x();
    int dist_y = (pacman->get_y() - red_ghost->get_y()) * 2 - ghost->get_y();
    return QPair<int, int>(dist_x, dist_y);
}

// 策略4：近距离时逃跑
QPair<int, int> strategy4(Ghost* ghost) {
    Pacman *pacman = ghost->game->pacman;
    int dist_x = pacman->get_x() - ghost->get_x();  // X方向距离
    int dist_y = pacman->get_y() - ghost->get_y();  // Y方向距离

    // 当距离小于8个单位时逃跑
    if (dist_x * dist_x + dist_y * dist_y < 64) {
        dist_x = 2 - ghost->get_x();  // 逃向地图左上角(2,2)
        dist_y = 2 - ghost->get_y();
    }
    return QPair<int, int>(dist_x, dist_y);
}
