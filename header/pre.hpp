#pragma once

constexpr int DEFAULT = -1;

using REAL  = float;
using WCHAR = wchar_t;

using uint = unsigned int;
using byte = unsigned char;

using cint  = const int;
using cchar = const char;
using cbool = const bool;

using cbyte = const byte;
using cREAL = const REAL;
using cuit  = const uint;

using filePath = const WCHAR *;

template<typename T>
inline T abs(const T &x) {
    return x >= 0 ? x : -x;
}