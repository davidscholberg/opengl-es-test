#include <functional>
#include <iostream>
#include <string>
#include <map>

#include "modules/movable_square.hpp"
#include "modules/perspective_cube.hpp"
#include "modules/perspective_square.hpp"
#include "modules/rotated_square.hpp"
#include "modules/static_triangle.hpp"
#include "modules/translated_triangle.hpp"

typedef std::map<std::string, std::function<int(int, char**)>> str_to_func_map;

std::string format_module_names(const str_to_func_map &function_map) {
    std::string module_names;
    for (const auto &element : function_map) {
        module_names += "    " + element.first + "\n";
    }
    return module_names;
}

int main(int argc, char **argv) {
    str_to_func_map function_map = {
        {movable_square::module_name, movable_square::run},
        {perspective_cube::module_name, perspective_cube::run},
        {perspective_square::module_name, perspective_square::run},
        {rotated_square::module_name, rotated_square::run},
        {static_triangle::module_name, static_triangle::run},
        {translated_triangle::module_name, translated_triangle::run},
    };

    std::string help_short_opt("-h");
    std::string help_long_opt("--help");
    if (argc < 2 || argv[1] == help_short_opt || argv[1] == help_long_opt) {
        std::cout << "usage: " << argv[0] << " <module-name> [args]" << std::endl;
        std::cout << "available modules:" << std::endl;
        std::cout << format_module_names(function_map);
        return 0;
    }

    auto module_func = function_map.find(argv[1]);

    if (module_func == function_map.end()) {
        std::cerr << "error: couldn't find module " << argv[1] << std::endl;
        return 2;
    }

    return module_func->second(argc, argv);
}
