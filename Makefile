CPPStandard = c++20
HEADER = $(subst .cpp,.o,$(wildcard header/*.cpp))
LIB = gdi32 gdiplus winmm

all: compile run

compile: core.exe

remake: clean compile

run: compile
	core

%.o: %.cpp %.hpp header/stdpre.hpp
	g++ $(CPPStandard:%=-std=%) -Wall -c $< -o $@

core.o: core.cpp header/stdpre.hpp
	g++ $(CPPStandard:%=-std=%) -Wall -c $< -o $@

core.exe: $(HEADER) core.o
	g++ $(CPPStandard:%=-std=%) -Wall $^ $(LIB:%=-l%) -o core.exe

clean:
	del *.o
	del header\*.o
	del core.exe

