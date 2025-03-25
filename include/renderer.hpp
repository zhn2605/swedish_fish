#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <vector>
#include <memory>

#include <particle.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include <physics.hpp>

class Renderer {
public:
    Renderer();

    void PrepareDraw(Shader& shader);
    void CleanUp();

    void UpdateCamera(Shader& shader, Camera& camera);

    void AddParticle(Particle& particle);
    void UpdateParticles(Physics physics);
    void DrawParticle(Particle& particle, Shader& shader);
    void DrawParticles(Shader& shader);

private:
    std::vector<Particle> particles;

    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;
};

#endif