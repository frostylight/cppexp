/**
 * @file frostylib.hpp
 * @author frostylight (810560476@qq.com)
 * @brief GDI+简单包装(C++实验用)(未完成)
 * @version 0.3.1
 * @date 2021-11-17
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once

#include<windows.h>
#include<gdiplus.h>

#include<functional>
#include<initializer_list>
#include<array>

#include"pre.hpp"

using voidF = std::function<void(void)>;
using filePathList = std::initializer_list<filePath>;
using COLOR = Gdiplus::Color;


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

/**
 * @brief 初始化控制台
 */
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

/**
 * @brief 初始化绘图
 */
void beginPaint(void);

/**
 * @brief 结束绘图
 */
void endPaint(void);

/**
 * @param color RGB/RGBA
 */
void setPenColor(const COLOR &color);

/**
 * @param w 宽度
 */
void setPenWidth(cREAL &w);

/**
 * @param color RGB/RGBA
 */
void setBrushColor(const COLOR &color);

/**
 * @brief 绘制一条穿过(x1, y1)和(x2, y2)的直线
 */
void line(cREAL &x1, cREAL &y1, cREAL &x2, cREAL &y2);

/**
 * @brief 以(x, y)为左上角绘制矩形
 *
 * @param w 矩形宽度
 * @param h 矩形高度
 */
void rectangle(cREAL &x, cREAL &y, cREAL &w, cREAL &h);


/**
 * @brief 图片读取错误异常
 */
struct ImageLoadException{
    //出错的图片路径
    filePath e;
    ImageLoadException(filePath p);
};

/**
 * @brief 重新包装的Image类
 */
class FImage :public Gdiplus::Image{
    uint w, h;

    public:
    FImage(filePath filename);
    static FImage *FromFile(filePath filename);

    /**
     * @brief 禁用赋值
     */
    auto operator=(const FImage &o) = delete;

    /**
     * @brief 以(x, y)为中心绘图
     */
    void drawAround(cREAL &x, cREAL &y);
    /**
     * @brief 以(x, y)为中心绘图
     *
     * @param flip 是否水平翻转图片
     */
    void drawAroundFlip(cREAL &x, cREAL &y, cbool &flip = true);
};

/**
 * @brief 包装图片序列
 *
 * @tparam sz 图片数量
 * @tparam lasting 每张图持续帧数
 */
template<uint sz, uint lasting>
class Animation{
    std::array<FImage *, sz>img;
    uint now;

    public:
    /**
     * @brief 循环一次所需帧数
     */
    static constexpr uint allt = sz * lasting;

    Animation();

    /**
     * @brief 禁用赋值
     */
    auto operator=(const Animation &o) = delete;

    /**
     * @brief 加载图片序列
     *
     * @param plist 图片文件路径列表
     */
    void load(const filePathList &plist);

    void reset();

    /**
     * @brief 以(x, y)为中心绘制一帧
     */
    void drawAround(cREAL &x, cREAL &y);
    /**
     * @brief 以(x, y)为中心绘制一帧
     *
     * @param flip 是否水平翻转图片
     */
    void drawAroundFlip(cREAL &x, cREAL &y, cbool &flip = true);
};

/*
底下是模板类Animation的实现
目前没有找到简单好用的分离模板类定义和实现的方法
*/

template<uint sz, uint lasting>
Animation<sz, lasting>::Animation():now(0){ img.fill(nullptr); }

template<uint sz, uint lasting>
void Animation<sz, lasting>::load(const filePathList &plist){
    uint ind = 0;
    for (auto fpath : plist){
        try{
            img[ind++] = FImage::FromFile(fpath);
        }
        catch (ImageLoadException &e){
            img[ind - 1] = nullptr;
            std::wprintf(L"ImageLoadError:%ls\n", e.e);
        }
        if (ind >= sz)break;
    }
}

template<uint sz, uint lasting>
void Animation<sz, lasting>::reset(){ now = 0; }

template<uint sz, uint lasting>
void Animation<sz, lasting>::drawAround(cREAL &x, cREAL &y){
    if (uint id = now / lasting;img.at(id))
        img[id]->drawAround(x, y);
    if (++now == allt)now = 0;
}

template<uint sz, uint lasting>
void Animation<sz, lasting>::drawAroundFlip(cREAL &x, cREAL &y, cbool &flip){
    if (uint id = now / lasting;img.at(id))
        img[id]->drawAroundFlip(x, y, flip);
    if (++now == allt)now = 0;
}