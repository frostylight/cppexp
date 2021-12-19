#include <cstdio>

#include "GameObjectCore.hpp"
#include "GameResource.hpp"

using namespace ObjectInterface;
using namespace ObjectCore;
using namespace FROSTYLIB;
using namespace Setting;


// swprintf用wchar数组
wchar _resload[1000];
//简单宽字符串格式化函数
template<typename T>
wchar *_FormatW(const wchar *const format, const T &index) {
    swprintf(_resload, format, index);
    return _resload;
}

// IImg类图片资源加载
template<typename T>
class ImgResManager: IImg<T> {
  public:
    //资源加载
    static void load();

    //删除工具类中默认构造、赋值函数以减少源文件大小
    ImgResManager()                         = delete;
    ImgResManager(const ImgResManager &irm) = delete;
    ImgResManager &operator=(const ImgResManager &irm) = delete;
};

// IAnima类图片资源加载
template<typename T>
class AnimaResManager: IAnima<T> {
  public:
    static void load();

    AnimaResManager()                           = delete;
    AnimaResManager(const AnimaResManager &irm) = delete;
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

Gdiplus::SolidBrush *AquaBrush;
Gdiplus::Font *FpsFont;

void loadResource() {
    AnimaResManager<Reimu>::load();
    ImgResManager<SpellCard>::load();
    ImgResManager<EnemyE>::load();
    ImgResManager<RoundBullet>::load();

    Mapbg   = Img::FromFile(L"res/img/UI/Map/Background.png");
    Mapmist = Img::FromFile(L"res/img/UI/Map/mist.png");
    Tabbg   = Img::FromFile(L"res/img/UI/TabBar/Background.png");
    Tabsyb  = Img::FromFile(L"res/img/UI/TabBar/Symbol.png");

    loadSound((uint)BGM::THEME, L"res/audio/bgm/theme.wav", 150);
    loadSound((uint)BGM::STG1, L"res/audio/bgm/stg1.wav", 150);
    loadSound((uint)BGM::EMPTY + (uint)SE::PLAYERSHOT, L"res/audio/se/playershot.wav", 40);
    loadSound((uint)BGM::EMPTY + (uint)SE::PLAYERDEAD, L"res/audio/se/playerdead.wav", 30);
    loadSound((uint)BGM::EMPTY + (uint)SE::ENEMYSHOT, L"res/audio/se/enemyshot.wav", 25);
    loadSound((uint)BGM::EMPTY + (uint)SE::ENEMYDEAD, L"res/audio/se/enemydead.wav", 35);

    AquaBrush = new Gdiplus::SolidBrush(COLOR::Aqua);
    FpsFont   = getFont(15);
}

//背景偏移量
REAL BGshift = 0;
namespace Game {
    void BGupdate() {
        BGshift += MapSpeed;
        if(BGshift >= 255)
            BGshift -= 255;
    }
    void BGdraw() {
        //地图背景
        Mapbg->FillRect(0, 0, MapWidth, MapHeight, 0, BGshift);
        //背景渐变迷雾
        Mapmist->draw(0, 0);
        //记分板背景
        Tabbg->FillRect(MapWidth + 1, 0, WinWidth - MapWidth, WinHeight);
        //记分板标志
        Tabsyb->drawC(UI::SymbolX, UI::SymbolY);
        //显示FPS/生命/得分
        paintText(_FormatW(L"FPS:%.2f", _fps), WinWidth - 50, WinHeight - 20, FpsFont, AquaBrush);
        paintText(_FormatW(L"Health:%d", _health), halfMapWidth + halfWinWidth, 75);
        paintText(_FormatW(L"Score:%10d", _score), halfWinWidth + halfMapWidth, 95);
    }

    //现在播放BGM
    BGM bgmnow(BGM::EMPTY);

    void playBGM(const BGM &bgm, const bool repeat) {
        //停止上一个BGM
        if(bgmnow != bgm && bgmnow != BGM::EMPTY)
            stopSound((uint)bgmnow);
        if(bgm == BGM::EMPTY)
            return;
        playSound((uint)bgm, repeat);
    }

    void playSE(const SE &se) {
        playSound((uint)se + (uint)BGM::EMPTY);
    }

} // namespace Game