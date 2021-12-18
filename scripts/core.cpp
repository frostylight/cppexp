#include <cstdio>
#include <list>

#include "GameObjectCore.hpp"
#include "GameResource.hpp"

using namespace FROSTYLIB;
using namespace ObjectBase;
using namespace ObjectCore;
using namespace Setting;

Reimu reimu(3);
std::list<SpellCard *> pb;
std::list<EnemyE *> en;
std::list<RoundBullet *> eb;

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

TIMER(GameRun) {
    //-数据更新
    Game::BGupdate();
    reimu.update();
    if(ishold(Key::AttackKey) && reimu.ready()) {
        pb.push_back(SpellCard::fromReimu(&reimu));
        Game::playSE(SE::PLAYERSHOT);
    }
    listupdate(pb);
    listupdate(en);
    for(auto it = en.begin(); it != en.end();) {
        if((*it)->ready()) {
            eb.push_back(RoundBullet::fromEnemyE(*it, &reimu));
            Game::playSE(SE::ENEMYSHOT);
        }
    }
    listupdate(eb);

    // todo 碰撞检测

    //-图像绘制
    beginPaint();
    Game::BGdraw();
    reimu.draw();
    for(auto it = pb.begin(); it != pb.end();) {
        (*it)->draw();
        ++it;
    }
    endPaint();
}

void Setup() {
    initWindow(title, DEFAULT, DEFAULT, WinWidth, WinHeight);
    initConsole();
    loadResource();
    setBrushColor(COLOR::White);
    Game::playBGM(BGM::STG1);
    startTimer(0, 15, GameRun);
}