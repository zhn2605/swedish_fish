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
    Physics(float dt, float a, float surfTens, float tg_density, float pm, float visc_str);

    // Update
    void UpdateSystem(std::vector<Particle>& particles); // over arching update function
    void UpdateParticle(Particle& particle, const glm::vec3& pressure_force);

    // Collision
    bool CheckContainerCollision(Particle& particle);
    void ResolveContainerCollision(Particle& particle);

    glm::vec3 ApplyNearWallPressure(Particle& particle);

    // Fluid
    float CalculateDensity(Particle& particle, std::vector<Particle>& particles);
    std::vector<float> CalculateAllDensities(std::vector<Particle>& particles);
    glm::vec3 CalculatePressureForce(Particle& particle, std::vector<Particle>& particles, std::vector<float>& densities, size_t particle_index);
    
    // Setters
    void SetBounds(glm::vec3& min, glm::vec3& max);
    void SetSimulationSpeed(float value);
    void SetDeltaTime(float dt);
    void SetPause(bool value);
    void SetGravity(bool value);
    void ToggleTopCollision(bool value);

    // Getters
    float GetSimulationSpeed();
    bool IsPaused();
    bool GetGravity();
    
private:
    // Container info
    glm::vec3 container_min;
    glm::vec3 container_max;

    // Fluid simulation settings
    float surface_tension;
    float viscosity_strength;
    float accel;
    float target_density;
    float pressure_multiplier;

    // Engine settings
    float delta_time;
    bool is_paused;
    bool toggle_top_collision;
    bool toggle_gravity;
    float simulation_speed;
};

#endif