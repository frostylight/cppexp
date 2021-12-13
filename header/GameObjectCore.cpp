#include "GameObjectCore.hpp"
#include "GameSetting.hpp"

using namespace FROSTYLIB;
using namespace ObjectInterface;
using namespace ObjectBase;
using namespace std;
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
    Reimu::Reimu(const int &health)
      : object(SpwanX, SpwanY), player(health), _speed(ReimuSpeed) {}

    void Reimu::update() {
        _last  = _state;
        _state = 0;
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

        IAnima<Reimu>::update();
    }

} // namespace ObjectCore