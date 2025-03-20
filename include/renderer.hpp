#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <vector>
#include <memory>

#include <particle.hpp>

class Renderer {
public:
    Renderer();

    void PrepareDraw();

private:
    std::vector<Particle> particles;

};

#endif