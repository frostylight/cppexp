#pragma once

#include "GameBase.hpp"
#include "GameSetting.hpp"


namespace Touhou {
    class Reimu: public Base::player {
        bool _direction;
        uint _onturn;

      protected:
        uint getw() const override;
        uint geth() const override;
        uint getbulletCD() const override;

      public:
        Reimu(cint &health);
        void respawn();

        void update() override;
        void draw() override;
    } reimu;

    class SpellCard: public Base::playerbullet {
      protected:
        uint getw() const override;
        uint geth() const override;

      public:
        SpellCard(const Reimu *const reimu);

        void draw() override;
    };

} // namespace Touhou