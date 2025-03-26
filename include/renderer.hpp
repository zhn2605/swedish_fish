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

    void CreateContainer(float length, float width, float height);
    void PrepareDraw(Shader& shader);
    
    void AddParticle(Particle& particle);
    
    void UpdateCamera(Shader& shader, Camera& camera);
    void UpdateParticles(Physics physics);

    void DrawContainer(Shader& shader, glm::vec3 pos, glm::vec3 angle, glm::vec3 scale);
    void DrawParticle(Particle& particle, Shader& shader);
    void DrawParticles(Shader& shader);
    
    // Setters
    void SetFrameMode(bool value);

    void CleanUp();

private:
    std::vector<Particle> particles;
    std::vector<float> container_vertices;
    std::vector<unsigned int> container_indices;
    std::vector<unsigned int> container_line_indices;

    bool toggle_frame = false;

    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;
};

#endif