#pragma once

#include<functional>
#include<initializer_list>

#define DEFAULT -1

typedef float REAL;
typedef wchar_t WCHAR;

typedef unsigned int uint;
typedef unsigned char byte;

typedef const int cint;
typedef const char cchar;
typedef const bool cbool;

typedef const byte cbyte;
typedef const REAL cREAL;
typedef const uint cuit;

typedef const WCHAR *filePath;

typedef std::function<void(void)> voidF;
typedef std::initializer_list<const WCHAR *> filePathList;


template<typename T>
inline T abs(const T &x){ return x >= 0 ? x : -x; }