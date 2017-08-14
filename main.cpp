#include <exception>
#include <iostream>

#include "engine/engine.hpp"

int main(int argv, char** argc) {
    try {
        engine e;
        e.loop();
    } catch (const std::exception& exc) {
        std::cerr << "error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
