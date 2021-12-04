/**
 * @file frostylib.hpp
 * @author frostylight (810560476@qq.com)
 * @brief acllib改造(C++实验用)(未完成)
 * @version 0.5.1
 * @date 2021-12-5
 *
 * @copyright Copyright (c) 2021
 */
#pragma once

#include <windows.h>
#include <gdiplus.h>

#include <initializer_list>

#include "pre.hpp"

#define UNUSED_TIMER_PARAM UINT __UNUSED0, UINT __UNUSED1, DWORD_PTR __UNUSED2, DWORD_PTR __UNUSED3, DWORD_PTR __UNUSED4

using filePathList = std::initializer_list<filePath>;
using COLOR        = Gdiplus::Color;


void ERROR_MSG(cchar *str);
void ERROR_MSG(WString str);
void WARNING_MSG(cchar *str);
void WARNING_MSG(WString str);

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
    void initWindow(cchar *title, int left, int top, int width, int height);
    void initConsole(void);

    using TIMERFUNC = LPTIMECALLBACK;

    //设置一个编号为timerID的计时器，每过timeinterval毫秒调用一次f
    void startTimer(cbyte &timerID, cuint &timeinterval, TIMERFUNC f);

    //取消编号为timerID的定时器
    void cancelTimer(cbyte &timerID);

    //判断按键key(VK-Code)是否被按下
    bool ishold(cbyte &key);

    void beginPaint();
    void endPaint();

    void setPenColor(const COLOR &color);
    void setPenWidth(cREAL &w);

    void setBrushColor(const COLOR &color);

    void setTextFont(WString fontname);
    void setTextSize(cREAL &s);
    void setTextStyle(const Gdiplus::FontStyle &style);
    void setTextColor(const COLOR &color);
    void setTextAlign(const Gdiplus::StringAlignment &align);
    void setTextLineAlign(const Gdiplus::StringAlignment &align);

    //绘制一条穿过(x1, y1), (x2, y2)的直线
    void line(cREAL &x1, cREAL &y1, cREAL &x2, cREAL &y2, const Gdiplus::Pen *_pen = nullptr);

    //以(x, y)为左上角，w为宽、h为高填充矩形
    void rectangle(cREAL &x, cREAL &y, cREAL &w, cREAL &h, const Gdiplus::Brush *_brush = nullptr);

    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font, const Gdiplus::StringFormat *_format, const Gdiplus::Brush *_brush);
    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font, const Gdiplus::Brush *_brush);
    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font);
    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Brush *_brush);
    //在(x, y)处绘制字符串str
    void paintText(WString str, cREAL &x, cREAL &y);

    //图片读取错误异常
    struct ImageLoadException {};

    //重封装的图片类
    class Img: public Gdiplus::Image {
        uint pw, ph;

        Img(filePath filename);

      public:
        static Img *FromFile(filePath filename);

        inline uint getw() const;
        inline uint geth() const;

        //以(x, y)为左上角绘图
        void draw(cREAL &x, cREAL &y);
        //以(x, y)为中心绘图
        void drawC(cREAL &x, cREAL &y);
        //以(x, y)为左上角绘图，flip决定是否水平翻转
        void drawFlip(cREAL &x, cREAL &y, cbool &flip = true);
        //以(x, y)为中心绘图，flip决定是否水平翻转
        void drawFlipC(cREAL &x, cREAL &y, cbool &flip = true);
        //以(x, y)为左上角，(sx, sy)为图片偏移量，平铺宽为w、高为h的矩形
        void FillRect(cREAL &x, cREAL &y, cREAL &w, cREAL &h, cREAL &sx = 0, cREAL &sy = 0);
        //以(x, y)为中心，(sx, sy)为图片偏移量，平铺宽为w、高为h的矩形
        void FillRectC(cREAL &x, cREAL &y, cREAL &w, cREAL &h, cREAL &sx = 0, cREAL &sy = 0);
    };

    //图片序列
    class ImgList {
        uint count, interval;
        uint imgindex, now;
        Img **imglist;

        inline void next();

      public:
        ImgList();
        ImgList(const filePathList &pathlist, cuint &duration);

        void reload(const filePathList &pathlist, cuint &duration);

        inline uint getTotalTime() const;

        //重置进度
        void restart();
        //以(x, y)为左上角绘制一帧
        void draw(cREAL &x, cREAL &y);
        //以(x, y)为中心绘制一帧
        void drawC(cREAL &x, cREAL &y);
        //以(x, y)为左上角绘制一帧，flip决定是否水平翻转
        void drawFlip(cREAL &x, cREAL &y, cbool &flip = true);
        //以(x, y)为中心绘制一帧，flip决定是否水平翻转
        void drawFlipC(cREAL &x, cREAL &y, cbool &flip = true);
    };

    //封装过后的高精度计时器
    class StopWatch {
        LARGE_INTEGER Fre, st, ed;

      public:
        StopWatch();
        void start();
        void stop();
        void restart();
        double getsecond();
    };
} // namespace FROSTYLIB
