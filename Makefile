CXX=g++
CXXFLAGS= -std=c++17 -O2 -Wall -Iinclude
TARGET=rasterizer
SRCS=$(wildcard src/*.cpp)

UNAME :=$(shell uname)

; it is checking the shell
ifeq ($(UNAME),Darwin)
	SDL_FLAGS=$(shell sdl2-config --cflags --libs)
else
	SDL_FLAGS=$(shell sdl2-config --cflags --libs)
endif

$(TARGET):$(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SDL_FLAGS)

clean:
	rm -f $(TARGET)
run all:
	./$(TARGET)

