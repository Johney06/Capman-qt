// game.cpp
#include "game.h"
#include <QString>
#include <QFile>
#include <QRandomGenerator>
#include <QKeyEvent>
#define W (GameObject::Width)  // 游戏对象宽度

// 鬼魂释放时间
int GHOST_RELEASE_TIME[] = {0, 200, 400, 600};

// 游戏场景构造函数
Game::Game(int x, int y, int map_w, int map_h, QString map_src)
    : QGraphicsScene(x, y, W * map_w, W * map_h)
{
    geo_x = x;  // 场景X坐标
    geo_y = y;  // 场景Y坐标
    stat = Playing;  // 初始游戏状态为进行中
    is_paused = false;  // 暂停状态

    map_size = map_w * map_h;  // 地图大小
    map_width = map_w;         // 地图宽度
    map_height = map_h;        // 地图高度

    // 初始化地图二维数组
    map = new GameObject**[map_height];
    for (int i = 0; i < map_height; i++) {
        map[i] = new GameObject*[map_width];
        for (int j = 0; j < map_width; j++)
            map[i][j] = nullptr;  // 初始化为空指针
    }

    ball_num = eat_num = score = 0;  // 初始化计数器和分数
    int ghost_i = 0;  // 鬼魂索引

    // 加载各种游戏对象的图像
    QPixmap wallpix(":/game_objects/map_objects/wall.png");
    QPixmap ballpix(":/game_objects/map_objects/dot.png");
    QPixmap powerballpix(":/game_objects/map_objects/power_ball.png");
    QPixmap gatepix(":/game_objects/map_objects/gate.png");
    QPixmap blankpix;  // 空白区域

    // 打开地图文件
    QFile mapfile(map_src);
    mapfile.open(QIODevice::ReadOnly|QIODevice::Text);

    pacman = new Pacman();  // 创建Pacman对象

    // 解析地图文件
    for (int i = 0; i < map_h; i++) {
        QByteArray line = mapfile.readLine().trimmed();
        // 处理行长度不正确的情况
        if (line.length() < map_w) {
            qWarning() << "地图行" << i << "太短，用空白填充";
            line = line.leftJustified(map_w, '3'); // 用空白填充
        }
        else if (line.length() > map_w) {
            line = line.left(map_w); // 截断过长行
        }

        // 解析每个地图格子
        for (int j = 0; j < map_w; j++) {
            int tmp_x = x + (j * W);  // 计算像素位置X
            int tmp_y = y + (i * W);  // 计算像素位置Y

            switch (line[j]) {
            case '1':  // 墙
                map[i][j] = new GameObject(GameObject::Wall, wallpix);
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                break;
            case '0':  // 豆子
                map[i][j] = new GameObject(GameObject::Ball, ballpix);
                map[i][j]->set_score(BALL_SCORE);  // 设置分数值
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                ball_num++;  // 增加豆子计数
                break;
            case '4':  // 能量豆
                map[i][j] = new GameObject(GameObject::PowerBall, powerballpix);
                map[i][j]->set_score(POWERBALL_SCORE);  // 设置分数值
                map[i][j]->setPos(tmp_x, tmp_y);
                addItem(map[i][j]);
                powerball.push_back(map[i][j]);  // 添加到能量豆列表
                ball_num++;  // 增加豆子计数
                break;
            case '3':  // 空白区域
                map[i][j] = new GameObject(GameObject::Blank, blankpix);
                break;
            case '2':  // 鬼门
                gate = new GameObject(GameObject::Gate, gatepix);
                gate->_x = j;
                gate->_y = i;
                gate->setPos(tmp_x, tmp_y);
                addItem(gate);
                map[i][j] = gate;
                break;
            case 'p':  // Pacman
                pacman = new Pacman();
                pacman->game = this;  // 设置游戏指针
                pacman->setZValue(2);  // 设置Z轴值（显示层级）
                pacman->setPos(tmp_x, tmp_y);
                addItem(pacman);
                map[i][j] = pacman;
                break;
            case 'g':  // 鬼魂
                map[i][j] = new GameObject(GameObject::Blank, blankpix);
                ghost[ghost_i] = new Ghost(ghost_i);  // 创建鬼魂
                ghost[ghost_i]->game = this;  // 设置游戏指针
                ghost[ghost_i]->setZValue(2);  // 设置Z轴值
                ghost[ghost_i]->release_time = GHOST_RELEASE_TIME[ghost_i];  // 设置释放时间
                ghost[ghost_i]->_x = j;  // 设置地图坐标X
                ghost[ghost_i]->_y = i;  // 设置地图坐标Y
                ghost[ghost_i]->set_score(GHOST_SCORE);  // 设置分数值
                ghost[ghost_i]->setPos(tmp_x, tmp_y);  // 设置像素位置
                addItem(ghost[ghost_i]);  // 添加到场景
                ghost_i++;  // 下一个鬼魂
                break;
            }
            if (map[i][j]) {
                map[i][j]->_x = j;  // 记录地图坐标X
                map[i][j]->_y = i;  // 记录地图坐标Y
            }
        }
    }

    // 设置鬼魂的追逐策略
    ghost[Ghost::Red]->chase_strategy = &strategy1;
    ghost[Ghost::Pink]->chase_strategy = &strategy2;
    ghost[Ghost::Green]->chase_strategy = &strategy3;
    ghost[Ghost::Yellow]->chase_strategy = &strategy4;

    // 分数显示
    scoreText = new QGraphicsTextItem;
    scoreText->setDefaultTextColor(Qt::yellow);  // 黄色文字
    scoreText->setFont(QFont("Arial", 18, QFont::Bold));  // 字体设置
    scoreText->setPos(10, 0);  // 位置
    addItem(scoreText);  // 添加到场景
    updateScoreText();  // 更新分数显示

    // 背景音乐
    bgm_player = new QMediaPlayer(this);
    bgm_audio = new QAudioOutput(this);
    bgm_player->setAudioOutput(bgm_audio);
    bgm_player->setSource(QUrl("qrc:/source/sound/bgm.mp3"));  // 背景音乐文件
    bgm_audio->setVolume(0.5f);  // 音量

    // 背景音乐循环播放
    connect(bgm_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status){
        if (status == QMediaPlayer::EndOfMedia)
            bgm_player->play();  // 循环播放
    });
    bgm_player->play();  // 开始播放

    // 能量豆音效
    powerball_effect = new QSoundEffect;
    powerball_effect->setSource(QUrl("qrc:/source/sound/powerball.wav"));  // 音效文件
    powerball_effect->setVolume(0.9f);  // 音量

    // 胜利文本
    winText = new QGraphicsTextItem("你赢了！");
    winText->setDefaultTextColor(Qt::green);  // 绿色文字
    winText->setFont(QFont("Arial", 24, QFont::Bold));  // 字体
    winText->setPos((W * map_w - winText->boundingRect().width()) / 2, 10);  // 居中显示
    winText->setZValue(10); // 确保在最上层
    winText->hide();  // 初始隐藏
    addItem(winText);  // 添加到场景

    // 失败文本
    loseText = new QGraphicsTextItem("你输了！");
    loseText->setDefaultTextColor(Qt::red);  // 红色文字
    loseText->setFont(QFont("Arial", 24, QFont::Bold));  // 字体
    loseText->setPos((W * map_w - loseText->boundingRect().width()) / 2, 10);  // 居中显示
    loseText->setZValue(10);  // 最上层
    loseText->hide();  // 初始隐藏
    addItem(loseText);  // 添加到场景
}


// 开始游戏
void Game::start()
{
    if (is_paused) return;  // 如果暂停中则不处理

    // 能量豆闪烁定时器
    powerball_flash_timer = new QTimer(this);
    connect(powerball_flash_timer, SIGNAL(timeout()), this , SLOT(powerball_flash()));
    powerball_flash_timer->start(FLASH_INTERVAL);  // 启动定时器

    // Pacman移动定时器
    pacman_timer = new QTimer(this);
    connect(pacman_timer, SIGNAL(timeout()), this , SLOT(pacman_handler()));
    pacman_timer->start(INTERVAL);  // 启动定时器

    // 鬼魂移动定时器
    for (int i = 0; i < Ghost::GhostNum; i++) {
        ghost_timer[i] = new QTimer(this);
        connect(ghost_timer[i], &QTimer::timeout, [=](){ghost_handler(i);} );
        ghost_timer[i]->start(NORMAL_INTERVAL);  // 启动定时器
    }

    // 播放背景音乐
    if (bgm_player) bgm_player->play();
}

// 停止游戏
void Game::stop()
{
    pacman_timer->stop();  // 停止Pacman定时器
    powerball_flash_timer->stop();  // 停止闪烁定时器

    // 停止所有鬼魂定时器
    for (int i = 0; i < Ghost::GhostNum; i++) {
        ghost_timer[i]->stop();
    }

    // 暂停背景音乐
    if (bgm_player) bgm_player->pause();
}

// 切换暂停状态
void Game::togglePause()
{
    if (stat == Pause) {
        stat = Playing;  // 从暂停恢复为进行中
        is_paused = false;
        start();  // 重新开始游戏
    } else if (stat == Playing) {
        stat = Pause;  // 从进行中变为暂停
        is_paused = true;
        stop();  // 停止游戏
    }
}

// 能量豆闪烁效果
void Game::powerball_flash()
{
    if (powerball.empty()) {
        powerball_flash_timer->stop();  // 没有能量豆时停止闪烁
        return;
    }

    if (flash_tick) {
        // 隐藏所有能量豆
        for (int i = 0; i < powerball.size(); i++) {
            powerball.at(i)->hide();
        }
        flash_tick = 0;
    } else {
        // 显示所有能量豆
        for (int i = 0; i < powerball.size(); i++) {
            powerball.at(i)->show();
        }
        flash_tick = 1;
    }
}

// Pacman处理函数
void Game::pacman_handler()
{
    pacman->move();  // 移动Pacman
    updateScoreText();  // 更新分数显示

    // 如果胜利，停止游戏并显示胜利文本
    if (stat == Win) {
        stop();
        winText->show();
    }
}

// 鬼魂处理函数
void Game::ghost_handler(int ghost_id)
{
    ghost[ghost_id]->move();  // 移动鬼魂

    // 如果失败，停止游戏并显示失败文本
    if (stat == Lose) {
        stop();
        loseText->show();
    }
}

// 设置Pacman下一个方向
void Game::pacman_next_direction(GameObject::Dir d)
{
    pacman->set_next_dir(d);  // 设置Pacman的下一个方向
}

// 获取当前分数
int Game::get_score()
{
    return score;
}

// 键盘事件处理
void Game::keyPressEvent(QKeyEvent *event)
{
    // 处理暂停键
    if (event->key() == Qt::Key_P) {
        togglePause();
        return;
    }

    // 非进行中状态不处理方向键
    if (stat != Playing) return;

    // 方向键处理
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        pacman_next_direction(GameObject::Up);  // 上
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        pacman_next_direction(GameObject::Down);  // 下
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        pacman_next_direction(GameObject::Left);  // 左
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        pacman_next_direction(GameObject::Right);  // 右
        break;
    default:
        break;
    }
}

// 更新分数显示
void Game::updateScoreText()
{
    scoreText->setPlainText(QString("分数：%1").arg(score));  // 设置分数文本
}

// 析构函数
Game::~Game()
{
    // 释放地图内存
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            if (map[i][j] != nullptr)
                delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;

    // 释放定时器
    delete pacman_timer;
    delete powerball_flash_timer;
    for (int i = 0; i < Ghost::GhostNum; i++) {
        delete ghost_timer[i];
    }

    // 释放其他资源
    delete scoreText;
    delete bgm_player;
    delete powerball_effect;
    delete winText;
    delete loseText;
}
