#include "GameResource.hpp"

using namespace FROSTYLIB;

namespace Resource {
    FROSTYLIB::ImgList Reimu_Idle;
    FROSTYLIB::ImgList Reimu_LR;
    FROSTYLIB::ImgList Reimu_Turn;
    Img *SpellCard;

    ImgList EnemyE;
    Img *RoundBullet;

    Img *Background;
    Img *TabBarBackground;
    Img *Symbol;

    void load() {
        Reimu_Idle.load(8, {L"img/Chara&Bullet/Reimu/Idle/0.png",
                            L"img/Chara&Bullet/Reimu/Idle/1.png",
                            L"img/Chara&Bullet/Reimu/Idle/2.png",
                            L"img/Chara&Bullet/Reimu/Idle/3.png"});
        Reimu_LR.load(8, {L"img/Chara&Bullet/Reimu/LeftRight/0.png",
                          L"img/Chara&Bullet/Reimu/LeftRight/1.png",
                          L"img/Chara&Bullet/Reimu/LeftRight/2.png",
                          L"img/Chara&Bullet/Reimu/LeftRight/3.png"});
        Reimu_Turn.load(6, {L"img/Chara&Bullet/Reimu/Turn/0.png",
                            L"img/Chara&Bullet/Reimu/Turn/1.png",
                            L"img/Chara&Bullet/Reimu/Turn/2.png"});
        SpellCard = Img::FromFile(L"img/Chara&Bullet/Reimu/SpellCard.png");

        EnemyE.load(8, {L"img/Chara&Bullet/EnemyE/Blue/Idle/0.png",
                        L"img/Chara&Bullet/EnemyE/Blue/Idle/1.png"
                        L"img/Chara&Bullet/EnemyE/Blue/Idle/2.png",
                        L"img/Chara&Bullet/EnemyE/Blue/Idle/3.png"});
        RoundBullet = Img::FromFile(L"img/Chara&Bullet/EnemyE/Blue/RoundBullet.png");

        Background       = Img::FromFile(L"img/UI/Map/Background.png");
        TabBarBackground = Img::FromFile(L"img/UI/TabBar/Background.png");
        Symbol           = Img::FromFile(L"img/UI/TabBar/Symbol.png");
    }

} // namespace Resource