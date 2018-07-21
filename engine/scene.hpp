#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <list>
#include <memory>

#include "engine/drawable.hpp"
#include "engine/shader_program.hpp"

class scene {
protected:
    std::shared_ptr<std::list<std::shared_ptr<drawable>>> drawables;
    std::shared_ptr<shader_program> program;
public:
    scene(std::shared_ptr<std::list<std::shared_ptr<drawable>>> drawables, std::shared_ptr<shader_program> program);
    scene(scene const &) = delete;
    void operator=(scene const &) = delete;
    void draw();
};

#endif // SCENE_HPP_
