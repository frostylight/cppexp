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

} // namespace ObjectCore