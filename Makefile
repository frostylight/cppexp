Compiler = g++
CompileParam = -std=c++20 -Wall
LIB = gdi32 gdiplus winmm

TopDir = $(CURDIR)
CacheDir = $(TopDir)/cache
Scripts = scripts
src=$(subst scripts,$(CacheDir),$(subst .cpp,.o,$(wildcard scripts/*.cpp)))

export CacheDir Compiler CompileParam

all : checkcache core.exe

checkcache :
	@if not exist $(CacheDir) mkdir $(CacheDir)

compile:
	@mingw32-make --no-print-directory -C scripts

core.exe : compile
	$(Compiler) $(CompileParam) $(CacheDir)/*.o $(LIB:%=-l%) -o core.exe



