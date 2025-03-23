#include <particle.hpp>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

Particle::Particle(float r, int sect, int stack) : position(0.0f, 0.0f, 0.0f), modelMatrix(1.0f) {
    radius = r;
    sectorCount = sect;
    stackCount = stack;

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    modelMatrix = glm::mat4(1.0f);
}

void Particle::GenerateVertices() {
    float x, y, z, xy;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        for(int j = 0; j <= sectorCount; ++j) {
            sectorAngle =j * sectorStep;

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
}

void Particle::GenerateIndices() {
    int k1, k2;
    for(int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 +1);
            }
        }
    }
}

void Particle::Initialize() {
    GenerateVertices();
    GenerateIndices(); 
}

void Particle::UpdateModelMatrix() {
    modelMatrix = glm::mat4(1.f);
    // printf("position of sphere: <%f, %f, %f>", position.x, position.y, position.z);
    modelMatrix = glm::translate(modelMatrix, position);
}

// Setters
void Particle::setRadius(int r) { radius = r; }
void Particle::setSectorCount(int sect) { sectorCount = sect; }
void Particle::setStackCount(int stack) { stackCount = stack; }
void Particle::setPosition(glm::vec3 vec) { 
    position = vec; 
}
void Particle::setVelocity(glm::vec3 vec) {
    velocity = vec;
}

// Getters
std::vector<float> Particle::getVertices() { return vertices; }
std::vector<unsigned int> Particle::getIndices() { return indices; }
glm::mat4 Particle::getModelMatrix() { return modelMatrix; }
glm::vec3 Particle::getPosition() { return position; }
glm::vec3 Particle::getVelocity() { return velocity; }