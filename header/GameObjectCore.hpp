#pragma once

#include "GameObjectInterface.hpp"


namespace ObjectCore {
    //玩家自机
    class Reimu final: public ObjectBase::player,
                       public ObjectInterface::IBox<Reimu>,
                       public ObjectInterface::IAnima<Reimu> {
        REAL _speed;

      public:
        Reimu(const int &health);

        void update() override;
    };

    class EnemyE final: public ObjectBase::enemy,
                        public ObjectInterface::IBox<EnemyE>,
                        public ObjectInterface::IImg<EnemyE> {
        REAL _sx, _sy;

      public:
    };

} // namespace ObjectCore