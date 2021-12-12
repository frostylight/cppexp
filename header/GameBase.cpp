#include "GameBase.hpp"


namespace Base {
    object::object(const REAL &x, const REAL &y)
      : _x(x), _y(y) {}

    item::item(const REAL &dx, const REAL &dy)
      : _dx(dx), _dy(dy) {}

    void item::update() {
        _x += _dx;
        _y += _dy;
    }

    bullet::bullet(const REAL &dx, const REAL &dy, const uint &damage)
      : item(dx, dy), _damage(damage) {}

    chara::chara(const int &health)
      : _health(health) {}

} // namespace Base