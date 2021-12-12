#pragma once

using sbyte     = signed char;
using ushort    = unsigned short;
using ulong     = unsigned long;
using ulonglong = unsigned long long;
using uint      = unsigned int;

using REAL = float;

using wchar = wchar_t;

using wstring = const wchar *const;

//浮点精度
constexpr REAL Realpcs = 1e-4;

//绝对值模板
template<typename T>
inline T abs(const T &x) {
    return x > 0 ? x : -x;
}