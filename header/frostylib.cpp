#include"frostylib.hpp"

void Setup();

//定义窗口所用变量

cchar g_wndClassName[] = "FROSTY_WND_CLASS";
cchar g_libName[] = "FROSTYLIB";
HINSTANCE g_hInstance;

//窗口句柄
HWND g_hWnd = nullptr;

//初始化GDI+所用变量

ULONG_PTR m_pGdiToken;
Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;

//双缓冲绘图所用变量

Gdiplus::Bitmap *g_bitmap = nullptr;
Gdiplus::CachedBitmap *g_cachebmp = nullptr;
Gdiplus::Graphics *g_graphics = nullptr;
Gdiplus::Graphics *g_hdcgp = nullptr;

//绘图所用变量

Gdiplus::Pen *g_pen = nullptr;
Gdiplus::SolidBrush *g_brush = nullptr;
Gdiplus::FontFamily *g_fontfamily = nullptr;
Gdiplus::Font *g_font = nullptr;
Gdiplus::SolidBrush *g_textbrush = nullptr;
Gdiplus::StringFormat *g_stringformat = nullptr;

//计时器变量
voidF timer[0xff];

//键盘记录变量
bool keyhold[0xff];

//事件处理函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


#define ASSERT(_Expression,errStr)  (!!(_Expression)) || (ASSERT_ERROR(errStr), 0)
#define ASSERT_HWND ASSERT(g_hWnd, "You should call initWindow(...) first")
#define ASSERT_PAINT ASSERT(g_graphics && g_bitmap, "You should call beginPaint() first")
void ASSERT_ERROR(cchar *errStr){
    MessageBoxA(g_hWnd, errStr, g_libName, MB_ICONERROR);
    exit(0);
}

//定义窗口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
    MSG msg;
    WNDCLASSA wndclass;

    g_hInstance = hInstance;

    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = g_wndClassName;

    if (!RegisterClassA(&wndclass)){
        MessageBoxA(NULL, "This program requires Windows NT!", g_libName, MB_ICONERROR);
        return 0;
    }

    Setup();

    ASSERT(g_hWnd, "You must call \"initWindow(...)\" in Setup()");

    while (GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message){
        case WM_CREATE:{
            //初始化GDI+及相关变量

            Gdiplus::GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, nullptr);
            g_pen = new Gdiplus::Pen(COLOR::Aqua);
            g_brush = new Gdiplus::SolidBrush(COLOR::Aqua);
            g_fontfamily = new Gdiplus::FontFamily(L"楷体");
            g_font = new Gdiplus::Font(g_fontfamily, 10, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
            g_textbrush = new Gdiplus::SolidBrush(COLOR::Black);
            g_stringformat = new Gdiplus::StringFormat(Gdiplus::StringFormat::GenericDefault());
            g_stringformat->SetAlignment(Gdiplus::StringAlignmentCenter);
            break;
        }

        case WM_ERASEBKGND:
            break;

        case WM_PAINT:{
            //双缓冲实现

            HDC hdc;
            PAINTSTRUCT ps;
            hdc = BeginPaint(hwnd, &ps);
            if (g_bitmap){
                g_hdcgp = Gdiplus::Graphics::FromHDC(hdc);
                g_cachebmp = new Gdiplus::CachedBitmap(g_bitmap, g_hdcgp);
                g_hdcgp->DrawCachedBitmap(g_cachebmp, 0, 0);
                delete g_cachebmp;
                delete g_hdcgp;
            }
            EndPaint(hwnd, &ps);
            if (g_bitmap)delete g_bitmap;
            g_bitmap = nullptr;
            if (g_graphics)delete g_graphics;
            g_graphics = nullptr;
            break;
        }

        case WM_KEYDOWN:{
            keyhold[(byte)wParam] = true;
            break;
        }

        case WM_KEYUP:{
            keyhold[(byte)wParam] = false;
            break;
        }

        case WM_TIMER:{
            if (timer[(byte)wParam])
                timer[(byte)wParam]();
            break;
        }

        case WM_DESTROY:{
            Gdiplus::GdiplusShutdown(m_pGdiToken);
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void initWindow(cchar *wndName, int x, int y, int width, int height){
    RECT rect;

    ASSERT(!g_hWnd, "Don't call initWindow twice");


    if (x == DEFAULT || y == DEFAULT)
        x = y = CW_USEDEFAULT;

    g_hWnd = CreateWindowA(
        g_wndClassName, wndName,
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
        x, y,
        width, height,
        NULL, NULL, 0, NULL);

    if (!g_hWnd){
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
void initConsole(){
    AllocConsole();
    freopen("CONIN$", "r+t", stdin);
    freopen("CONOUT$", "w+t", stdout);
}

void startTimer(cbyte &timerID, cint &timeinterval, voidF f){
    SetTimer(g_hWnd, timerID, timeinterval, NULL);
    timer[timerID] = f;
}
void cancelTimer(cbyte &timerID){
    KillTimer(g_hWnd, timerID);
    timer[timerID] = nullptr;
}

bool ishold(cbyte &key){ return keyhold[key]; }

void beginPaint(){
    ASSERT_HWND;
    ASSERT(!g_bitmap, "Don't call beginPaint() twice");
    static RECT rc;
    GetClientRect(g_hWnd, &rc);
    g_bitmap = new Gdiplus::Bitmap(int(rc.right), int(rc.bottom));
    g_graphics = Gdiplus::Graphics::FromImage(g_bitmap);
    g_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
}
void endPaint(){
    ASSERT_PAINT;
    InvalidateRect(g_hWnd, 0, 0);
}

void setPenColor(const COLOR &color){ g_pen->SetColor(color); }
void setPenWidth(cREAL &w){ g_pen->SetWidth(w); }
void setBrushColor(const COLOR &color){ g_brush->SetColor(color); }

void line(cREAL &x1, cREAL &y1, cREAL &x2, cREAL &y2){
    ASSERT_PAINT;
    g_graphics->DrawLine(g_pen, x1, y1, x2, y2);
}
void rectangle(cREAL &x, cREAL &y, cREAL &w, cREAL &h){
    ASSERT_PAINT;
    g_graphics->FillRectangle(g_brush, x, y, w, h);
}

FImage::FImage(const WCHAR *filename):Gdiplus::Image(filename){ w = GetWidth();h = GetHeight(); }
FImage *FImage::FromFile(const WCHAR *filename){ return new FImage(filename); }


void FImage::drawAround(cREAL &x, cREAL &y){
    ASSERT_PAINT;
    g_graphics->DrawImage(this, x - (w >> 1), y - (h >> 1), (REAL)w, (REAL)h);
}

Gdiplus::PointF points[3];
void FImage::drawAroundFlip(cREAL &x, cREAL &y, cbool &flip){
    ASSERT_PAINT;
    if (!flip)drawAround(x, y);
    else{
        points[0] = { x + (w >> 1), y - (h >> 1) };
        points[1] = { x - (w >> 1), y - (h >> 1) };
        points[2] = { x + (w >> 1), y + (h >> 1) };
        g_graphics->DrawImage(this, points, 3);
    }
}
