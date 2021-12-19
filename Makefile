Compiler = g++
CompileParam = -std=c++20 -Wall
LIB = gdi32 gdiplus winmm

Scripts = scripts
src=$(subst scripts,$(CacheDir),$(subst .cpp,.o,$(wildcard scripts/*.cpp)))

export Compiler CompileParam

all : core.exe

compile:
	@mingw32-make --no-print-directory -C $(Scripts)

core.exe : compile
	$(Compiler) $(CompileParam) $(Scripts)/*.o $(LIB:%=-l%) -o core.exe

clean:
	@del $(Scripts)\*.o
	@del $(Scripts)\*.d
	@del core.exe


