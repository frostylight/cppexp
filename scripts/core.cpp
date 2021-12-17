#include <cstdio>
#include <list>

#include "GameObjectCore.hpp"
#include "GameResource.hpp"

using namespace FROSTYLIB;
using namespace ObjectBase;
using namespace ObjectCore;
using namespace Setting;

player *reimu = new Reimu(3);
std::list<playerbullet *> pb;
std::list<enemy *> en;
std::list<enemybullet *> eb;

TIMER(GameRun) {
    //数据更新
    Game::BGupdate();
    reimu->update();

    //图像绘制
    beginPaint();
    Game::BGdraw();
    reimu->draw();
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