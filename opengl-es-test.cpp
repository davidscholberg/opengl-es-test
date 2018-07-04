#include <functional>
#include <iostream>
#include <unordered_map>

#include "modules/static_triangle.hpp"
#include "modules/translated_triangle.hpp"
#include "modules/rotated_square.hpp"
#include "modules/perspective_square.hpp"
#include "modules/perspective_cube.hpp"

int main(int argc, char **argv) {
    if (argc < 2 || argv[1] == "-h" || argv[1] == "--help") {
        std::cout << "usage: " << argv[0] << " <module-name> [args]" << std::endl;
        std::cout << "e.g: " << argv[0] << " static_triangle" << std::endl;
        return 0;
    }

    std::unordered_map<std::string, std::function<int(int, char**)>> function_map = {
        {static_triangle::module_name, static_triangle::run},
        {translated_triangle::module_name, translated_triangle::run},
        {rotated_square::module_name, rotated_square::run},
        {perspective_square::module_name, perspective_square::run},
        {perspective_cube::module_name, perspective_cube::run},
    };

    auto module_func = function_map.find(argv[1]);

    if (module_func == function_map.end()) {
        std::cerr << "error: couldn't find module " << argv[1] << std::endl;
        return 2;
    }

    return module_func->second(argc, argv);
}
