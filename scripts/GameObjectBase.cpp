#include "GameObjectBase.hpp"


namespace ObjectBase {
    //-object
    object::object(const REAL &x, const REAL &y): _x(x), _y(y) {}
    object::~object() {}
    REAL object::getx() const {
        return _x;
    }
    REAL object::gety() const {
        return _y;
    }
    bool object::inMap(const REAL &width, const REAL &height) const {
        return _x >= 0 && _x <= width && _y >= 0 && _y <= height;
    }

    //-item
    item::item(const REAL &dx, const REAL &dy): object(0, 0), _dx(dx), _dy(dy) {}
    //实际object构造由最后派生类决定,此处占位
    void item::update() {
        _x += _dx;
        _y += _dy;
    }

    //-bullet
    bullet::bullet(const REAL &dx, const REAL &dy, const uint &damage): item(dx, dy), _damage(damage) {}

    //-chara
    chara::chara(const int &health): object(0, 0), _health(health) {}
    //实际object构造由最后派生类决定,此处占位
    void chara::hurt(const bullet *const bt) {
        _health -= bt->_damage;
    }
    bool chara::alive() const {
        return _health > 0;
    }

    //-player
    player::player(const int &health, const REAL &speed): chara(health), _speed(speed) {}

} // namespace ObjectBase