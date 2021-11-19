#pragma once

#include "frostylib.hpp"

template<uint sz>
using LoopAnimation = Animation<sz, 8>;

template<uint sz>
using TransitAnimation = Animation<sz, 6>;

//自机待机
extern LoopAnimation<4> reimu_idle;

//自机转向(过渡)
extern TransitAnimation<3> reimu_t;

//自机转向
extern LoopAnimation<4> reimu_lr;

//自机弹幕
extern FImage *spellcard;

//敌机(简易)
extern LoopAnimation<4> enemyblue;

//敌机弹幕
extern FImage *bluebullet;

/**
 * @brief 加载图片资源
 */
void loadResource();
