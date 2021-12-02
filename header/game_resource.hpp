#pragma once

#include "gdiplib.hpp"

template<uint sz>
using LoopAnimation = Animation<sz, 8>;

template<uint sz>
using TransitAnimation = Animation<sz, 6>;

enum ENEMY_TYPE { BLUE,
                  MAXSIZE };


namespace GAME_RESOURCE {


    //自机待机
    extern LoopAnimation<4> reimu_idle;
    //自机转向(过渡)
    extern TransitAnimation<3> reimu_t;
    //自机转向
    extern LoopAnimation<4> reimu_lr;
    //自机弹幕
    extern FImage *spellcard;

    //敌机(简易)
    extern LoopAnimation<4> enemy_easy[ENEMY_TYPE::MAXSIZE];
    //敌机弹幕
    extern FImage *bullet_round[ENEMY_TYPE::MAXSIZE];

    extern FImage *background;

    //加载图片资源
    void loadResource();
} // namespace GAME_RESOURCE
