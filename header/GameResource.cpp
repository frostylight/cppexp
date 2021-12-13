#include <cstdio>

#include "GameObjectCore.hpp"
#include "GameResource.hpp"

using namespace ObjectInterface;
using namespace ObjectCore;
using namespace FROSTYLIB;


template<typename T>
class ResourceManager: IAnima<T> {
  public:
    static void load();
};

wchar *_resload;
wchar *_FormatW(const wchar *const format, const uint &index) {
    swprintf(_resload, format, index);
    return _resload;
}

template<typename T>
Img *IAnima<T>::_Idle[IAnima<T>::_Icount]{nullptr};
template<typename T>
Img *IAnima<T>::_Turn[IAnima<T>::_Tcount]{nullptr};
template<typename T>
Img *IAnima<T>::_LeftRight[IAnima<T>::_LRcount]{nullptr};

template<>
void ResourceManager<Reimu>::load() {
    for(int i = 0; i < IAnima<Reimu>::_Icount; ++i)
        IAnima<Reimu>::_Idle[i] = Img::FromFile(_FormatW(L"res/img/Chara&Bullet/Reimu/Idle/%d.png", i));

    for(int i = 0; i < IAnima<Reimu>::_Tcount; ++i)
        IAnima<Reimu>::_Turn[i] = Img::FromFile(_FormatW(L"res/img/Chara&Bullet/Reimu/Turn/%d.png", i));

    for(int i = 0; i < IAnima<Reimu>::_LRcount; ++i)
        IAnima<Reimu>::_LeftRight[i] = Img::FromFile(_FormatW(L"res/img/Chara&Bullet/Reimu/LeftRight/%d.png", i));
}

void loadResource() {
    _resload = new wchar[10000];
    ResourceManager<Reimu>::load();
    delete[] _resload;
}