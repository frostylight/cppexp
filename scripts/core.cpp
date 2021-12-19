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

//定时器回调函数GameRun
TIMER(GameRun) {
    //帧计数加1,敌机生成与FPS计算基于此实现
    ++framecount;

    //-物体的数据更新
    Game::BGupdate(); //更新背景数据
    reimu.update(); //更新自机数据
    //如果当前按下发射键且自机发射子弹冷却已结束
    if(ishold(Key::AttackKey) && reimu.ready()) {
        //向自机子弹链表中添加新生成的子弹
        spellcard.push_back(SpellCard::fromReimu(&reimu));
        //播放子弹发射音效
        Game::playSE(SE::PLAYERSHOT);
    }
    //更新以链表为容器的自机子弹,
    listupdate(spellcard);
    //根据当前帧计数向敌机链表中添加生成的敌机
    stgenemy();
    //更新敌机
    listupdate(enemye);
    //对于敌机链表内的敌机,即在场的敌机
    for(auto &e: enemye) {
        //如果发射冷却已经结束
        if(e->ready()) {
            //向敌机子弹链表中加入新生成的子弹
            roundbullet.push_back(RoundBullet::fromEnemyE(e, &reimu));
            //播放音效
            Game::playSE(SE::ENEMYSHOT);
        }
    }
    //更新敌机子弹
    listupdate(roundbullet);

    //-碰撞检测相关,因规模较小故不使用四叉树
    //对于每个在场敌机
    for(auto eit = enemye.begin(); eit != enemye.end();) {
        //敌机是否被子弹杀死的标识
        bool dead = false;
        //对于每个在场的自机子弹
        for(auto sit = spellcard.begin(); sit != spellcard.end();) {
            //如果两者碰撞
            if((*eit)->collide(*sit)) {
                //敌机受到子弹伤害
                (*eit)->hurt(*sit);
                //将命中的子弹移除
                spellcard.erase(sit++);
                //如果敌机死亡
                if(!(*eit)->alive()) {
                    //播放敌机死亡的音效
                    Game::playSE(SE::ENEMYDEAD);
                    //将敌机移除
                    enemye.erase(eit++);
                    //标记为已死亡避免迭代错误
                    dead = true;
                    //玩家得分
                    Game::_score += 50;
                    //退出循环
                    break;
                }
            }
            //未碰撞的情况
            else {
                //开始下一个自机子弹判断
                ++sit;
            }
        }
        //如果敌机未死亡,迭代下一个敌机
        if(!dead)
            ++eit;
    }
    //同上,自机与敌机子弹的碰撞检测
    for(auto rbit = roundbullet.begin(); rbit != roundbullet.end(); ++rbit) {
        if((*rbit)->collide(&reimu)) {
            reimu.hurt(*rbit);
            --Game::_health;
            //死亡惩罚,损失分数
            Game::_score -= 1000;
            Game::playSE(SE::PLAYERDEAD);
            //如果生命值归0
            if(!reimu.alive()) {
                //停止游戏
                cancelTimer(0);
                ERROR_MSG(L"You Lose");
            }
            //否则自机重生
            reimu.respwan();
            //清屏
            roundbullet.clear();
            enemye.clear();
            break;
        }
    }

    //-图像绘制
    //如果上一次绘制未结束,则跳过此次绘制
    if(onpaint)
        return;
    onpaint = true;

    beginPaint();
    //绘制背景/自机/自机子弹/敌机/敌机子弹
    Game::BGdraw();
    reimu.draw();
    listdraw(spellcard);
    listdraw(enemye);
    listdraw(roundbullet);
    endPaint();

    onpaint = false;
}
//定时器回调函数fpscounter
TIMER(fpscounter) {
    //上一次帧计数
    static uint last = framecount;
    //停止计时
    sw.stop();
    //计算游戏FPS
    Game::_fps = (framecount - last) / sw.getsecond();
    //记录帧计数
    last = framecount;
    //重新开始计时
    sw.restart();
}

void Setup() {
    initWindow(title, DEFAULT, DEFAULT, WinWidth, WinHeight);
    initConsole();
    srand(time(nullptr));
    //加载游戏资源
    loadResource();
    //播放BGM
    Game::playBGM(BGM::STG1);
    //开始秒表计时
    sw.start();
    //开始游戏逻辑
    startTimer(0, 15, GameRun);
    // FPS计算
    startTimer(1, 1000, fpscounter);
}