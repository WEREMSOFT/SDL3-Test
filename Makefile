SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL_LIBS   := $(shell pkg-config --libs sdl3)

EMCC = em++
WEB_FLAGS = -std=c++14 -O2 -sALLOW_MEMORY_GROWTH=1 --preload-file Assets
WEB_OUT = web/index.html

CXX = g++
CXXFLAGS = -std=c++14 -g $(SDL_CFLAGS)

build: clean
	g++ $(CXXFLAGS) main.cpp -o main.bin $(SDL_LIBS)

run_main: build
	./main.bin

build_web:
	mkdir -p web
	$(EMCC) main.cpp libSDL3.a $(WEB_FLAGS) -o $(WEB_OUT)

run_web: build_web
	emrun $(WEB_OUT)

clean:
	rm -f $(NATIVE_BIN)
	rm -rf web
