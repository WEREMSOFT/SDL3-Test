SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL_LIBS   := $(shell pkg-config --libs sdl3)

CXX = g++
CXXFLAGS = -std=c++14 -g $(SDL_CFLAGS)

build:
	g++ $(CXXFLAGS) main.cpp -o main.bin $(SDL_LIBS)

run_main: build
	./main.bin

clean:
	rm main.bin
