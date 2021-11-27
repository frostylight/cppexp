#include "game_resource.hpp"

using namespace GAME_RESOURCE;


namespace GAME_RESOURCE {
    LoopAnimation<4> reimu_idle;
    TransitAnimation<3> reimu_t;
    LoopAnimation<4> reimu_lr;
    FImage *spellcard;

    LoopAnimation<4> enemy_easy[ENEMY_TYPE::MAXSIZE];
    FImage *bullet_round[ENEMY_TYPE::MAXSIZE];

    void loadResource() {
        reimu_idle.load({L"img/reimu/idle/0.png",
                         L"img/reimu/idle/1.png",
                         L"img/reimu/idle/2.png",
                         L"img/reimu/idle/3.png"});
        reimu_t.load({L"img/reimu/turn/0.png",
                      L"img/reimu/turn/1.png",
                      L"img/reimu/turn/2.png"});
        reimu_lr.load({L"img/reimu/lr/0.png",
                       L"img/reimu/lr/1.png",
                       L"img/reimu/lr/2.png",
                       L"img/reimu/lr/3.png"});
        spellcard = FImage::FromFile(L"img/reimu/spellcard.png");

        enemy_easy[ENEMY_TYPE::BLUE].load({L"img/blue/0.png",
                                           L"img/blue/1.png",
                                           L"img/blue/2.png",
                                           L"img/blue/3.png"});
        bullet_round[ENEMY_TYPE::BLUE] = FImage::FromFile(L"img/blue/bullet.png");
    }
} // namespace GAME_RESOURCE
