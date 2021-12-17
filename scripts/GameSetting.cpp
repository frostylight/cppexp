#include "GameObjectCore.hpp"

using namespace FROSTYLIB;
using namespace ObjectCore;

#define definebox(classname, width, height)                     \
    template<>                                                  \
    constexpr uint IBox<classname>::_boxwidth = width;          \
    template<>                                                  \
    constexpr uint IBox<classname>::_boxheight = height;        \
    template<>                                                  \
    constexpr uint IBox<classname>::_halfboxwidth = width >> 1; \
    template<>                                                  \
    constexpr uint IBox<classname>::_halfboxheight = height >> 1;


namespace ObjectInterface {
    definebox(Reimu, 24, 40)
    definebox(SpellCard, 12, 14)
    definebox(EnemyE, 20, 22)
    definebox(RoundBullet, 16, 16)
} // namespace ObjectInterface