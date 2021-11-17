HEADER=$(subst .cpp,.o,$(wildcard header/*.cpp))
LIB = gdi32 gdiplus

all: $(HEADER) core.o make

%.o:%.cpp
	g++ -std=c++17 -c $^ -o $@

make:
	g++ -std=c++17 $(HEADER) core.o $(LIB:%=-l%) -o core.exe