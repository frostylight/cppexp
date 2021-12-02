#include <memory>

#include "gdiplib.hpp"

using std::make_unique;
using std::unique_ptr;


void Setup();

//定义窗口所用变量

cchar g_wndClassName[] = "FROSTY_WND_CLASS";
cchar g_libName[]      = "GDIPLIB";

HINSTANCE g_hInstance;

//窗口句柄
HWND g_hWnd = nullptr;

//初始化GDI+所用变量

ULONG_PTR m_pGdiToken;
Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;

//双缓冲绘图所用变量

unique_ptr<Gdiplus::Bitmap> g_bitmap;
unique_ptr<Gdiplus::CachedBitmap> g_cachebmp;
unique_ptr<Gdiplus::Graphics> g_graphics;
unique_ptr<Gdiplus::Graphics> g_hdcgp;

//绘图所用变量

Gdiplus::Pen *g_pen                   = nullptr;
Gdiplus::SolidBrush *g_brush          = nullptr;
Gdiplus::SolidBrush *g_textbrush      = nullptr;
Gdiplus::StringFormat *g_stringformat = nullptr;
unique_ptr<Gdiplus::FontFamily> g_fontfamily;
unique_ptr<Gdiplus::Font> g_font;
REAL g_fontsize                = 15;
Gdiplus::FontStyle g_fontstyle = Gdiplus::FontStyleRegular;
Gdiplus::Unit g_fontunit       = Gdiplus::UnitPixel;


//计时器变量
voidF timer[0xff];

//键盘记录变量
bool keyhold[0xff];

//事件处理函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


#define ASSERT(_Expression, errStr) (!!(_Expression)) || (ASSERT_ERROR(errStr), 0)
#define ASSERT_HWND ASSERT(g_hWnd, "You should call initWindow(...) first")
#define ASSERT_PAINT ASSERT(g_graphics &&g_bitmap, "You should call beginPaint() first")
void ASSERT_ERROR(cchar *errStr) {
    MessageBoxA(g_hWnd, errStr, g_libName, MB_ICONERROR);
    exit(0);
}

//定义窗口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    MSG msg;
    WNDCLASSA wndclass;

    g_hInstance = hInstance;

    wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = g_wndClassName;

    if(!RegisterClassA(&wndclass)) {
        MessageBoxA(NULL, "This program requires Windows NT!", g_libName, MB_ICONERROR);
        return 0;
    }

    Setup();

    ASSERT(g_hWnd, "You must call \"initWindow(...)\" in Setup()");

    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_CREATE: {
            //初始化GDI+及相关变量

            Gdiplus::GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, nullptr);
            g_pen          = new Gdiplus::Pen(COLOR::Aqua);
            g_brush        = new Gdiplus::SolidBrush(COLOR::Aqua);
            g_textbrush    = new Gdiplus::SolidBrush(COLOR::Black);
            g_fontfamily   = make_unique<Gdiplus::FontFamily>(L"Consolas");
            g_font         = make_unique<Gdiplus::Font>(g_fontfamily.get(), g_fontsize, g_fontstyle, g_fontunit);
            g_stringformat = new Gdiplus::StringFormat(Gdiplus::StringFormat::GenericDefault());
            g_stringformat->SetAlignment(Gdiplus::StringAlignmentCenter);
            break;
        }

        case WM_ERASEBKGND:
            break;

        case WM_PAINT: {
            //双缓冲实现

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

        case WM_TIMER: {
            if(timer[(byte)wParam])
                timer[(byte)wParam]();
            break;
        }

        case WM_DESTROY: {
            Gdiplus::GdiplusShutdown(m_pGdiToken);
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void initWindow(cchar *wndName, int x, int y, int width, int height) {
    RECT rect;

    ASSERT(!g_hWnd, "Don't call initWindow twice");

    if(x == DEFAULT || y == DEFAULT)
        x = y = CW_USEDEFAULT;

    g_hWnd = CreateWindowA(
      g_wndClassName, wndName,
      WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
      x, y,
      width, height,
      NULL, NULL, 0, NULL);

    if(!g_hWnd) {
        MessageBoxA(NULL, "Fail to create window", g_libName, MB_ICONERROR);
        exit(0);
    }
    GetClientRect(g_hWnd, &rect);
    width += width - (rect.right - rect.left);
    height += height - (rect.bottom - rect.top);
    SetWindowPos(g_hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);

    ShowWindow(g_hWnd, 1);
    UpdateWindow(g_hWnd);
}
void initConsole() {
    AllocConsole();
    freopen("CONIN$", "r+t", stdin);
    freopen("CONOUT$", "w+t", stdout);
}

void startTimer(cbyte &timerID, cint &timeinterval, voidF f) {
    SetTimer(g_hWnd, timerID, timeinterval, NULL);
    timer[timerID] = f;
}
void cancelTimer(cbyte &timerID) {
    KillTimer(g_hWnd, timerID);
    timer[timerID] = nullptr;
}

bool ishold(cbyte &key) {
    return keyhold[key];
}

void beginPaint() {
    ASSERT_HWND;
    ASSERT(!g_bitmap, "Don't call beginPaint() twice");
    static RECT rc;
    GetClientRect(g_hWnd, &rc);
    g_bitmap = make_unique<Gdiplus::Bitmap>(int(rc.right), int(rc.bottom));
    g_graphics.reset(Gdiplus::Graphics::FromImage(g_bitmap.get()));
    g_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
}
void endPaint() {
    ASSERT_PAINT;
    InvalidateRect(g_hWnd, 0, 0);
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
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Brush *_brush) {
    ASSERT_PAINT;
    g_graphics->DrawString(str, -1, g_font.get(), {x, y}, g_stringformat, _brush ? _brush : g_textbrush);
}
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font) {
    ASSERT_PAINT;
    g_graphics->DrawString(str, -1, _font, {x, y}, g_stringformat, g_textbrush);
}
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Brush *_brush, const Gdiplus::Font *_font) {
    ASSERT_PAINT;
    g_graphics->DrawString(str, -1, _font, {x, y}, g_stringformat, _brush);
}
void paintText(WString str, cREAL &x, cREAL &y, const Gdiplus::Font *_font, const Gdiplus::Brush *_brush) {
    paintText(str, x, y, _brush, _font);
}

ImageLoadException::ImageLoadException(filePath p)
  : e(p) {}

FImage::FImage(filePath filename)
  : Gdiplus::Image(filename) {
    if(this->GetLastStatus() != Gdiplus::Ok)
        throw ImageLoadException(filename);
    _w = GetWidth();
    _h = GetHeight();
}
FImage *FImage::FromFile(filePath filename) {
    try {
        return new FImage(filename);
    }
    catch(ImageLoadException &e) {
        wprintf(L"ImageLoadError:%ls\n", e.e);
        return nullptr;
    }
}
uint FImage::getw() const {
    return _w;
}
uint FImage::geth() const {
    return _h;
}
void FImage::drawAround(cREAL &x, cREAL &y) {
    ASSERT_PAINT;
    g_graphics->DrawImage(this, x - (_w >> 1), y - (_h >> 1), (REAL)_w, (REAL)_h);
}
Gdiplus::PointF points[3];
void FImage::drawAroundFlip(cREAL &x, cREAL &y, cbool &flip) {
    ASSERT_PAINT;
    if(!flip)
        drawAround(x, y);
    else {
        points[0] = {x + (_w >> 1), y - (_h >> 1)};
        points[1] = {x - (_w >> 1), y - (_h >> 1)};
        points[2] = {x + (_w >> 1), y + (_h >> 1)};
        g_graphics->DrawImage(this, points, 3);
    }
}
unique_ptr<Gdiplus::TextureBrush> texbrush;
void FImage::drawCover(cREAL &x, cREAL &y, cREAL &w, cREAL &h, cREAL &tx, cREAL &ty) {
    texbrush = make_unique<Gdiplus::TextureBrush>(this);
    texbrush->TranslateTransform(tx, ty);
    g_graphics->FillRectangle(texbrush.get(), x - (w / 2), y - (h / 2), w, h);
}
