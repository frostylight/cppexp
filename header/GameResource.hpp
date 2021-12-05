#pragma once

#include "frostylib.hpp"

namespace Resource {
    enum ReimuAnima { Idle,
                      LeftRight,
                      Turn };

    extern FROSTYLIB::ImgList Reimu[3];
    extern FROSTYLIB::Img *SpellCard;

    extern FROSTYLIB::ImgList EnemyE;
    extern FROSTYLIB::Img *RoundBullet;

    extern FROSTYLIB::Img *Background;
    extern FROSTYLIB::Img *TabBarBackground;
    extern FROSTYLIB::Img *Symbol;

    void load();

} // namespace Resource