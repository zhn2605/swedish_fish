#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Particle{
public:
    Particle(float r, int sect, int stack);
    void Initialize();
    void UpdateModelMatrix();
    void UpdateColor();

    // Setters
    void setRadius(float r);
    void setSectorCount(int sect);
    void setStackCount(int stack);
    void setPosition(glm::vec3 vec);
    void setVelocity(glm::vec3 vec);

    // Getters
    float getRadius();
    std::vector<float> getVertices();
    std::vector<unsigned int> getIndices();
    glm::mat4 getModelMatrix();
    glm::vec3 getPosition();
    glm::vec3 getVelocity();
    glm::vec3 getColor();

private:
    // Particle properties
    float radius;
    int sectorCount, stackCount;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Particle initialization functions
    void GenerateVertices();
    void GenerateIndices();

    // Particle physics data 
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 color;
    glm::mat4 modelMatrix;
};

#endif