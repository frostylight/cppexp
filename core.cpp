#include <cstdio>

#include "header/GameObjectCore.hpp"
#include "header/GameResource.hpp"

using namespace FROSTYLIB;
using namespace ObjectCore;
using namespace Setting;

Reimu reimu(3);

TIMER(GameRun) {
    reimu.update();
    beginPaint();
    rectangle(0, 0, MapWidth, MapHeight);
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