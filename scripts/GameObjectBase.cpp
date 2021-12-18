#include "GameObjectBase.hpp"


namespace ObjectBase {
    object::object(const REAL &x, const REAL &y)
      : _x(x), _y(y) {}
    object::~object() {}

    bool object::inMap(const REAL &width, const REAL &height) const {
        return _x >= 0 && _x <= width && _y >= 0 && _y <= height;
    }

    item::item(const REAL &dx, const REAL &dy)
      : object(0, 0), _dx(dx), _dy(dy) {}
    //实际object构造由最后派生类决定,此处占位

    void item::update() {
        _x += _dx;
        _y += _dy;
    }

    bullet::bullet(const REAL &dx, const REAL &dy, const uint &damage)
      : item(dx, dy), _damage(damage) {}

    chara::chara(const int &health)
      : _health(health) {}

    player::player(const int &health, const REAL &speed)
      : object(0, 0), chara(health), _speed(speed) {}
    //实际object构造由最后派生类决定,此处占位

} // namespace ObjectBase