#include"resource.hpp"


Animation<4, 3> reimu_idle;
Animation<3, 3> reimu_t;
Animation<4, 3> reimu_lr;

FImage *spellcard;
FImage *enemyblue[4];
FImage *bluebullet;

void loadResource(){
    reimu_idle.load({
        L"img/reimu/idle/0.png",
        L"img/reimu/idle/1.png",
        L"img/reimu/idle/2.png",
        L"img/reimu/idle/3.png" });
    reimu_t.load({
        L"img/reimu/turn/0.png",
        L"img/reimu/turn/1.png",
        L"img/reimu/turn/2.png" });
    reimu_lr.load({
        L"img/reimu/lr/0.png",
        L"img/reimu/lr/1.png",
        L"img/reimu/lr/2.png",
        L"img/reimu/lr/3.png" });

    spellcard = FImage::FromFile(L"img/reimu/spellcard.png");

    enemyblue[0] = FImage::FromFile(L"img/blue/0.png");
    enemyblue[1] = FImage::FromFile(L"img/blue/1.png");
    enemyblue[2] = FImage::FromFile(L"img/blue/2.png");
    enemyblue[3] = FImage::FromFile(L"img/blue/3.png");

    bluebullet = FImage::FromFile(L"img/blue/blue.png");
}