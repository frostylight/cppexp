#pragma once

#include"frostylib.hpp"

//自机待机
extern Animation<4, 3> reimu_idle;

//自机转向(过渡)
extern Animation<3, 3> reimu_t;

//自机转向
extern Animation<4, 3> reimu_lr;

//自机弹幕
extern FImage *spellcard;

//敌机(简易)
extern FImage *enemyblue[4];

//敌机弹幕
extern FImage *bluebullet;

/**
 * @brief 加载图片资源
 */
void loadResource();

