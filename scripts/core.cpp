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
bool onpaint    = 0;
StopWatch sw;

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
        case 20:;
        case 40:;
        case 60:;
        case 80:;
        case 150:;
        case 170:;
        case 190:;
        case 210:;
        case 230:;
            enemye.push_back(new EnemyE((halfMapWidth >> 1) + randft(-40, 0), 0, halfMapWidth + randft(-80, 0), halfMapHeight + randft(-20, 20), -1, (halfMapHeight >> 1) + randft(-20, 20)));
            enemye.push_back(new EnemyE(MapWidth * 0.75 + randft(0, 40), 0, halfMapWidth + randft(0, 80), halfMapHeight + randft(-20, 20), MapWidth + 1, (halfMapHeight >> 1) + randft(-20, 20)));
            break;
    }
}

TIMER(GameRun) {
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

    //-碰撞检测
    for(auto eit = enemye.begin(); eit != enemye.end();) {
        bool dead = false;
        for(auto sit = spellcard.begin(); sit != spellcard.end();) {
            if((*eit)->collide(*sit)) {
                (*eit)->hurt(*sit);
                spellcard.erase(sit++);
                if(!(*eit)->alive()) {
                    Game::playSE(SE::ENEMYDEAD);
                    enemye.erase(eit++);
                    dead = true;
                    Game::_score += 50;
                    break;
                }
            }
            else {
                ++sit;
            }
        }
        if(!dead)
            ++eit;
    }
    for(auto rbit = roundbullet.begin(); rbit != roundbullet.end(); ++rbit) {
        if((*rbit)->collide(&reimu)) {
            reimu.hurt(*rbit);
            --Game::_health;
            Game::_score -= 1000;
            Game::playSE(SE::PLAYERDEAD);
            if(!reimu.alive()) {
                cancelTimer(0);
                ERROR_MSG(L"You Lose");
            }
            reimu.respwan();
            roundbullet.clear();
            enemye.clear();
            break;
        }
    }

    //-图像绘制
    if(onpaint)
        return;
    onpaint = true;

    beginPaint();
    Game::BGdraw();
    reimu.draw();
    listdraw(spellcard);
    listdraw(enemye);
    listdraw(roundbullet);
    endPaint();

    onpaint = false;
}
TIMER(fpscounter) {
    static uint last = framecount;
    sw.stop();
    Game::_fps = (framecount - last) / sw.getsecond();
    last       = framecount;
    sw.restart();
}

void Setup() {
    initWindow(title, DEFAULT, DEFAULT, WinWidth, WinHeight);
    initConsole();
    srand(time(nullptr));
    loadResource();
    Game::playBGM(BGM::STG1);
    sw.start();
    startTimer(0, 15, GameRun);
    startTimer(1, 1000, fpscounter);
}