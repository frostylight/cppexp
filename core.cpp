#include <windows.h>

#include <cmath>
#include <ctime>
#include <forward_list>

#include "header/game_core.hpp"

using namespace GAME_BASE;
using namespace GAME_CORE;
using namespace GAME_DEFAULT_SETTING;
using namespace GAME_RESOURCE;

//自机
player reimu(halfMapWidth, MapHeight * 0.8);
//自机弹幕
std::forward_list<playerBullet> playerBulletList;
//敌机
std::forward_list<enemyBullet> enemyBulletList;
//敌机弹幕
std::forward_list<enemy> enemyList;
//背景偏移量
REAL bkshift = 0;
//得分
uint score = 0;

void drawBackground(cREAL &shift);
void drawTab();
void enemyAppear();
void playerShot();
//计算数据、更新画面
void runGame();

void Setup() {
    initWindow("Touhou Project Imperishable Night. (Fake) ver 0.9.5", DEFAULT, DEFAULT, WinWidth, WinHeight);
    initConsole();

    loadResource();
    reimu.updateSize();

    srand(time(nullptr));

    // win32计时器误差, 15ms=60+fps
    startTimer(0, 15, runGame);
}

void runGame() {
    //更新背景偏移
    bkshift += 0.3;
    //更新自机弹幕
    for(auto i = playerBulletList.before_begin(); i._M_next() != playerBulletList.end(); ++i)
        if(!(i._M_next()->update()))
            playerBulletList.erase_after(i);

    //更新敌机弹幕
    for(auto i = enemyBulletList.before_begin(); i._M_next() != enemyBulletList.end(); ++i)
        if(!(i._M_next()->update()))
            enemyBulletList.erase_after(i);

    //更新自机
    reimu.update();
    //碰撞检测
    for(auto i: enemyBulletList) {
        if(reimu.iscol(&i)) {
            reimu.hurt(1);
            if(reimu.dead()) {
                /* TODO 死亡处理
                  暂停所有update
                  显示重开/退出GUI
                  等待鼠标点击*/
            }
            else {
                // 弹幕清屏
                enemyBulletList.clear();
                // TODO 此处应有特效，并停止所有object的update
                //  重置自机位置
                reimu.setxy(halfMapWidth, MapHeight * 0.8);
                // TODO 2-3s无敌状态
                break;
            }
        }
    }
    if(ishold(0x5a))
        playerShot();

    //更新敌机
    for(auto i = enemyList.before_begin(); i._M_next() != enemyList.end(); ++i)
        if(!(i._M_next()->update()))
            enemyList.erase_after(i);
        else {
            //碰撞检测
            for(auto j: playerBulletList)
                if(i._M_next()->iscol(&j)) {
                    /* TODO 伤害计算
                    消除命中子弹*/
                    if(i._M_next()->dead()) {
                        /* TODO 死亡结算
                        消除敌机
                        可能有特效
                        分数增加
                        道具掉落*/
                    }
                }
            //发射弹幕
            if(i._M_next()->ready()) {
                REAL px = reimu.getx(), py = reimu.gety();
                REAL x = i._M_next()->getx(), y = i._M_next()->gety();
                REAL l = sqrt((px - x) * (px - x) + (py - y) * (py - y));
                enemyBulletList.emplace_front(x, y, (px - x) / l, (py - y) / l);
            }
        }
    enemyAppear();


    beginPaint();

    //重绘
    drawBackground(bkshift);
    drawTab();

    for(auto i: playerBulletList)
        i.draw();
    reimu.draw();
    for(auto i: enemyList)
        i.draw();
    for(auto i: enemyBulletList)
        i.draw();

    endPaint();
}

void enemyAppear() {
    // TODO 重新设计关卡
    static int framecount = 0;
    switch(++framecount) {
        case 60:;
        case 70:;
        case 80:;
        case 90:;
        case 100:;
            enemyList.emplace_front((MapWidth >> 2) + rand() % 41 - 20, -5, (MapWidth >> 2) + rand() % 41 - 20, halfMapHeight, 120);
            break;
        case 110:;
        case 120:;
        case 130:;
        case 140:;
        case 150:;
            enemyList.emplace_front(MapWidth - (MapWidth >> 2) + rand() % 41 - 20, -5, MapWidth - (MapWidth >> 2) + rand() % 41 - 20, halfMapHeight, 120);
            break;
    }
}

void playerShot() {
    static int CD = 5;
    if(++CD == 6) {
        CD = 0;
        playerBulletList.emplace_front(reimu.getx(), reimu.gety() - 10, 0, -15);
    }
}


void drawBackground(cREAL &shift) {
    background->drawCover(halfMapWidth, halfMapHeight, MapWidth, MapHeight, 0, shift);
    TabBackground->drawCover(halfMapWidth + halfWinWidth, halfWinHeight, WinWidth - MapWidth, WinHeight, 0, 0);
}

WCHAR c[25];
void drawTab() {
    // TODO 重新设计模块
    setTextAlign(Gdiplus::StringAlignment::StringAlignmentNear);
    setTextSize(20);
    setTextColor(COLOR::White);
    setTextStyle(Gdiplus::FontStyle::FontStyleBold);
    wsprintfW(c, L"Score  %010d", score);
    paintText(c, MapWidth + 20, 35);
    wsprintfW(c, L"Health %d", reimu.gethealth());
    paintText(c, MapWidth + 20, 85);
    setTextSize(15);
    setTextStyle(Gdiplus::FontStyle::FontStyleBoldItalic);
    setTextColor(COLOR::Aqua);
    paintText(L"LOOP", 550, 200);
    Symbol->drawAround(halfMapWidth + halfWinWidth, 340);
    setTextSize(12);
    setTextStyle(Gdiplus::FontStyle::FontStyleItalic);
    setTextColor(COLOR::Violet);
    paintText(L"Imperishable Night", 430, 405);
}
