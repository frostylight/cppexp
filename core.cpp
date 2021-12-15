#include <cstdio>

#include "header/GameObjectCore.hpp"
#include "header/GameResource.hpp"

using namespace FROSTYLIB;
using namespace ObjectCore;
using namespace Setting;

Reimu reimu(3);

TIMER(GameRun) {
    //数据更新
    Game::BGupdate();
    reimu.update();

    //图像绘制
    beginPaint();
    Game::BGdraw();
    reimu.draw();
    endPaint();
}


void Setup() {
    initWindow(title, DEFAULT, DEFAULT, WinWidth, WinHeight);
    initConsole();
    loadResource();
    setBrushColor(COLOR::White);
    startTimer(0, 15, GameRun);
}