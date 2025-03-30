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

    glm::vec3 dv_vector = glm::vec3(0.0f);

    // Kinematics
    // Gravity / acceleration
    dv_vector.y += initial_vel.y + accel * delta_time;

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
    // take it one step at a time
    glm::vec3 pos = particle.getPosition();

    if (pos.y - particle.getRadius() <= container_min.y) {
        printf("Particle: <%f, %f, %f>\tMin y: %f\n", pos.x, pos.y, pos.z, container_min.y);
        particle.setPosition(glm::vec3(pos.x, container_min.y + particle.getRadius(), pos.z));
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