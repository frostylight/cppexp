#include <ctime>

#include "game_core.hpp"

using namespace GAME_CORE;
using namespace GAME_RESOURCE;
using namespace GAME_DEFAULT_SETTING;

template<typename T>
concept isnumber = std::is_integral_v<T> || std::is_floating_point_v<T>;

template<isnumber T1, isnumber T2>
REAL max(const T1 &x, const T2 &y) {
    return x > y ? x : y;
}
template<isnumber T1, isnumber T2>
REAL min(const T1 &x, const T2 &y) {
    return x < y ? x : y;
}
template<isnumber T>
T abs(const T &x) {
    return x > 0 ? x : -x;
}


playerBullet::playerBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cuit &Damage)
  : bullet(X, Y, dX, dY, Damage) {
    w = spellcard->GetWidth();
    h = spellcard->GetHeight();
}
bool playerBullet::update() {
    return bullet::update() && x <= MapWidth && y <= MapHeight;
}
void playerBullet::draw() const {
    spellcard->drawAround(x, y);
}

enemyBullet::enemyBullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cuit &Damage)
  : bullet(X, Y, dX, dY, Damage) {
    w = bullet_round[ENEMY_TYPE::BLUE]->GetWidth();
    h = bullet_round[ENEMY_TYPE::BLUE]->GetHeight();
}
bool enemyBullet::update() {
    return bullet::update() && x <= MapWidth && y <= MapHeight;
}
void enemyBullet::draw() const {
    bullet_round[ENEMY_TYPE::BLUE]->drawAround(x, y);
}

player::player(cREAL &X, cREAL &Y, cint &Health, cREAL &Speed)
  : chara(X, Y, Health), speed(Speed) {
    srand(time(nullptr));
}
void player::updateSize() {
    w = reimu_idle.getw();
    h = reimu_idle.geth();
}
bool player::update() {
    if(ishold(VK_UP))
        y = max(y - speed, h >> 1);
    if(ishold(VK_DOWN))
        y = min(y + speed, MapHeight - (h >> 1));
    if(ishold(VK_LEFT))
        x = max(x - speed, w >> 1);
    if(ishold(VK_RIGHT))
        x = min(x + speed, MapWidth - (w >> 1));
    return true;
}
void player::draw() const {
    static bool lr = 0;
    static uint lt = 0;
    if(ishold(VK_LEFT) ^ ishold(VK_RIGHT)) {
        if(ishold(VK_RIGHT) ^ lr) {
            lr = !lr;
            lt = 0;
            reimu_lr.reset();
            reimu_t.reset();
        }
        if(++lt <= reimu_t.allt) {
            reimu_t.drawAroundFlip(x, y, lr);
            reimu_idle.reset();
        }
        else {
            reimu_lr.drawAroundFlip(x, y, lr);
            reimu_t.reset();
        }
    }
    else {
        lt = 0;
        reimu_idle.drawAroundFlip(x, y, lr);
        reimu_lr.reset();
        reimu_t.reset();
    }
}

enemy::enemy(cREAL &X, cREAL &Y, cREAL &sX, cREAL &sY, cuit &sTime, cint &Health)
  : chara(X, Y, Health), sx(sX), sy(sY), ex(X), ey(Y), stayTime(sTime) {
    w  = enemy_easy[ENEMY_TYPE::BLUE].getw();
    h  = enemy_easy[ENEMY_TYPE::BLUE].geth();
    f  = 0;
    cd = 0;
}
bool enemy::ready() const {
    return f && stayTime && cd == enemyShotCD;
}
bool enemy::update() {
    cd = cd % enemyShotCD + 1;
    if(!f) {
        x += (sx - x) * 0.1 + x > sx ? -2 : 2;
        y += (sy - y) * 0.1 + y > sy ? -2 : 2;
        if(abs(x - sx) <= 20, abs(y - sy) <= 20)
            f = 1;
    }
    else {
        if(stayTime) {
            if(cd & 1) {
                x += rand() % 2 ? -2 : 2;
                y += rand() % 2 ? -2 : 2;
            }
            --stayTime;
        }
        else {
            x += (ex - x) * 0.1 + x > ex ? -2 : 2;
            y += (ey - y) * 0.1 + y > ey ? -2 : 2;
        }
    }
    return !f || (0 <= x && x <= MapWidth && 0 <= y && y <= MapHeight);
}
void enemy::draw() const {
    enemy_easy[ENEMY_TYPE::BLUE].drawAround(x, y);
}