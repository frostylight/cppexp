#include "GameObjectCore.hpp"

using namespace FROSTYLIB;
using namespace ObjectCore;

//设置碰撞箱
#define setBox(classname, width, height)                        \
    template<>                                                  \
    constexpr uint IBox<classname>::_boxwidth = width;          \
    template<>                                                  \
    constexpr uint IBox<classname>::_boxheight = height;        \
    template<>                                                  \
    constexpr uint IBox<classname>::_halfboxwidth = width >> 1; \
    template<>                                                  \
    constexpr uint IBox<classname>::_halfboxheight = height >> 1;

//设置发射CD
#define setshotCD(classname, CD) \
    template<>                   \
    constexpr uint Ishot<classname>::_shotCD = CD;


namespace ObjectInterface {
    setBox(Reimu, 14, 30)
    setBox(SpellCard, 12, 14)
    setBox(EnemyE, 20, 22)
    setBox(RoundBullet, 16, 16)

    setshotCD(Reimu, 5);
    setshotCD(EnemyE, 120);
} // namespace ObjectInterface