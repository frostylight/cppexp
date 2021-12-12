#pragma once

#include "GameObjectBase.hpp"


namespace ObjectInterface {
    //碰撞箱接口
    template<typename T>
    class IBox: virtual public ObjectBase::object {
      public:
        //碰撞检测
        template<typename U>
        inline bool collide(const IBox<U> *const obj) const {
            return 2 * abs(_x - obj->_x) - T::_boxwidth - U::_boxwidth <= Realpcs &&
                   2 * abs(_y - obj->_y) - T::_boxheight - U::_boxheight <= Realpcs;
        }
        inline bool collide(const IBox<T> *const obj) const {
            return abs(_x - obj->_x) - T::_boxwidth <= Realpcs &&
                   abs(_y - obj->_y) - T::_boxheight <= Realpcs;
        }
        IBox() {}
    };
} // namespace ObjectInterface