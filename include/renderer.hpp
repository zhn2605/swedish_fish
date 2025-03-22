#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <vector>
#include <memory>

#include <particle.hpp>
#include <shader.hpp>

class Renderer {
public:
    Renderer(Shader s);

    void PrepareDraw();
    void TestSquare();
    void CleanUp();

    void AddParticle(Particle particle);
    void DrawObject(std::vector<float> vertices, std::vector<unsigned int> indices);
    void DrawParticles();

private:
    std::vector<Particle> particles;

    Shader shader;

    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;
};

#endif