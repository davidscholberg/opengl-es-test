#include "engine/scene.hpp"

scene::scene(std::shared_ptr<std::list<std::shared_ptr<drawable>>> drawables, std::shared_ptr<shader_program> program)
    : drawables(drawables), program(program) {
}

void scene::draw() {
    this->program->use();

    for (const auto &d : *this->drawables) {
        d->draw();
    }

    this->program->clear();
}
