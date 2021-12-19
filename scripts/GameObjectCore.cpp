#include "GameObjectCore.hpp"
#include "GameSetting.hpp"

using namespace FROSTYLIB;
using namespace ObjectInterface;
using namespace ObjectBase;
using namespace Setting;

using std::max;
template<std::integral T, std::floating_point U>
inline U max(const T &x, const U &y) {
    return x > y ? x : y;
}
template<std::integral T, std::floating_point U>
inline U max(const U &x, const T &y) {
    return x > y ? x : y;
}
using std::min;
template<std::integral T, std::floating_point U>
inline U min(const T &x, const U &y) {
    return x < y ? x : y;
}
template<std::integral T, std::floating_point U>
inline U min(const U &x, const T &y) {
    return x < y ? x : y;
}

//-距离计算临时变量
REAL dx, dy, dis;

namespace ObjectCore {
    //-Reimu
    Reimu::Reimu(const int &health): object(SpwanX, SpwanY), player(health, ReimuSpeed) {}
    void Reimu::update() {
        //保存状态
        _last = _state;
        //初始化状态
        _state = IDLE;
        //如果只按下上下键之一
        if(ishold(VK_UP) ^ ishold(VK_DOWN)) {
            //上
            if(ishold(VK_UP))
                _y = max(IBox<Reimu>::_halfboxheight, _y - _speed);
            //下
            else
                _y = min(MapHeight - IBox<Reimu>::_halfboxheight, _y + _speed);
        }
        //只按下左右键之一
        if(ishold(VK_LEFT) ^ ishold(VK_RIGHT)) {
            //按下左
            if(ishold(VK_LEFT)) {
                //记录状态
                _state |= LEFT;
                _x = max(IBox<Reimu>::_halfboxwidth, _x - _speed);
            }
            //按下右
            else {
                //记录状态
                _state |= RIGHT;
                _x = min(MapWidth - IBox<Reimu>::_halfboxwidth, _x + _speed);
            }
        }
        //如果按下攻击键,计算CD
        if(ishold(Key::AttackKey))
            Ishot<Reimu>::update();
        //更新动画状态
        IAnima<Reimu>::update();
    }
    void Reimu::respwan() {
        _x = SpwanX;
        _y = SpwanY;
    }

    //-EnemyE
    EnemyE::EnemyE(const REAL &x, const REAL &y, const REAL &sx, const REAL &sy, const REAL &lx, const REAL &ly): object(x, y), enemy(1), _sx(sx), _sy(sy), _lx(lx), _ly(ly), _arr(false) {}
    void EnemyE::update() {
        //如果没到达目标点
        if(!_arr) {
            dx  = _sx - _x;
            dy  = _sy - _y;
            dis = sqrt(dx * dx + dy * dy);
            _x += dx * (0.005 + EnemyESpeed / dis);
            _y += dy * (0.005 + EnemyESpeed / dis);
            //判断是否到达目标点
            if(abs(_x - _sx) <= IBox<EnemyE>::_halfboxwidth && abs(_y - _sy) <= IBox<EnemyE>::_halfboxheight)
                _arr = true;
        }
        //前往离开点
        else {
            dx  = _lx - _x;
            dy  = _ly - _y;
            dis = sqrt(dx * dx + dy * dy);
            _x += dx * (0.0025 + EnemyESpeed / dis);
            _y += dy * (0.0025 + EnemyESpeed / dis);
        }
        //更新发射CD
        Ishot<EnemyE>::update();
    }
    bool EnemyE::inMap(const REAL &width, const REAL &height) const {
        //未到达目标点时不因位置被移除
        return _arr ? object::inMap(width, height) : true;
    }

    //-SpellCard
    SpellCard::SpellCard(const REAL &x, const REAL &y, const REAL &speed, const uint &damage): object(x, y), playerbullet(0, -speed, damage) {}
    SpellCard *SpellCard::fromReimu(const Reimu *const reimu) {
        return new SpellCard(reimu->_x, reimu->_y, SpellCardSpeed, 1);
    }
    void SpellCard::draw() {
        //根据拖尾特效调整绘制位置
        IImg<SpellCard>::_img->drawC(_x, _y + SpellCardFix);
    }

    //-RoundBullet
    RoundBullet::RoundBullet(const REAL &x, const REAL &y, const REAL &dx, const REAL &dy): object(x, y), enemybullet(dx, dy, 1){};
    RoundBullet *RoundBullet::fromEnemyE(const EnemyE *const enemye, const Reimu *const reimu) {
        dx  = reimu->_x - enemye->_x;
        dy  = reimu->_y - enemye->_y;
        dis = std::sqrt(dx * dx + dy * dy);
        //发射方向瞄准自机
        return new RoundBullet(enemye->_x, enemye->_y, RoundSpeed * dx / dis, RoundSpeed * dy / dis);
    }

} // namespace ObjectCore