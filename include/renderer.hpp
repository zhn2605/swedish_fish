#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <vector>
#include <memory>

#include <particle.hpp>
#include <shader.hpp>
#include <camera.hpp>

class Renderer {
public:
    Renderer(Shader s, Camera cam);

    void PrepareDraw();
    void TestSquare();
    void CleanUp();

    void UpdateCamera();

    void AddParticle(Particle particle);
    void UpdateParticles();
    void DrawParticle(Particle particle);
    void DrawParticles();

private:
    std::vector<Particle> particles;

    Shader shader;
    Camera camera;

    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;
};

#endif