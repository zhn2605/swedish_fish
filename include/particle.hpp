#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Particle{
public:
    Particle(float r, float m, int sect, int stack, float smoothing_r);
    void Initialize();
    void UpdateModelMatrix();
    void UpdateColor();

    // Setters
    void setRadius(float r);
    void setSectorCount(int sect);
    void setStackCount(int stack);
    void setPosition(glm::vec3 vec);
    void setVelocity(glm::vec3 vec);
    void setMass(float m);

    // Getters
    float getRadius();
    float getMass();
    std::vector<float> getVertices();
    std::vector<unsigned int> getIndices();
    float getSmoothingRadius();
    glm::mat4 getModelMatrix();
    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    glm::vec4 getColor();

private:
    // Particle properties
    float radius;
    float mass;
    int sectorCount, stackCount;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float smoothing_radius;
    
    // Particle initialization functions
    void GenerateVertices();
    void GenerateIndices();

    // Particle physics data 
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    glm::mat4 modelMatrix;
};

#endif