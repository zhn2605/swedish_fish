#include <physics.hpp>

Physics::Physics(float dt=0.1f, float a=-9.81f, float surfTens=72.0f) 
{
    delta_time = dt;
    accel = a;
    surface_tension = surfTens;
}

void Physics::Update(Particle& particle) {
    glm::vec3 initial_pos = particle.getPosition();
    glm::vec3 initial_vel = particle.getVelocity();

    glm::vec3 dv_vector = initial_vel;

    // Gravity / acceleration
    dv_vector.y += accel * delta_time;


    
    // Final vec changes
    particle.setVelocity(dv_vector);

    glm::vec3 final_pos = initial_pos + (dv_vector * delta_time);
    particle.setPosition(final_pos);

    // Collision
    if (CheckContainerCollision(particle)) {
        ResolveContainerCollision(particle);
    }

    // Apply necessary updates within particle
    particle.UpdateModelMatrix();
    particle.UpdateColor();

    // Debug
    // printf("Particle y value: %f\n", particle.getPosition().y);
}

// Collision
bool Physics::CheckContainerCollision(Particle& particle) {
    bool collision = false;
    glm::vec3 pos = particle.getPosition();
    float radius = particle.getRadius();

    // Clamp position and register collisions
    if (pos.y - radius <= container_min.y) { // Bottom collision
        particle.setPosition(glm::vec3(pos.x, container_min.y + radius, pos.z));
        collision = true;
    }
    if (pos.x - radius <= container_min.x) { // Left collision
        particle.setPosition(glm::vec3(container_min.x + radius, pos.y, pos.z));
        collision = true;
    }
    if (pos.x + radius >= container_max.x) { // Right collision
        particle.setPosition(glm::vec3(container_max.x - radius, pos.y, pos.z));
        collision = true;
    }
    if (pos.z - radius <= container_min.z) { // Front collision
        particle.setPosition(glm::vec3(pos.x, pos.y, container_min.z + radius));
        collision = true;
    }
    if (pos.z + radius >= container_max.z) { // Back collision
        particle.setPosition(glm::vec3(pos.x, pos.y, container_max.z - radius));
        collision = true;
    }

    return collision;
}

void Physics::ResolveContainerCollision(Particle& particle) {
    glm::vec3 pos = particle.getPosition();
    glm::vec3 vel = particle.getVelocity();
    float radius = particle.getRadius();
    float restitution = 0.6f; // Higher = bouncier
    
    // Bottom collision
    if (pos.y - radius <= container_min.y + EPSILON) {
        if (vel.y < 0) {
            particle.setVelocity(glm::vec3(vel.x, -restitution * vel.y, vel.z));
        }
    }
    // Left collision
    if (pos.x - radius <= container_min.x + EPSILON) {
        if (vel.x < 0) {
            particle.setVelocity(glm::vec3(-restitution * vel.x, vel.y, vel.z));
        }
    }
    // Right collision
    if (pos.x + radius >= container_max.x - EPSILON) {
        if (-vel.x < 0) {
            particle.setVelocity(glm::vec3(-restitution * vel.x, vel.y, vel.z));
        }
    }
    // Front collision
    if (pos.z - radius <= container_min.z + EPSILON) {
        if (vel.z < 0) {
            particle.setVelocity(glm::vec3(vel.x, vel.y, -restitution * vel.z));
        }
    }
    // Back collision
    if (pos.z + radius >= container_max.z - EPSILON) {
        if (-vel.z < 0) {
            particle.setVelocity(glm::vec3(vel.x, vel.y, -restitution * vel.z));
        }
    }
    
    // Epislon to remove arbitraty small velocity
    if (glm::length(particle.getVelocity()) < EPSILON) {
        particle.setVelocity(glm::vec3(0.0f));
    }
}


// Setters
void Physics::SetBounds(glm::vec3& min, glm::vec3& max) {
    container_min = min;
    container_max = max;
}

void Physics::SetDeltaTime(float dt) {
    delta_time = dt;
}