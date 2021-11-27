#include "game_base.hpp"

using namespace GAME_BASE;

REAL abs(const cREAL &x) {
    return x > 0 ? x : -x;
}

bool GAME_BASE::iscol(const object *const a, const object *const b) {
    return abs(a->x - b->x) <= ((a->w + b->w) >> 1) &&
           abs(a->y - b->y) <= ((a->h + b->h) >> 1);
}
object::object(cREAL &X, cREAL &Y)
  : x(X), y(Y) {}
REAL object::getx() const {
    return x;
}
REAL object::gety() const {
    return y;
}
void object::setxy(cREAL &X, cREAL &Y) {
    x = X;
    y = Y;
}
uint object::getw() const {
    return w;
}
uint object::geth() const {
    return h;
}
bool object::iscol(const object *const a) const {
    return GAME_BASE::iscol(this, a);
}

item::item(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY)
  : object(X, Y), dx(dX), dy(dY) {}

bullet::bullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cuit &Damage)
  : item(X, Y, dX, dY), damage(Damage) {}
bool bullet::update() {
    x += dx;
    y += dy;
    return x >= 0 && y >= 0;
}

chara::chara(cREAL &X, cREAL &Y, cint &Health)
  : object(X, Y), health(Health) {}
void chara::hurt(cuit &damage) {
    health -= damage;
}
bool chara::dead() const {
    return health <= 0;
}
int chara::gethealth() const {
    return health;
}
void chara::sethealth(cint &Health) {
    health = Health;
}