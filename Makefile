CPPStandard = c++20
HEADER = $(subst .cpp,.o,$(wildcard header/*.cpp))
LIB = gdi32 gdiplus

all: compile run

compile: $(HEADER) core.exe

%.o: %.cpp
	g++ $(CPPStandard:%=-std=%) -c $^ -o $@

core.exe: $(HEADER) core.o
	g++ $(CPPStandard:%=-std=%) $^ $(LIB:%=-l%) -o core.exe

run: compile
	core

clean:
	del *.o
	del header\*.o
	del core.exe

remake: clean compile