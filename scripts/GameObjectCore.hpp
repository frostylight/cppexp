#pragma once

#include "GameObjectInterface.hpp"


namespace ObjectCore {
    //玩家自机
    class Reimu final: public ObjectBase::player,
                       public ObjectInterface::IBox<Reimu>,
                       public ObjectInterface::IAnima<Reimu> {
      public:
        Reimu(const int &health);

        void update() override;
    };

    class SpellCard final: public ObjectBase::playerbullet,
                           public ObjectInterface::IBox<SpellCard>,
                           public ObjectInterface::IImg<SpellCard> {
      public:
    };

    class EnemyE final: public ObjectBase::enemy,
                        public ObjectInterface::IBox<EnemyE>,
                        public ObjectInterface::IImg<EnemyE> {
        REAL _sx, _sy;

      public:
    };

    class RoundBullet final: public ObjectBase::enemybullet,
                             public ObjectInterface::IBox<RoundBullet>,
                             public ObjectInterface::IImg<RoundBullet> {
      public:
    };

} // namespace ObjectCore