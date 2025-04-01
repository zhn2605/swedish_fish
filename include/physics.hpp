#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <cstdlib>

#include <particle.hpp>

#define EPSILON 0.00000001

class Physics {
public:
    Physics(float dt, float a, float surfTens, float tg_density, float pm);

    // Update
    void UpdateSystem(std::vector<Particle>& particles); // over arching update function
    void UpdateParticle(Particle& particle, const glm::vec3& pressure_force);

    // Collision
    bool CheckContainerCollision(Particle& particle);
    void ResolveContainerCollision(Particle& particle);
    bool CheckParticleCollision(Particle& particle);
    void ResolveParticleCollision(Particle& particle);

    // Fluid
    float CalculateDensity(Particle& particle, std::vector<Particle>& particles);
    std::vector<float> CalculateAllDensities(std::vector<Particle>& particles);
    glm::vec3 CalculatePressureForce(Particle& particle, std::vector<Particle>& particles, std::vector<float>& densities, size_t particle_index);
    
    // Setters
    void SetBounds(glm::vec3& min, glm::vec3& max);
    void SetDeltaTime(float dt);
    
private:
    glm::vec3 container_min;
    glm::vec3 container_max;

    float delta_time;
    float surface_tension;
    float accel;
    float target_density;
    float pressure_multiplier;
};

#endif