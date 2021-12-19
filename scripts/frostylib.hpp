/**
 * @file frostylib.hpp
 * @author frostylight
 * @brief 仿ACLlib(C++实验用)(未完成)
 * @version 0.5.5
 * @date 2021-12-5
 *
 * @copyright Copyright (c) 2021
 */
#pragma once

#include <windows.h>
#include <gdiplus.h>

#include "stdpre.hpp"

#define UNUSED_TIMER_PARAM [[maybe_unused]] HWND UNUSED0, [[maybe_unused]] UINT UNUSED1, [[maybe_unused]] UINT_PTR UNUSED2, [[maybe_unused]] DWORD UNUSED3
#define TIMER(x) void CALLBACK x(UNUSED_TIMER_PARAM)
#define CALLTIMER(x) x(0, 0, 0, 0)

constexpr int DEFAULT = -1;

using COLOR     = Gdiplus::Color;
using TIMERFUNC = TIMERPROC;


void ERROR_MSG(wstring str);
void WARNING_MSG(wstring str);

void Setup(void);

namespace FROSTYLIB {
    /**
     * @brief 初始化程序窗口
     *
     * @param title 窗口标题
     * @param left 窗口相对于屏幕左侧距离
     * @param top 窗口相对于屏幕上侧距离
     * @param width 窗口宽度
     * @param height 窗口高度
     */
    void initWindow(const char *title, int left, int top, int width, int height);
    void initConsole(void);

    //设置一个编号为timerID的计时器,每过timeinterval毫秒调用一次f
    void startTimer(const byte &timerID, const uint &timeinterval, TIMERFUNC f);

    //取消编号为timerID的定时器
    void cancelTimer(const byte &timerID);

    //判断按键key(VK-Code)是否被按下
    bool ishold(const byte &key);

    void loadSound(const uint &index, wstring sound);
    void loadSound(const uint &index, wstring sound, const uint &volume);
    void playSound(const uint &index, const bool &repeat = false);
    void stopSound(const uint &index);
    void setVolume(const uint &index, const uint &volume);

    void beginPaint();
    void endPaint();

    void setPenColor(const COLOR &color);
    void setPenWidth(const REAL &w);

    void setBrushColor(const COLOR &color);

    void setTextFont(wstring fontname);
    void setTextSize(const REAL &s);
    void setTextStyle(const Gdiplus::FontStyle &style);
    [[nodiscard]] Gdiplus::Font *getFont(const Gdiplus::FontStyle &style, const REAL &s);
    [[nodiscard]] Gdiplus::Font *getFont(const REAL &s);
    [[nodiscard]] Gdiplus::Font *getFont(const Gdiplus::FontStyle &style);
    void setTextColor(const COLOR &color);
    void setTextAlign(const Gdiplus::StringAlignment &align);
    void setTextLineAlign(const Gdiplus::StringAlignment &align);

    //绘制一条穿过(x1, y1), (x2, y2)的直线
    void line(const REAL &x1, const REAL &y1, const REAL &x2, const REAL &y2, const Gdiplus::Pen *_pen = nullptr);

    //以(x, y)为左上角,w为宽、h为高填充矩形
    void rectangle(const REAL &x, const REAL &y, const REAL &w, const REAL &h, const Gdiplus::Brush *_brush = nullptr);

    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Font *_font, const Gdiplus::StringFormat *_format, const Gdiplus::Brush *_brush);
    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Font *_font, const Gdiplus::Brush *_brush);
    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Font *_font);
    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Brush *_brush);
    //在(x, y)处绘制字符串str
    void paintText(wstring str, const REAL &x, const REAL &y);

    //图片读取错误异常
    struct ImageLoadException {};

    //重封装的图片类
    class Img: public Gdiplus::Image {
        uint _pw, _ph;

        Img(wstring filename);

      public:
        //从文件加载图片
        [[nodiscard]] static Img *FromFile(wstring filename);

        inline uint getw() const;
        inline uint geth() const;


        //以(x, y)为左上角绘图
        void draw(const REAL &x, const REAL &y);
        //以(x, y)为左上角绘图, 透明度50%
        void drawH(const REAL &x, const REAL &y);
        //以(x, y)为中心绘图
        void drawC(const REAL &x, const REAL &y);
        //以(x, y)为中心绘图, 透明度50%
        void drawHC(const REAL &x, const REAL &y);
        //以(x, y)为左上角绘图,flip决定是否水平翻转
        void drawFlip(const REAL &x, const REAL &y, const bool &flip = true);
        //以(x, y)为左上角绘图,透明度50%,flip决定是否水平翻转
        void drawFlipH(const REAL &x, const REAL &y, const bool &flip = true);
        //以(x, y)为中心绘图,flip决定是否水平翻转
        void drawFlipC(const REAL &x, const REAL &y, const bool &flip = true);
        //以(x, y)为中心绘图,透明度50%,flip决定是否水平翻转
        void drawFlipHC(const REAL &x, const REAL &y, const bool &flip = true);
        //以(x, y)为左上角,(sx, sy)为图片偏移量,平铺宽为w、高为h的矩形
        void FillRect(const REAL &x, const REAL &y, const REAL &w, const REAL &h, const REAL &sx = 0, const REAL &sy = 0);
        //以(x, y)为中心,(sx, sy)为图片偏移量,平铺宽为w、高为h的矩形
        void FillRectC(const REAL &x, const REAL &y, const REAL &w, const REAL &h, const REAL &sx = 0, const REAL &sy = 0);
    };

    //封装过后的高精度计时器
    class StopWatch {
        static LARGE_INTEGER Fre;

        LARGE_INTEGER st, ed;

      public:
        StopWatch();
        void start();
        void stop();
        void restart();
        double getsecond();
    };

} // namespace FROSTYLIB
