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
    void UpdateParticles(Physics& physics);

    void SetDefaultParticle(Particle& particle);
    void InitializeParticleMesh(Particle& sample_particle);
    void SpawnParticleGrid(int count, float spacing, float start_y);

    void DrawContainer(Shader& shader);
    void DrawParticle(Particle& particle, Shader& shader);
    void DrawParticles(Shader& shader);

    void ClearParticles();
    
    // Setters
    void SetFrameMode(bool value);
    void SetConstantFlow(bool value);
    
    // Getters
    unsigned int GetParticleCount();
    bool IsConstantFlow();
    Particle GetDefaultParticle();
    
    void CleanUp();

    glm::vec3 container_min_bound;
    glm::vec3 container_max_bound;
private:
    Particle default_particle;
    void InitializeContainerMesh();

    std::vector<Particle> particles;
    std::vector<float> particle_vertices;
    std::vector<unsigned int> particle_indices;

    std::vector<float> container_vertices;
    std::vector<unsigned int> container_indices;
    std::vector<unsigned int> container_line_indices;

    bool toggle_frame = false;
    bool constant_flow = false;

    unsigned int particle_EBO;
    unsigned int particle_VBO;
    unsigned int particle_VAO;

    unsigned int container_EBO;
    unsigned int container_VBO;
    unsigned int container_VAO;
};

#endif