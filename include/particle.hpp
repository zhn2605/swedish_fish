#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <math.h>

class Particle{
public:
    Particle(float r, int sect, int stack);
    void Initialize();

    // setters
    void setRadius(int r);
    void setSectorCount(int sect);
    void setStackCount(int stack);

    // getters
    std::vector<float> getVertices();
    std::vector<unsigned int> getIndices();

private:
    float radius;
    int sectorCount, stackCount;
    
    void GenerateVertices();
    void GenerateIndices();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif