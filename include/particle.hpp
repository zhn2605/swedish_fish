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

    // setters
    void setRadius(int r);
    void setSectorCount(int sect);
    void setStackCount(int stack);
    void setPosition(float x, float y, float z);

    // getters
    std::vector<float> getVertices();
    std::vector<unsigned int> getIndices();
    glm::mat4 getModelMatrix();
    glm::vec3 getPosition();

private:
    float radius;
    int sectorCount, stackCount;
    
    void GenerateVertices();
    void GenerateIndices();

    glm::vec3 position;
    glm::mat4 modelMatrix;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif