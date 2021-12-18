#pragma once

enum class BGM {
    THEME,
    STG1,
    EMPTY
};
enum class SE {
    PLAYERSHOT,
    PLAYERDEAD,
    ENEMYSHOT,
    ENEMYDEAD
};

//加载图片相关资源
void loadResource();


namespace Game {
    //更新背景
    void BGupdate();
    //绘制背景
    void BGdraw();

    //播放背景音乐
    void playBGM(const BGM &bgm, const bool repeat = true);

    //播放音效
    void playSE(const SE &se);

}; // namespace Game