#include "GameCore.hpp"
#include "GameResource.hpp"

using FROSTYLIB::ishold;
using namespace Setting::Game;

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
      : Base::player(SpawnX, SpawnY,
                     health, Setting::Reimu::Speed),
        _direction(0), _onturn(0) {}
    void Reimu::respawn() {
        _x         = SpawnX;
        _y         = SpawnY;
        _direction = 0;
        _onturn    = 0;
    }
    SpellCard *Reimu::shot() {
        return new SpellCard(this);
    }
    void Reimu::update() {
        if(ishold(VK_UP))
            _y = max(_y - _speed, geth() >> 1);
        if(ishold(VK_DOWN))
            _y = min(_y + _speed, MapHeight - (geth() >> 1));
        if(ishold(VK_LEFT))
            _x = max(_x - _speed, getw() >> 1);
        if(ishold(VK_RIGHT))
            _x = min(_x + _speed, MapWidth - (getw() >> 1));
        if(ishold('Z'))
            Base::chara::update();
    }
    void Reimu::draw() {
        if(ishold(VK_LEFT) ^ ishold(VK_RIGHT)) {
            if(ishold(VK_RIGHT) ^ _direction) {
                _direction = !_direction;
                _onturn    = 0;
                Resource::Reimu_LR.restart();
                Resource::Reimu_Turn.restart();
            }
            if(++_onturn <= Resource::Reimu_Turn.getTotalTime()) {
                Resource::Reimu_Turn.drawFlipC(_x, _y, _direction);
                Resource::Reimu_Idle.restart();
            }
            else {
                Resource::Reimu_LR.drawFlipC(_x, _y, _direction);
                Resource::Reimu_Turn.restart();
            }
        }
        else {
            _onturn = 0;
            Resource::Reimu_Idle.drawFlipC(_x, _y, _direction);
            Resource::Reimu_LR.restart();
            Resource::Reimu_Turn.restart();
        }
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
    SpellCard *SpellCard::FromReimu(const Reimu *const reimu) {
        return new SpellCard(reimu);
    }
    void SpellCard::draw() {
        Resource::SpellCard->drawC(_x, _y + Setting::Reimu::SpellCard::CenterFix);
    }
    bool SpellCard::active() const {
        return object::active() && inscreen(MapWidth, MapHeight);
    }
#pragma endregion

} // namespace Touhou