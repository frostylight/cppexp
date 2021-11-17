HEADER=$(subst .cpp,.o,$(wildcard header/*.cpp))
LIB = gdi32 gdiplus

all: $(HEADER) core.o make

%.o:%.cpp
	g++ -c $^ -o $@

make:
	g++ $(HEADER) core.o $(LIB:%=-l%) -o core.exe