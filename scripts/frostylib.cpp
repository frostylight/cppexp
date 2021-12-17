#include <cstdio>
#include <cstring>
#include <memory>

#include "frostylib.hpp"

using std::make_unique;
using std::unique_ptr;


void Setup();

HWND g_hWnd               = nullptr;
const char g_wndClsName[] = "FROSTY_WND_CLASS";
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

Gdiplus::ColorMatrix _halfAlphaMatrix{
  {{1, 0, 0, 0, 0},
   {0, 1, 0, 0, 0},
   {0, 0, 1, 0, 0},
   {0, 0, 0, 0.5, 0},
   {0, 0, 0, 0, 1}}
};
Gdiplus::ImageAttributes *_halfAlpha;

//计时器
bool timerid[0xff];

//键盘记录变量
bool keyhold[0xff];


const char g_libName[]   = "FROSTYLIB";
const wchar g_libNameW[] = L"FROSTYLIB";

void ERROR_MSG(wstring str) {
    MessageBoxW(g_hWnd ? g_hWnd : nullptr, str, g_libNameW, MB_ICONERROR);
    exit(0);
}
void WARNING_MSG(wstring str) {
    MessageBoxW(g_hWnd ? g_hWnd : nullptr, str, g_libNameW, MB_ICONWARNING);
}
void ASSERT(const bool &state, wstring errstr) {
    if(!state)
        ERROR_MSG(errstr);
}

#define ASSERT_PAINT ASSERT(g_graphics &&g_bitmap, L"你应该在绘图前调用beginPaint()")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PSTR szCmdLine, [[maybe_unused]] int iCmdShow) {
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

    ASSERT(RegisterClassA(&wndclass), L"程序仅支持Windows系统");

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
            g_font         = make_unique<Gdiplus::Font>(g_fontfamily.get(), g_fontsize, g_fontstyle, Gdiplus::UnitPixel);
            g_stringformat = new Gdiplus::StringFormat(Gdiplus::StringFormat::GenericDefault());
            g_stringformat->SetAlignment(Gdiplus::StringAlignmentCenter);

            _halfAlpha = new Gdiplus::ImageAttributes();
            _halfAlpha->SetColorMatrix(&_halfAlphaMatrix);

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
            for(int i = 0; i < 0xff; ++i)
                if(timerid[i])
                    KillTimer(g_hWnd, i);
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

    void initWindow(const char *wndName, int x, int y, int width, int height) {
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

        ASSERT(g_hWnd, L"创建窗口失败");

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

    void startTimer(const byte &timerID, const uint &timeinterval, TIMERFUNC f) {
        SetTimer(g_hWnd, timerID, timeinterval, f);
        timerid[timerID] = true;
    }
    void cancelTimer(const byte &timerID) {
        KillTimer(g_hWnd, timerID);
        timerid[timerID] = false;
    }

    bool ishold(const byte &key) {
        return keyhold[key];
    }

    wchar _mcistr[300];
    void loadSound(const uint &index, wstring sound) {
        swprintf(_mcistr, L"open mpegvideo!\"%ls\" alias S%d", sound, index);
        if(mciSendStringW(_mcistr, NULL, 0, NULL)) {
            swprintf(_mcistr, L"loadSound error:%ls", sound);
            WARNING_MSG(_mcistr);
        }
    }
    void playSound(const uint &index, const bool &repeat) {
        swprintf(_mcistr, L"play S%d from 0%ls", index, repeat ? L" repeat" : L"");
        if(mciSendStringW(_mcistr, NULL, 0, NULL)) {
            swprintf(_mcistr, L"playSound error:%d", index);
            WARNING_MSG(_mcistr);
        }
    }
    void stopSound(const uint &index) {
        swprintf(_mcistr, L"stop S%d", index);
        if(mciSendStringW(_mcistr, NULL, 0, NULL)) {
            swprintf(_mcistr, L"stopSound error:%d", index);
            WARNING_MSG(_mcistr);
        }
    }
    void setVolume(const uint &index, const uint &volume) {
        if(volume > 1000 || volume < 0)
            return;
        swprintf(_mcistr, L"setaudio S%d volume to %d", index, volume);
        if(mciSendStringW(_mcistr, NULL, 0, NULL)) {
            swprintf(_mcistr, L"setVolume error:%d->%d", index, volume);
            WARNING_MSG(_mcistr);
        }
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
    void setPenWidth(const REAL &w) {
        g_pen->SetWidth(w);
    }

    void setBrushColor(const COLOR &color) {
        g_brush->SetColor(color);
    }

    void setTextFont(wstring fontname) {
        g_fontfamily.reset(new Gdiplus::FontFamily(fontname));
        g_font.reset(new Gdiplus::Font(g_fontfamily.get(), g_fontsize, g_fontstyle, Gdiplus::UnitPixel));
    }
    void setTextSize(const REAL &s) {
        g_font.reset(new Gdiplus::Font(g_fontfamily.get(), g_fontsize = s, g_fontstyle, Gdiplus::UnitPixel));
    }
    void setTextStyle(const Gdiplus::FontStyle &style) {
        g_font.reset(new Gdiplus::Font(g_fontfamily.get(), g_fontsize, g_fontstyle = style, Gdiplus::UnitPixel));
    }
    Gdiplus::Font *getFont(const Gdiplus::FontStyle &style, const REAL &s) {
        return new Gdiplus::Font(g_fontfamily.get(), s, style, Gdiplus::UnitPixel);
    }
    Gdiplus::Font *getFont(const REAL &s) {
        return new Gdiplus::Font(g_fontfamily.get(), s, g_fontstyle, Gdiplus::UnitPixel);
    }
    Gdiplus::Font *getFont(const Gdiplus::FontStyle &style) {
        return new Gdiplus::Font(g_fontfamily.get(), g_fontsize, style, Gdiplus::UnitPixel);
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

    void line(const REAL &x1, const REAL &y1, const REAL &x2, const REAL &y2, const Gdiplus::Pen *_pen) {
        ASSERT_PAINT;
        g_graphics->DrawLine(_pen ? _pen : g_pen, x1, y1, x2, y2);
    }
    void rectangle(const REAL &x, const REAL &y, const REAL &w, const REAL &h, const Gdiplus::Brush *_brush) {
        ASSERT_PAINT;
        g_graphics->FillRectangle(_brush ? _brush : g_brush, x, y, w, h);
    }

    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Font *_font, const Gdiplus::StringFormat *_format, const Gdiplus::Brush *_brush) {
        ASSERT_PAINT;
        g_graphics->DrawString(str, -1, _font, {x, y}, _format, _brush);
    }
    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Font *_font, const Gdiplus::Brush *_brush) {
        paintText(str, x, y, _font, g_stringformat, _brush);
    }
    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Font *_font) {
        paintText(str, x, y, _font, g_stringformat, g_textbrush);
    }
    void paintText(wstring str, const REAL &x, const REAL &y, const Gdiplus::Brush *_brush) {
        paintText(str, x, y, g_font.get(), g_stringformat, _brush);
    }
    void paintText(wstring str, const REAL &x, const REAL &y) {
        paintText(str, x, y, g_font.get(), g_stringformat, g_textbrush);
    }


    Gdiplus::PointF points[3];
    unique_ptr<Gdiplus::TextureBrush> texbrush;
    Img::Img(wstring filename)
      : Gdiplus::Image(filename) {
        if(this->GetLastStatus() != Gdiplus::Ok)
            throw ImageLoadException();
        _pw = GetWidth();
        _ph = GetHeight();
    }
    Img *Img::FromFile(wstring filename) {
        try {
            return new Img(filename);
        }
        catch(ImageLoadException) {
            static wchar _IMGERRORSTR[300];
            swprintf(_IMGERRORSTR, 300, L"图片加载错误:%ls", filename);
            WARNING_MSG(_IMGERRORSTR);
            return nullptr;
        }
    }
    uint Img::getw() const {
        return _pw;
    }
    uint Img::geth() const {
        return _ph;
    }
    void Img::draw(const REAL &x, const REAL &y) {
        ASSERT_PAINT;
        g_graphics->DrawImage(this, x, y);
    }
    void Img::drawC(const REAL &x, const REAL &y) {
        draw(x - (_pw >> 1), y - (_ph >> 1));
    }
    void Img::drawH(const REAL &x, const REAL &y) {
        ASSERT_PAINT;
        g_graphics->DrawImage(this, (Gdiplus::RectF){x, y, (REAL)_pw, (REAL)_ph}, 0, 0, _pw, _ph, Gdiplus::UnitPixel, _halfAlpha);
    }
    void Img::drawHC(const REAL &x, const REAL &y) {
        drawH(x - (_pw >> 1), y - (_ph >> 1));
    }
    void Img::drawFlip(const REAL &x, const REAL &y, const bool &flip) {
        if(!flip)
            return draw(x, y);
        ASSERT_PAINT;
        points[0] = {x + _pw, y};
        points[1] = {x, y};
        points[2] = {x + _pw, y + _ph};
        g_graphics->DrawImage(this, points, 3);
    }
    void Img::drawFlipH(const REAL &x, const REAL &y, const bool &flip) {
        if(!flip)
            return drawH(x, y);
        ASSERT_PAINT;
        points[0] = {x + _pw, y};
        points[1] = {x, y};
        points[2] = {x + _pw, y + _ph};
        g_graphics->DrawImage(this, points, 3, 0, 0, _pw, _ph, Gdiplus::UnitPixel, _halfAlpha);
    }
    void Img::drawFlipC(const REAL &x, const REAL &y, const bool &flip) {
        drawFlip(x - (_pw >> 1), y - (_ph >> 1), flip);
    }
    void Img::drawFlipHC(const REAL &x, const REAL &y, const bool &flip) {
        drawFlipH(x - (_pw >> 1), y - (_ph >> 1), flip);
    }
    void Img::FillRect(const REAL &x, const REAL &y, const REAL &w, const REAL &h, const REAL &sx, const REAL &sy) {
        ASSERT_PAINT;
        texbrush = make_unique<Gdiplus::TextureBrush>(this);
        texbrush->TranslateTransform(sx, sy);
        g_graphics->FillRectangle(texbrush.get(), x, y, w, h);
    }
    void Img::FillRectC(const REAL &x, const REAL &y, const REAL &w, const REAL &h, const REAL &sx, const REAL &sy) {
        FillRect(x - (_pw >> 1), y - (_ph >> 1), w, h, sx, sy);
    }


    LARGE_INTEGER StopWatch::Fre;
    StopWatch::StopWatch() {
        if(!Fre.QuadPart)
            ASSERT(QueryPerformanceFrequency(&Fre), L"按官方文档来说XP及以后的系统都是支持的...");
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
