#include "GameObjectCore.hpp"
#include "GameSetting.hpp"

using namespace FROSTYLIB;
using namespace ObjectInterface;
using namespace ObjectBase;
using namespace Setting;

template<std::integral T, std::floating_point U>
inline U max(const T &x, const U &y) {
    return x > y ? x : y;
}
template<std::integral T, std::floating_point U>
inline U max(const U &x, const T &y) {
    return x > y ? x : y;
}
template<std::integral T, std::floating_point U>
inline U min(const T &x, const U &y) {
    return x < y ? x : y;
}
template<std::integral T, std::floating_point U>
inline U min(const U &x, const T &y) {
    return x < y ? x : y;
}

namespace ObjectCore {
    //-Reimu
    Reimu::Reimu(const int &health): object(SpwanX, SpwanY), player(health, ReimuSpeed) {}
    void Reimu::update() {
        _last  = _state;
        _state = IDLE;
        if(ishold(VK_UP) ^ ishold(VK_DOWN)) {
            if(ishold(VK_UP))
                _y = max(IBox<Reimu>::_halfboxheight, _y - _speed);
            else {
                _y = min(MapHeight - IBox<Reimu>::_halfboxheight, _y + _speed);
            }
        }
        if(ishold(VK_LEFT) ^ ishold(VK_RIGHT)) {
            if(ishold(VK_LEFT)) {
                _state |= LEFT;
                _x = max(IBox<Reimu>::_halfboxwidth, _x - _speed);
            }
            else {
                _state |= RIGHT;
                _x = min(MapWidth - IBox<Reimu>::_halfboxwidth, _x + _speed);
            }
        }
        if(ishold(Key::AttackKey))
            Ishot<Reimu>::update();
        IAnima<Reimu>::update();
    }

    //-SpellCard
    SpellCard::SpellCard(const REAL &x, const REAL &y, const REAL &speed, const uint &damage): object(x, y), playerbullet(0, -speed, damage) {}
    SpellCard *SpellCard::fromReimu(const Reimu *const reimu) {
        return new SpellCard(reimu->_x, reimu->_y, SpellCardSpeed, 1);
    }
    void SpellCard::draw() {
        IImg<SpellCard>::_img->drawC(_x, _y + SpellCardFix);
    }

    //-RoundBullet
    RoundBullet::RoundBullet(const REAL &x, const REAL &y, const REAL &dx, const REAL &dy): object(x, y), enemybullet(dx, dy, 1){};
    RoundBullet *RoundBullet::fromEnemyE(const EnemyE *const enemye, const Reimu *const reimu) {
        static REAL dx, dy, dis;
        dx  = reimu->_x - enemye->_x;
        dy  = reimu->_y - enemye->_y;
        dis = std::sqrt(dx * dx + dy * dy);
        return new RoundBullet(enemye->_x, enemye->_y, RoundSpeed * dx / dis, RoundSpeed * dy / dis);
    }

} // namespace ObjectCore