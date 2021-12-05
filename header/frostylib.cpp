#include <cstdio>
#include <cstring>
#include <memory>

#include "frostylib.hpp"

using std::make_unique;
using std::unique_ptr;


void Setup();

HWND g_hWnd          = nullptr;
cchar g_wndClsName[] = "FROSTY_WND_CLASS";
RECT rect;


ULONG_PTR m_pGdiToken;
Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;


unique_ptr<Gdiplus::Bitmap> g_bitmap;
unique_ptr<Gdiplus::CachedBitmap> g_cachebmp;
unique_ptr<Gdiplus::Graphics> g_graphics;
unique_ptr<Gdiplus::Graphics> g_hdcgp;


Gdiplus::Pen *g_pen                   = nullptr;
Gdiplus::SolidBrush *g_brush          = nullptr;
Gdiplus::SolidBrush *g_textbrush      = nullptr;
Gdiplus::StringFormat *g_stringformat = nullptr;
unique_ptr<Gdiplus::FontFamily> g_fontfamily;
unique_ptr<Gdiplus::Font> g_font;
REAL g_fontsize                = 15;
Gdiplus::FontStyle g_fontstyle = Gdiplus::FontStyleRegular;
Gdiplus::Unit g_fontunit       = Gdiplus::UnitPixel;

//计时器序号
uint timerid[0xff];

//键盘记录变量
bool keyhold[0xff];


cchar g_libName[]        = "FROSTYLIB";
const WCHAR g_libNameW[] = L"FROSTYLIB";

void ERROR_MSG(cchar *str) {
    MessageBoxA(g_hWnd, str, g_libName, MB_ICONERROR);
    exit(0);
}
void ERROR_MSG(WString str) {
    MessageBoxW(g_hWnd, str, g_libNameW, MB_ICONERROR);
    exit(0);
}
void WARNING_MSG(cchar *str) {
    MessageBoxA(g_hWnd, str, g_libName, MB_ICONWARNING);
}
void WARNING_MSG(WString str) {
    MessageBoxW(g_hWnd, str, g_libNameW, MB_ICONWARNING);
}

#define ASSERT(_Expression, str) (!!(_Expression)) || (ERROR_MSG(str), 0)
#define ASSERT_PAINT ASSERT(g_graphics &&g_bitmap, L"你应该在绘图前调用beginPaint()")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    MSG msg;
    WNDCLASSA wndclass;

    wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = g_wndClsName;

    if(!RegisterClassA(&wndclass)) {
        MessageBoxA(NULL, "This program requires Windows NT!", g_libName, MB_ICONERROR);
        return 0;
    }

    Setup();

    ASSERT(g_hWnd, L"你应该在Setup()中调用initWindow()");

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_CREATE: {
            Gdiplus::GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, nullptr);
            g_pen          = new Gdiplus::Pen(COLOR::Aqua);
            g_brush        = new Gdiplus::SolidBrush(COLOR::Aqua);
            g_textbrush    = new Gdiplus::SolidBrush(COLOR::Black);
            g_fontfamily   = make_unique<Gdiplus::FontFamily>(L"Consolas");
            g_font         = make_unique<Gdiplus::Font>(g_fontfamily.get(), g_fontsize, g_fontstyle, g_fontunit);
            g_stringformat = new Gdiplus::StringFormat(Gdiplus::StringFormat::GenericDefault());
            g_stringformat->SetAlignment(Gdiplus::StringAlignmentCenter);

            memset(timerid, 0, sizeof timerid);
            break;
        }

        case WM_PAINT: {
            HDC hdc;
            PAINTSTRUCT ps;
            hdc = BeginPaint(hwnd, &ps);
            if(g_bitmap) {
                g_hdcgp    = make_unique<Gdiplus::Graphics>(hdc);
                g_cachebmp = make_unique<Gdiplus::CachedBitmap>(g_bitmap.get(), g_hdcgp.get());
                g_hdcgp->DrawCachedBitmap(g_cachebmp.get(), 0, 0);
            }
            EndPaint(hwnd, &ps);
            g_bitmap.reset();
            g_graphics.reset();

            break;
        }

        case WM_KEYDOWN: {
            keyhold[(byte)wParam] = true;
            break;
        }

        case WM_KEYUP: {
            keyhold[(byte)wParam] = false;
            break;
        }

        case WM_DESTROY: {
            for(uint id: timerid)
                if(id)
                    timeKillEvent(id);
            Gdiplus::GdiplusShutdown(m_pGdiToken);
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

namespace FROSTYLIB {

    void initWindow(cchar *wndName, int x, int y, int width, int height) {
        if(g_hWnd) {
            WARNING_MSG(L"不要调用两次initWindow()");
            return;
        }

        if(x == DEFAULT || y == DEFAULT)
            x = y = CW_USEDEFAULT;

        g_hWnd = CreateWindowA(
          g_wndClsName, wndName,
          WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
          x, y,
          width, height,
          NULL, NULL, 0, NULL);

        if(!g_hWnd) {
            MessageBoxW(NULL, L"创建窗口失败", g_libNameW, MB_ICONERROR);
            exit(0);
        }
        GetClientRect(g_hWnd, &rect);
        width += width - (rect.right - rect.left);
        height += height - (rect.bottom - rect.top);
        SetWindowPos(g_hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSENDCHANGING);

        ShowWindow(g_hWnd, 1);
        UpdateWindow(g_hWnd);
    }
    void initConsole() {
        AllocConsole();
        freopen("CONIN$", "r+t", stdin);
        freopen("CONOUT$", "w+t", stdout);
    }


    void startTimer(cbyte &timerID, cuint &timeinterval, TIMERFUNC f) {
        // SetTimer(g_hWnd, timerID, timeinterval, f);
        timerid[timerID] = timeSetEvent(timeinterval, 0, f, 0, TIME_KILL_SYNCHRONOUS | TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
    }
    void cancelTimer(cbyte &timerID) {
        // KillTimer(g_hWnd, timerID);
        timeKillEvent(timerid[timerID]);
        timerid[timerID] = 0;
    }

    bool ishold(cbyte &key) {
        return keyhold[key];
    }

    void beginPaint() {
        ASSERT(g_hWnd, L"你应该先调用initWindow()");
        ASSERT(!g_bitmap, L"不要连续调用两次beginPaint()");
        GetClientRect(g_hWnd, &rect);
        g_bitmap = make_unique<Gdiplus::Bitmap>(int(rect.right), int(rect.bottom));
        g_graphics.reset(Gdiplus::Graphics::FromImage(g_bitmap.get()));
        g_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    }
    void endPaint() {
        ASSERT_PAINT;
        InvalidateRect(g_hWnd, 0, false);
    }


    void setPenColor(const COLOR &color) {
        g_pen->SetColor(color);
    }
    void setPenWidth(cREAL &w) {
        g_pen->SetWidth(w);
    }


    void setBrushColor(const COLOR &color) {
        g_brush->SetColor(color);
    }


    void setTextFont(WString fontname) {
        g_fontfamily.reset(new Gdiplus::FontFamily(fontname));
        g_font.reset(new Gdiplus::Font(g_fontfamily.get(), g_fontsize, g_fontstyle, g_fontunit));
    }
    void setTextSize(cREAL &s) {
        g_font.reset(new Gdiplus::Font(g_fontfamily.get(), g_fontsize = s, g_fontstyle, g_fontunit));
    }
    void setTextStyle(const Gdiplus::FontStyle &style) {
        g_font.reset(new Gdiplus::Font(g_fontfamily.get(), g_fontsize, g_fontstyle = style, g_fontunit));
    }
    void setTextColor(const COLOR &color) {
        g_textbrush->SetColor(color);
    }
    void setTextAlign(const Gdiplus::StringAlignment &align) {
        g_stringformat->SetAlignment(align);
    }
    void setTextLineAlign(const Gdiplus::StringAlignment &align) {
        g_stringformat->SetLineAlignment(align);
    }


    void line(cREAL &x1, cREAL &y1, cREAL &x2, cREAL &y2, const Gdiplus::Pen *_pen) {
        ASSERT_PAINT;
        g_graphics->DrawLine(_pen ? _pen : g_pen, x1, y1, x2, y2);
    }
    void rectangle(cREAL &x, cREAL &y, cREAL &w, cREAL &h, const Gdiplus::Brush *_brush) {
        ASSERT_PAINT;
        g_graphics->FillRectangle(_brush ? _brush : g_brush, x, y, w, h);
    }


    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font, const Gdiplus::StringFormat *_format, const Gdiplus::Brush *_brush) {
        ASSERT_PAINT;
        g_graphics->DrawString(str, -1, _font, {x, y}, _format, _brush);
    }
    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font, const Gdiplus::Brush *_brush) {
        paintText(str, x, y, _font, g_stringformat, _brush);
    }
    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font) {
        paintText(str, x, y, _font, g_stringformat, g_textbrush);
    }
    void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Brush *_brush) {
        paintText(str, x, y, g_font.get(), g_stringformat, _brush);
    }
    void paintText(WString str, cREAL &x, cREAL &y) {
        paintText(str, x, y, g_font.get(), g_stringformat, g_textbrush);
    }


    Gdiplus::PointF points[3];
    unique_ptr<Gdiplus::TextureBrush> texbrush;

    Img::Img(filePath filename)
      : Gdiplus::Image(filename) {
        if(this->GetLastStatus() != Gdiplus::Ok)
            throw ImageLoadException();
        pw = GetWidth();
        ph = GetHeight();
    }
    Img *Img::FromFile(filePath filename) {
        try {
            return new Img(filename);
        }
        catch(ImageLoadException) {
            wprintf(L"ImageLoadError:%ls\n", filename);
            return nullptr;
        }
    }
    uint Img::getw() const {
        return pw;
    }
    uint Img::geth() const {
        return ph;
    }
    void Img::draw(cREAL &x, cREAL &y) {
        ASSERT_PAINT;
        g_graphics->DrawImage(this, x, y);
    }
    void Img::drawC(cREAL &x, cREAL &y) {
        draw(x - (pw >> 1), y - (ph >> 1));
    }
    void Img::drawFlip(cREAL &x, cREAL &y, cbool &flip) {
        if(!flip)
            return draw(x, y);
        ASSERT_PAINT;
        points[0] = {x + pw, y};
        points[1] = {x, y};
        points[2] = {x + pw, y + ph};
        g_graphics->DrawImage(this, points, 3);
    }
    void Img::drawFlipC(cREAL &x, cREAL &y, cbool &flip) {
        drawFlip(x - (pw >> 1), y - (ph >> 1), flip);
    }
    void Img::FillRect(cREAL &x, cREAL &y, cREAL &w, cREAL &h, cREAL &sx, cREAL &sy) {
        ASSERT_PAINT;
        texbrush = make_unique<Gdiplus::TextureBrush>(this);
        texbrush->TranslateTransform(sx, sy);
        g_graphics->FillRectangle(texbrush.get(), x, y, w, h);
        g_graphics->GetLastStatus() == Gdiplus::Ok;
    }
    void Img::FillRectC(cREAL &x, cREAL &y, cREAL &w, cREAL &h, cREAL &sx, cREAL &sy) {
        FillRect(x - (pw >> 1), y - (ph >> 1), w, h, sx, sy);
    }

    ImgList::ImgList()
      : count(0), interval(0), now(0), imgindex(0), imglist(nullptr) {}
    ImgList::ImgList(const filePathList &pathlist, cuint &duration)
      : count(pathlist.size()), interval(duration), imgindex(0), now(0) {
        imglist = new Img *[count];
        for(int i = 0; auto path: pathlist)
            imglist[i++] = Img::FromFile(path);
    }
    void ImgList::reload(const filePathList &pathlist, cuint &duration) {
        if(count) {
            for(int i = 0; i < count; ++i)
                delete imglist[i];
            delete[] imglist;
        }
        count    = pathlist.size();
        interval = duration;
        imgindex = now = 0;
    }
    uint ImgList::getTotalTime() const {
        return count * interval;
    }
    void ImgList::restart() {
        imgindex = now = 0;
    }
    void ImgList::next() {
        if(++now == interval) {
            if(++imgindex == count)
                imgindex = 0;
            now = 0;
        }
    }
    void ImgList::draw(cREAL &x, cREAL &y) {
        imglist[imgindex]->draw(x, y);
        next();
    }
    void ImgList::drawC(cREAL &x, cREAL &y) {
        imglist[imgindex]->drawC(x, y);
        next();
    }
    void ImgList::drawFlip(cREAL &x, cREAL &y, cbool &flip) {
        imglist[imgindex]->drawFlip(x, y);
        next();
    }
    void ImgList::drawFlipC(cREAL &x, cREAL &y, cbool &flip) {
        imglist[imgindex]->drawFlipC(x, y);
        next();
    }

    StopWatch::StopWatch() {
        if(QueryPerformanceFrequency(&Fre) == 0)
            throw L"按官方文档来说XP及以后的系统都是支持的...";
        st.QuadPart = ed.QuadPart = 0;
    }
    void StopWatch::start() {
        st.QuadPart = ed.QuadPart = 0;
        QueryPerformanceCounter(&st);
    }
    void StopWatch::stop() {
        if(!st.QuadPart)
            ed = st;
        else
            QueryPerformanceCounter(&ed);
    }
    void StopWatch::restart() {
        start();
    }
    double StopWatch::getsecond() {
        if(ed.QuadPart)
            return 1.0 * (ed.QuadPart - st.QuadPart) / Fre.QuadPart;
        else {
            LARGE_INTEGER temp;
            QueryPerformanceCounter(&temp);
            return 1.0 * (temp.QuadPart - st.QuadPart) / Fre.QuadPart;
        }
    }
} // namespace FROSTYLIB
