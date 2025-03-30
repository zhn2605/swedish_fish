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

    // Kinematics
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
    // Determine the collision axis and create a normal vector
    if (glm::length(particle.getVelocity()) < EPSILON) {
        particle.setVelocity(glm::vec3(0.0f));
    }

    particle.setVelocity(-0.6f * particle.getVelocity());
}

// Setters
void Physics::SetBounds(glm::vec3& min, glm::vec3& max) {
    container_min = min;
    container_max = max;
}

void Physics::SetDeltaTime(float dt) {
    delta_time = dt;
}