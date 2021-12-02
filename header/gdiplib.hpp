/**
 * @file gdiplib.hpp
 * @author frostylight (810560476@qq.com)
 * @brief GDI+简单包装(C++实验用)(未完成)
 * @version 0.3.1
 * @date 2021-11-17
 *
 * @copyright Copyright (c) 2021
 */
#pragma once

#include <windows.h>
#include <gdiplus.h>

#include <array>
#include <cstdio>
#include <functional>
#include <initializer_list>

#include "pre.hpp"

using voidF        = std::function<void(void)>;
using filePathList = std::initializer_list<filePath>;
using COLOR        = Gdiplus::Color;


void Setup(void);

/**
 * @brief 初始化程序窗口
 *
 * @param title 窗口标题
 * @param left 窗口相对于屏幕左侧距离
 * @param top 窗口相对于屏幕上侧距离
 * @param width 窗口宽度
 * @param height 窗口高度
 */
void initWindow(cchar title[], int left, int top, int width, int height);

void initConsole(void);

/**
 * @brief 设置一个定时器
 *
 * @param timerID 定时器编号 (0-255)
 * @param timeinterval 时间间隔
 * @param f 调用函数
 */
void startTimer(cbyte &timerID, cint &timeinterval, voidF f);

/**
 * @brief 取消一个定时器
 *
 * @param timerID 定时器编号 (0-255)
 */
void cancelTimer(cbyte &timerID);

/**
 * @brief 按键是否被按下
 *
 * @param key Virtual-Key Codes
 */
bool ishold(cbyte &key);

void beginPaint(void);

void endPaint(void);

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

//在(x, y)处绘制字符串str，对齐方式取决于TextAlign
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Brush *_brush = nullptr);
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font);
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Brush *_brush, const Gdiplus::Font *_font);
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font, const Gdiplus::Brush *_brush);

//图片读取错误异常
struct ImageLoadException {
    //出错的图片路径
    filePath e;
    ImageLoadException(filePath p);
};

//重新包装的Image类
class FImage: public Gdiplus::Image {
    uint _w, _h;

  public:
    FImage(filePath filename);
    static FImage *FromFile(filePath filename);

    uint getw() const;
    uint geth() const;

    //以(x, y)为中心绘图
    void drawAround(cREAL &x, cREAL &y);
    //以(x, y)为中心绘制一帧，flip决定是否水平翻转图片
    void drawAroundFlip(cREAL &x, cREAL &y, cbool &flip = true);
    //以(x, y)为中心，(tx, ty)为图片偏移量，用图片平铺宽为w、高为h的矩形
    void drawCover(cREAL &x, cREAL &y, cREAL &w, cREAL &h, cREAL &tx = 0, cREAL &ty = 0);
};

/**
 * @brief 包装图片序列
 *
 * @tparam sz 图片数量
 * @tparam lasting 每张图持续帧数
 */
template<uint sz, uint lasting>
class Animation {
    std::array<FImage *, sz> img;
    uint now;

  public:
    //循环一次所需帧数
    static constexpr uint allt = sz * lasting;

    Animation();

    uint getw() const;
    uint geth() const;

    //从图片路径列表读取动画
    void load(const filePathList &plist);

    //重置播放进度
    void reset();

    //以(x, y)为中心绘制一帧
    void drawAround(cREAL &x, cREAL &y);
    //以(x, y)为中心绘制一帧，flip决定是否水平翻转图片
    void drawAroundFlip(cREAL &x, cREAL &y, cbool &flip = true);
};

/*
底下是模板类Animation的实现
目前没有找到简单好用的分离模板类定义和实现的方法
*/

template<uint sz, uint lasting>
Animation<sz, lasting>::Animation()
  : now(0) {
    img.fill(nullptr);
}
template<uint sz, uint lasting>
uint Animation<sz, lasting>::getw() const {
    return img[0] ? img[0]->getw() : 0;
}
template<uint sz, uint lasting>
uint Animation<sz, lasting>::geth() const {
    return img[0] ? img[0]->geth() : 0;
}
template<uint sz, uint lasting>
void Animation<sz, lasting>::load(const filePathList &plist) {
    uint ind = 0;
    for(auto fpath: plist) {
        img[ind++] = FImage::FromFile(fpath);
        if(ind >= sz)
            break;
    }
}
template<uint sz, uint lasting>
void Animation<sz, lasting>::reset() {
    now = 0;
}
template<uint sz, uint lasting>
void Animation<sz, lasting>::drawAround(cREAL &x, cREAL &y) {
    if(uint id = now / lasting; img.at(id))
        img[id]->drawAround(x, y);
    if(++now == allt)
        now = 0;
}
template<uint sz, uint lasting>
void Animation<sz, lasting>::drawAroundFlip(cREAL &x, cREAL &y, cbool &flip) {
    if(uint id = now / lasting; img.at(id))
        img[id]->drawAroundFlip(x, y, flip);
    if(++now == allt)
        now = 0;
}