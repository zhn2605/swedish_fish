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

    Initialize();
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
    // printf("position of sphere: <%f, %f, %f>", position.x, position.y, position.z);
    //FIXME: no fixes here. 
    // But i just want to note that this was the bane of my existence for three hours
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Particle::UpdateColor() {
    float magnitude = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));

    glm::vec4 blue      = glm::vec4(0.2f, 0.4f, 1.0f, 1.0f);
    glm::vec4 green     = glm::vec4(0.3686f, 1.0f, 0.321f, 1.0f);
    glm::vec4 yellow    = glm::vec4(1.0f, 1.0f, 0.4f, 1.0f);
    glm::vec4 orange    = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
    glm::vec4 red       = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);

    if (magnitude <= 3.0f) {
        color = blue;
    } else if (magnitude <= 6.0f) {
        float t = (magnitude - 3.0f) / (6.0f - 3.0f); // Normalize [0,1]
        color = glm::mix(blue, green, t);
    } else if (magnitude <= 9.0f) {
        float t = (magnitude - 6.0f) / (9.0f - 6.0f);
        color = glm::mix(green, yellow, t);
    } else if (magnitude <= 12.0f) {
        float t = (magnitude - 9.0f) / (12.0f - 9.0f);
        color = glm::mix(yellow, orange, t);
    } else {
        float t = (magnitude - 12.0f) / (15.0f - 12.0f); // Clamp max around 15m/s
        color = glm::mix(orange, red, glm::clamp(t, 0.0f, 1.0f));
    }
}

// Setters
void Particle::setRadius(float r) { radius = r; }
void Particle::setSectorCount(int sect) { sectorCount = sect; }
void Particle::setStackCount(int stack) { stackCount = stack; }
void Particle::setPosition(glm::vec3 vec) { position = vec; }
void Particle::setVelocity(glm::vec3 vec) { velocity = vec; }

// Getters
float Particle::getRadius() { return radius; }
std::vector<float> Particle::getVertices() { return vertices; }
std::vector<unsigned int> Particle::getIndices() { return indices; }
glm::mat4 Particle::getModelMatrix() { return modelMatrix; }
glm::vec3 Particle::getPosition() { return position; }
glm::vec3 Particle::getVelocity() { return velocity; }
glm::vec4 Particle::getColor() { return color; }
