#include "GameBase.hpp"

template<typename T>
T abs(const T &x) {
    return x > 0 ? x : -x;
}

namespace Base {
#pragma region object
    object::object(cREAL &x, cREAL &y)
      : _x(x), _y(y), _state(true) {}
    REAL object::getx() const {
        return _x;
    }
    REAL object::gety() const {
        return _y;
    }
    void object::setx(cREAL &x) {
        _x = x;
    }
    void object::sety(cREAL &y) {
        _y = y;
    }
    void object::setxy(cREAL &x, cREAL &y) {
        _x = x;
        _y = y;
    }
    bool object::active() const {
        return _state;
    }
    void object::disable() {
        _state = false;
    }
    bool object::inscreenB(cREAL &width, cREAL &height) const {
        return _state &&
               (getw() >> 1) <= _x && _x + (getw() >> 1) <= width &&
               (geth() >> 1) <= _y && _y + (geth() >> 1) <= height;
    }
    bool object::inscreen(cREAL &width, cREAL &height) const {
        return _state &&
               0 <= _x && _x <= width &&
               0 <= _y && _y <= height;
    }
    bool object::collide(const object *const obj) const {
        return Base::collide(this, obj);
    }
#pragma endregion

    bool collide(const object *const obj1, const object *const obj2) {
        return obj1->_state && obj2->_state &&
               (obj1->_x - obj2->_x) <= ((obj1->getw() + obj2->getw()) >> 1) &&
               abs(obj1->_y - obj2->_y) <= ((obj1->geth() + obj2->geth()) >> 1);
    }

#pragma region chara
    chara::chara(cREAL &x, cREAL &y, cint &health)
      : object(x, y), _health(health), _cd(1) {}
    int chara::gethealth() const {
        return _health;
    }
    void chara::sethealth(cint &health) {
        _health = health;
    }
    void chara::hurt(cuint &damage) {
        _health -= damage;
    }
    void chara::heal(cuint &health) {
        _health += health;
    }
    bool chara::dead() const {
        return _health <= 0;
    }
    bool chara::bulletReady() const {
        return _state && _cd == getbulletCD();
    }
    bool chara::active() const {
        return object::active() && _health > 0;
    }
    void chara::update() {
        if(_cd == getbulletCD())
            _cd = 1;
        else
            ++_cd;
    }
#pragma endregion

#pragma region player
    player::player(cREAL &x, cREAL &y, cint &health, cREAL &speed)
      : chara(x, y, health), _speed(speed) {}
    REAL player::getspeed() const {
        return _speed;
    }
    void player::setspeed(cREAL &speed) {
        _speed = speed;
    }
#pragma endregion

#pragma region enemy
    enemy::enemy(cREAL &x, cREAL &y, cint &health)
      : chara(x, y, health) {}
#pragma endregion

#pragma region drop
    drop::drop(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy)
      : object(x, y), _dx(dx), _dy(dy) {}
    void drop::update() {
        if(!_state)
            return;
        _x += _dx;
        _y += _dy;
    }
#pragma endregion

#pragma region bullet
    bullet::bullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage)
      : drop(x, y, dx, dy), _damage(damage) {}
#pragma endregion

#pragma region playerbullet
    playerbullet::playerbullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage)
      : bullet(x, y, dx, dy, damage) {}
    playerbullet::playerbullet(const player *const p, cREAL &dx, cREAL &dy, cuint &damage)
      : bullet(p->_x, p->_y, dx, dy, damage) {}
#pragma endregion

#pragma region enemybullet
    enemybullet::enemybullet(cREAL &x, cREAL &y, cREAL &dx, cREAL &dy, cuint &damage)
      : bullet(x, y, dx, dy, damage) {}
    enemybullet::enemybullet(const enemy *const e, cREAL &dx, cREAL &dy, cuint &damage)
      : bullet(e->_x, e->_y, dx, dy, damage) {}
#pragma endregion

} // namespace Base