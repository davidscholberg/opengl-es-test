all: build

build:
	g++ -std=c++14 -l GLESv2 -l SDL2 -o graphics -I . main.cpp engine/*.cpp utils/*.cpp

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
