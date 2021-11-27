CPPStandard = c++20
HEADER = $(subst .cpp,.o,$(wildcard header/*.cpp))
LIB = gdi32 gdiplus


all: $(HEADER) core.exe run

%.o: %.cpp
	g++ $(CPPStandard:%=-std=%) -c $^ -o $@

core.exe: $(HEADER) core.o
	g++ $(CPPStandard:%=-std=%) $^ $(LIB:%=-l%) -o core.exe

run:
	core

clean:
	del *.o
	del header\*.o
	del core.exe

remake: clean all