#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>


struct Vertex{
    
}

class Particle{
public:
    Particle(int radius);

    

private:
    void initialize();
    std::vector<float> verticies;
};

#endif