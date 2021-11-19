#include "object.hpp"

object::object(cREAL &X, cREAL &Y)
  : x(X), y(Y) {}
REAL object::getX() const {
    return x;
}
REAL object::getY() const {
    return y;
}
void object::disable() {
    x = y = -1;
}

item::item(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY)
  : object(X, Y), dx(dX), dy(dY) {}

bullet::bullet(cREAL &X, cREAL &Y, cREAL &dX, cREAL &dY, cint &Damage)
  : item(X, Y, dX, dY), damage(Damage) {}
void bullet::update() {
    if(!getState())
        return;
    x += dx;
    y += dy;
}

chara::chara(cREAL &X, cREAL &Y, cint &Health)
  : object(X, Y), health(Health) {}
void chara::setHealth(cint &Health) {
    health = Health;
}
int chara::getHealth() const {
    return health;
}
bool chara::getState() const {
    return object::getState() ? (health > 0) : false;
}
