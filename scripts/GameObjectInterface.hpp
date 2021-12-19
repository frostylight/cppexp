#pragma once

#include <cmath>

#include "frostylib.hpp"
#include "GameObjectBase.hpp"
#include "GameSetting.hpp"

using std::abs;


namespace ObjectInterface {
    //碰撞箱接口
    template<typename T>
    class IBox: virtual public ObjectBase::object {
      protected:
        IBox(): ObjectBase::object(0, 0) {}
        //实际object构造由最后派生类决定,此处占位

      public:
        //碰撞箱宽/高度
        static const uint _boxwidth, _boxheight;
        //碰撞箱宽/高度的一半
        static const uint _halfboxwidth, _halfboxheight;
        //此类静态成员应在GameSetting.cpp中定义并赋值

        //碰撞检测
        template<typename U>
        inline bool collide(const IBox<U> *const obj) const {
            return abs(_x - obj->getx()) <= IBox<T>::_halfboxwidth + IBox<U>::_halfboxwidth && abs(_y - obj->gety()) <= IBox<T>::_halfboxheight + IBox<U>::_halfboxheight;
        }
        inline bool collide(const IBox<T> *const obj) const {
            return abs(_x - obj->_x) <= IBox<T>::_boxwidth && abs(_y - obj->_y) <= IBox<T>::_boxheight;
        }
    };

    template<typename T>
    class Ishot: virtual public ObjectBase::object {
      protected:
        Ishot(): ObjectBase::object(0, 0), _shotcount(0) {}
        // 实际object构造由最后派生类决定,此处占位
        uint _shotcount;

      public:
        static const uint _shotCD;
        //此类静态成员应在GameSetting.cpp中定义并赋值

        virtual void update() override {
            if(++_shotcount == Ishot<T>::_shotCD)
                _shotcount = 0;
        }

        virtual bool ready() const {
            return !_shotcount;
        }
    };

    //静态图片接口
    template<typename T>
    class IImg: virtual public ObjectBase::object {
      protected:
        //图片
        static FROSTYLIB::Img *_img;
        //此类静态成员应在GameResource.cpp中赋值

        IImg(): ObjectBase::object(0, 0) {}
        //实际object构造由最后派生类决定,此处占位

      public:
        //绘图
        virtual void draw() override {
            _img->drawC(_x, _y);
        }
    };
    template<typename T>
    FROSTYLIB::Img *IImg<T>::_img(nullptr);

    enum { IDLE = 0b00, LEFT = 0b10, RIGHT = 0b01 };
    //动画接口
    template<typename T>
    class IAnima: virtual public ObjectBase::object {
      public:
        //待机,转身,左/右转图片数量
        static constexpr byte _Icount = 4, _Tcount = 3, _LRcount = 4;
        //每张图片持续帧数
        static constexpr byte _Itime = 8, _Ttime = 6, _LRtime = 8;

      protected:
        //图片序列
        static FROSTYLIB::Img *_Idle[_Icount], *_Turn[_Tcount], *_LeftRight[_LRcount];
        //此类静态成员应在GameResource.cpp中赋值

        //当前图片序号,时间计数
        byte _count, _time;

        //当前状态
        byte _state, _last;

        IAnima(): ObjectBase::object(0, 0), _count(0), _time(0), _state(0), _last(0) {}
        //实际object构造由最后派生类决定,此处占位

      public:
        //总播放时间
        static const uint _alltime = _Icount * _Itime + _Tcount * _Ttime + _LRcount * _LRtime;

        //更新动画
        virtual void update() override {
            if(_state != _last) //重置动画
                _count = _time = 0;
            else {
                ++_time;
                if(!_state) { //待机
                    if(_time >= _Itime) {
                        ++_count;
                        _time = 0;
                        if(_count >= _Icount) //循环待机动画
                            _count = 0;
                    }
                }
                else { //向左/右
                    if(_count < _Tcount) { //转身动画
                        if(_time >= _Ttime) {
                            ++_count;
                            _time = 0;
                        }
                    }
                    else { //向左/右动画
                        if(_time >= _LRtime) {
                            ++_count;
                            _time = 0;
                            if(_count >= _Tcount + _LRcount) //循环向左/右动画
                                _count = _Tcount;
                        }
                    }
                }
            }
        }

        //绘制动画
        virtual void draw() override {
            if(!_state) //待机
                _Idle[_count]->drawC(_x, _y);
            else { //向左/右
                if(_count < _Tcount) //转身动画
                    _Turn[_count]->drawFlipC(_x, _y, _state & RIGHT);
                else //向左/右动画
                    _LeftRight[_count - _Tcount]->drawFlipC(_x, _y, _state & RIGHT);
            }
        }
    };
    template<typename T>
    FROSTYLIB::Img *IAnima<T>::_Idle[IAnima<T>::_Icount]{nullptr};
    template<typename T>
    FROSTYLIB::Img *IAnima<T>::_Turn[IAnima<T>::_Tcount]{nullptr};
    template<typename T>
    FROSTYLIB::Img *IAnima<T>::_LeftRight[IAnima<T>::_LRcount]{nullptr};

} // namespace ObjectInterface