#include <exception>
#include <iostream>
#include <unordered_map>

#include "engine/engine.hpp"
#include "modules/rotated_square.hpp"
#include "modules/static_triangle.hpp"
#include "modules/translated_triangle.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "error: need module arg" << std::endl;
        return 1;
    }

    std::unordered_map<std::string,void(*)()> module_map;
    module_map["rotated_square"] = rotated_square;
    module_map["static_triangle"] = static_triangle;
    module_map["translated_triangle"] = translated_triangle;

    auto it = module_map.find(argv[1]);
    if (it == module_map.end()) {
        std::cerr << "error: invalid module arg" << std::endl;
        return 1;
    }

    auto module_function = it->second;

    try {
        engine e;
        module_function();
    } catch (const std::exception& exc) {
        std::cerr << "error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
