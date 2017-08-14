all: build

build:
	g++ -std=c++14 -l SDL2 -o graphics-test -I . main.cpp engine/*.cpp

retab:
	find . -name '*.[ch]pp' -type f -exec bash -c 'expand -i -t 4 "{}" > ./not_a_filename && mv ./not_a_filename "{}"' \;
