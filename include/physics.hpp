#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <particle.hpp>

class Physics {
public:
    Physics(float dt, float a, float surfTens);

    // Update
    void Update(Particle& particle);      // over arching update function 
    
    // Helper functions
    float CalcVelMagnitude(Particle& particle);

    // Setters
    void SetDeltaTime(float dt);

private:
    float delta_time;
    float surface_tension;
    float accel;
};

#endif