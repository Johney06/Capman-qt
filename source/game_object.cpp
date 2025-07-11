// game_object.cpp
#include "game.h"

// 游戏对象构造函数
GameObject::GameObject(ObjectType t, QPixmap pixmap)
    : QGraphicsPixmapItem(pixmap)
{
    type = t;  // 设置对象类型
}

// 获取对象类型
GameObject::ObjectType GameObject::get_type()
{
    return type;
}

// 获取X坐标
int GameObject::get_x()
{
    return _x;
}

// 获取Y坐标
int GameObject::get_y()
{
    return _y;
}

// 获取分数值
int GameObject::get_score()
{
    return score;
}

// 设置分数值
void GameObject::set_score(int s)
{
    score = s;
}

// 设置当前方向
void GameObject::set_dir(Dir d)
{
    dir = d;
}

// 获取当前方向
GameObject::Dir GameObject::get_dir()
{
    return dir;
}

// 获取下一个方向
GameObject::Dir GameObject::get_next_dir()
{
    return next_dir;
}

// 设置下一个方向
void GameObject::set_next_dir(Dir d)
{
    next_dir = d;
}

// 析构函数
GameObject::~GameObject()
{
}
