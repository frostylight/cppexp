#include <cstdio>

#include "GameObjectCore.hpp"
#include "GameResource.hpp"

using namespace ObjectInterface;
using namespace ObjectCore;
using namespace FROSTYLIB;
using namespace Setting;


wchar *_resload;
wchar *_FormatW(const wchar *const format, const uint &index) {
    swprintf(_resload, format, index);
    return _resload;
}

template<typename T>
class ImgResManager: IImg<T> {
  public:
    static void load();

    ImgResManager()                         = delete;
    ImgResManager(const ImgResManager &irm) = delete;
    ~ImgResManager()                        = delete;
    ImgResManager &operator=(const ImgResManager &irm) = delete;
};

template<typename T>
class AnimaResManager: IAnima<T> {
  public:
    static void load();

    AnimaResManager()                           = delete;
    AnimaResManager(const AnimaResManager &irm) = delete;
    ~AnimaResManager()                          = delete;
    AnimaResManager &operator=(const AnimaResManager &irm) = delete;
};


template<>
void AnimaResManager<Reimu>::load() {
    for(int i = 0; i < IAnima<Reimu>::_Icount; ++i)
        IAnima<Reimu>::_Idle[i] = Img::FromFile(_FormatW(L"res/img/Chara&Bullet/Reimu/Idle/%d.png", i));

    for(int i = 0; i < IAnima<Reimu>::_Tcount; ++i)
        IAnima<Reimu>::_Turn[i] = Img::FromFile(_FormatW(L"res/img/Chara&Bullet/Reimu/Turn/%d.png", i));

    for(int i = 0; i < IAnima<Reimu>::_LRcount; ++i)
        IAnima<Reimu>::_LeftRight[i] = Img::FromFile(_FormatW(L"res/img/Chara&Bullet/Reimu/LeftRight/%d.png", i));
}
template<>
void ImgResManager<SpellCard>::load() {
    IImg<SpellCard>::_img = Img::FromFile(L"res/img/Chara&Bullet/Reimu/SpellCard.png");
}
template<>
void ImgResManager<EnemyE>::load() {
    IImg<EnemyE>::_img = Img::FromFile(L"res/img/Chara&Bullet/EnemyE/Blue/Idle/0.png");
}
template<>
void ImgResManager<RoundBullet>::load() {
    IImg<RoundBullet>::_img = Img::FromFile(L"res/img/Chara&Bullet/EnemyE/Blue/RoundBullet.png");
}

Img *Mapbg   = nullptr;
Img *Mapmist = nullptr;
Img *Tabbg   = nullptr;
Img *Tabsyb  = nullptr;

void loadResource() {
    _resload = new wchar[10000];
    AnimaResManager<Reimu>::load();
    ImgResManager<SpellCard>::load();
    delete[] _resload;

    Mapbg   = Img::FromFile(L"res/img/UI/Map/Background.png");
    Mapmist = Img::FromFile(L"res/img/UI/Map/mist.png");
    Tabbg   = Img::FromFile(L"res/img/UI/TabBar/Background.png");
    Tabsyb  = Img::FromFile(L"res/img/UI/TabBar/Symbol.png");

    loadSound((uint)BGM::THEME, L"res/audio/bgm/theme.wav");
    loadSound((uint)BGM::STG1, L"res/audio/bgm/stg1.wav");
    loadSound((uint)BGM::EMPTY + (uint)SE::PLAYERSHOT, L"res/audio/se/playershot.wav");
    loadSound((uint)BGM::EMPTY + (uint)SE::ENEMYDEAD, L"res/audio/se/enemydead.wav");
    loadSound((uint)BGM::EMPTY + (uint)SE::PLAYERDEAD, L"res/audio/se/playerdead.wav");
}

REAL BGshift = 0;
namespace Game {
    void BGupdate() {
        BGshift += 0.3;
        if(BGshift >= 255)
            BGshift -= 255;
    }
    void BGdraw() {
        Mapbg->FillRect(0, 0, MapWidth, MapHeight, 0, BGshift);
        Mapmist->draw(0, 0);
        Tabbg->FillRect(MapWidth + 1, 0, WinWidth - MapWidth, WinHeight);
        Tabsyb->drawC(UI::SymbolX, UI::SymbolY);
    }

    BGM bgmnow(BGM::EMPTY);

    void playBGM(const BGM &bgm, const bool repeat) {
        if(bgmnow != bgm && bgmnow != BGM::EMPTY)
            stopSound((uint)bgmnow);
        if(bgm == BGM::EMPTY)
            return;
        playSound((uint)bgm, repeat);
        setVolume((uint)bgm, 150);
    }

    void playSE(const SE &se) {
        playSound((uint)se + (uint)BGM::EMPTY);
    }

} // namespace Game