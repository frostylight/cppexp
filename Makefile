#编译器
Compiler = g++
#编译选项
CompileParam = -std=c++20 -static -O3
#链接库
LIB = gdi32 gdiplus winmm
#源代码路径
Scripts = scripts
src=$(subst scripts,$(CacheDir),$(subst .cpp,.o,$(wildcard scripts/*.cpp)))

export Compiler CompileParam

all : core.exe

#编译.o文件
compile:
	@mingw32-make --no-print-directory -C $(Scripts)

#链接生成exe文件
core.exe : compile
	$(Compiler) $(CompileParam) $(Scripts)/*.o $(LIB:%=-l%) -o core.exe

#清理缓存文件
cleancache:
	@del $(Scripts)\*.o
	@del $(Scripts)\*.d

#清理缓存文件并删除exe文件
clean : cleancache
	@del core.exe


