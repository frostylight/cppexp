#pragma once

#include "GameBase.hpp"
#include "GameSetting.hpp"


namespace Touhou {
    class Reimu final: public Base::player {
        bool _direction;
        uint _onturn;

      protected:
        uint getw() const override;
        uint geth() const override;
        uint getbulletCD() const override;

      public:
        Reimu(cint &health);
        void respawn();

        SpellCard *shot();

        void update() override;
        void draw() override;
    };

    class SpellCard final: public Base::playerbullet {
      protected:
        uint getw() const override;
        uint geth() const override;

      public:
        SpellCard(const Reimu *const reimu);
        SpellCard *FromReimu(const Reimu *const reimu);

        void draw() override;

        bool active() const override;
    };

} // namespace Touhou