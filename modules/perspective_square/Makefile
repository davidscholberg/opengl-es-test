PROJECT_ROOT = ../..

all: build

build:
	g++ \
		-std=c++14 \
		-I ${PROJECT_ROOT} \
		main.cpp \
		${PROJECT_ROOT}/engine/*.cpp \
		${PROJECT_ROOT}/utils/*.cpp \
		$(shell sdl2-config --cflags --libs) \
		-l GLESv2 \
		-o graphics

retab:
	find ${PROJECT_ROOT} -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
