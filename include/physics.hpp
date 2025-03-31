#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <iostream>
#include <glm/glm.hpp>
#include <particle.hpp>

#define EPSILON 0.00000001

class Physics {
public:
    Physics(float dt, float a, float surfTens);

    // Update
    void Update(Particle& particle);      // over arching update function

    // Collision
    bool CheckContainerCollision(Particle& particle);
    void ResolveContainerCollision(Particle& particle);
    bool CheckParticleCollision(Particle& particle);
    void ResolveParticleCollision(Particle& particle);

    // Setters
    void SetBounds(glm::vec3& min, glm::vec3& max);
    void SetDeltaTime(float dt);
    
private:
    glm::vec3 container_min;
    glm::vec3 container_max;

    float delta_time;
    float surface_tension;
    float accel;
};

#endif