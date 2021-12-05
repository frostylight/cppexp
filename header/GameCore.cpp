#include "GameCore.hpp"
#include "GameResource.hpp"

using FROSTYLIB::ishold;

inline REAL min(cREAL &x, cREAL &y) {
    return x < y ? x : y;
}
inline REAL max(cREAL &x, cREAL &y) {
    return x > y ? x : y;
}


namespace Touhou {
#pragma region Reimu
    uint Reimu::getw() const {
        return Setting::Reimu::BoxWidth;
    }
    uint Reimu::geth() const {
        return Setting::Reimu::BoxHeight;
    }
    uint Reimu::getbulletCD() const {
        return Setting::Reimu::BulletCD;
    }
    Reimu::Reimu(cint &health)
      : Base::player(Setting::Game::SpawnX,
                     Setting::Game::SpawnY,
                     health,
                     Setting::Reimu::Speed),
        _direction(0), _onturn(0) {}
    void Reimu::respawn() {
        _x         = Setting::Game::SpawnX;
        _y         = Setting::Game::SpawnY;
        _direction = 0;
        _onturn    = 0;
    }
    void Reimu::update() {
        if(ishold(VK_UP))
            _y = max(_y - _speed, geth() >> 1);
        if(ishold(VK_DOWN))
            _y = min(_y + _speed, Setting::Game::MapHeight - (geth() >> 1));
        if(ishold(VK_LEFT))
            _x = max(_x - _speed, getw() >> 1);
        if(ishold(VK_RIGHT))
            _x = min(_x + _speed, Setting::Game::MapWidth - (getw() >> 1));
        if(ishold('Z'))
            Base::chara::update();
    }
    void Reimu::draw() {
#define Reimu_Idle Resource::Reimu[Resource::ReimuAnima::Idle]
#define Reimu_LR Resource::Reimu[Resource::ReimuAnima::LeftRight]
#define Reimu_T Resource::Reimu[Resource::ReimuAnima::Turn]
        if(ishold(VK_LEFT) ^ ishold(VK_RIGHT)) {
            if(ishold(VK_RIGHT) ^ _direction) {
                _direction = !_direction;
                _onturn    = 0;
                Reimu_LR.restart();
                Reimu_T.restart();
            }
            if(++_onturn <= Reimu_T.getTotalTime()) {
                Reimu_T.drawFlipC(_x, _y, _direction);
                Reimu_Idle.restart();
            }
            else {
                Reimu_LR.drawFlipC(_x, _y, _direction);
                Reimu_T.restart();
            }
        }
        else {
            _onturn = 0;
            Reimu_Idle.drawFlipC(_x, _y, _direction);
            Reimu_LR.restart();
            Reimu_T.restart();
        }
#undef Reimu_Idle
#undef Reimu_LR
#undef Reimu_T
    }
#pragma endregion

#pragma region SpellCard
    uint SpellCard::getw() const {
        return Setting::Reimu::SpellCard::BoxWidth;
    }
    uint SpellCard::geth() const {
        return Setting::Reimu::SpellCard::BoxHeight;
    }
    SpellCard::SpellCard(const Reimu *const reimu)
      : Base::playerbullet(reimu, 0, -Setting::Reimu::SpellCard::Speed, 1) {}
    void SpellCard::draw() {
        Resource::SpellCard->drawC(_x, _y + Setting::Reimu::SpellCard::CenterFix);
    }
#pragma endregion

} // namespace Touhou