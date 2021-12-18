#pragma once

#include "GameObjectInterface.hpp"


namespace ObjectCore {
    //玩家自机
    class Reimu final: public ObjectBase::player,
                       public ObjectInterface::IBox<Reimu>,
                       public ObjectInterface::IAnima<Reimu>,
                       public ObjectInterface::Ishot<Reimu> {
        friend class SpellCard;
        friend class RoundBullet;

      public:
        Reimu(const int &health);

        void update() override;
    };

    class SpellCard final: public ObjectBase::playerbullet,
                           public ObjectInterface::IBox<SpellCard>,
                           public ObjectInterface::IImg<SpellCard> {
      protected:
        SpellCard(const REAL &x, const REAL &y, const REAL &speed, const uint &damage);

      public:
        void draw() override;

        [[nodiscard]] static SpellCard *fromReimu(const Reimu *const reimu);
    };

    class EnemyE final: public ObjectBase::enemy,
                        public ObjectInterface::IBox<EnemyE>,
                        public ObjectInterface::IImg<EnemyE>,
                        public ObjectInterface::Ishot<EnemyE> {
        friend class RoundBullet;
        REAL _sx, _sy;

      public:
    };

    class RoundBullet final: public ObjectBase::enemybullet,
                             public ObjectInterface::IBox<RoundBullet>,
                             public ObjectInterface::IImg<RoundBullet> {
      protected:
        RoundBullet(const REAL &x, const REAL &y, const REAL &dx, const REAL &dy);

      public:
        [[nodiscard]] static RoundBullet *fromEnemyE(const EnemyE *const enemye, const Reimu *const reimu);
    };

} // namespace ObjectCore