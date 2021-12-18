#include <cstdio>
#include <ctime>
#include <list>
#include <random>

#include "GameObjectCore.hpp"
#include "GameResource.hpp"

using namespace FROSTYLIB;
using namespace ObjectBase;
using namespace ObjectCore;
using namespace Setting;

uint framecount = 0;
bool onrun      = 0;

Reimu reimu(3);
std::list<SpellCard *> spellcard;
std::list<EnemyE *> enemye;
std::list<RoundBullet *> roundbullet;

template<typename T>
requires std::derived_from<T, object>
void listupdate(std::list<T *> &objlist) {
    for(auto it = objlist.begin(); it != objlist.end();) {
        (*it)->update();
        if(!(*it)->inMap(MapWidth, MapHeight)) {
            delete *it;
            objlist.erase(it++);
        }
        else
            ++it;
    }
}
template<typename T>
requires std::derived_from<T, object>
void listdraw(std::list<T *> &objlist) {
    for(auto &obj: objlist)
        obj->draw();
}

REAL randft(const REAL &a, const REAL &b) {
    static uint len = (b - a) * 10 + 1;
    return a + (rand() % len) / 10.0;
}

//关卡设计
void stgenemy() {
    if(framecount < 300)
        return;
    switch(framecount % 300) {
        case 0:;
        case 10:;
        case 15:;
            enemye.push_back(new EnemyE((halfMapWidth >> 1) + randft(-40, 0), 0, halfMapWidth + randft(-40, 0), halfMapHeight + randft(-20, 20), -1, (halfMapHeight >> 1) + randft(-20, 20), 1));
            break;
    }
}

TIMER(GameRun) {
    if(onrun)
        return;
    onrun = true;

    ++framecount;

    //-数据更新
    Game::BGupdate();
    reimu.update();
    if(ishold(Key::AttackKey) && reimu.ready()) {
        spellcard.push_back(SpellCard::fromReimu(&reimu));
        Game::playSE(SE::PLAYERSHOT);
    }
    listupdate(spellcard);
    stgenemy();
    listupdate(enemye);
    for(auto &e: enemye) {
        if(e->ready()) {
            roundbullet.push_back(RoundBullet::fromEnemyE(e, &reimu));
            Game::playSE(SE::ENEMYSHOT);
        }
    }
    listupdate(roundbullet);

    // todo 碰撞检测

    //-图像绘制
    beginPaint();
    Game::BGdraw();
    reimu.draw();
    listdraw(spellcard);
    listdraw(enemye);
    listdraw(roundbullet);
    endPaint();

    onrun = false;
}

void Setup() {
    initWindow(title, DEFAULT, DEFAULT, WinWidth, WinHeight);
    initConsole();
    srand(time(nullptr));
    loadResource();
    setBrushColor(COLOR::White);
    Game::playBGM(BGM::STG1);
    startTimer(0, 15, GameRun);
}