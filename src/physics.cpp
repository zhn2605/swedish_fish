#include <physics.hpp>

Physics::Physics(float dt=0.1f, float a=-9.81f, float surfTens=72.0f) {
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

    // Other stuff like collision.. fluid dynamics.. etc.


    // Final vec changes
    particle.setVelocity(dv_vector);

    glm::vec3 final_pos = initial_pos + (dv_vector * delta_time);
    particle.setPosition(final_pos);

    // Apply necessary updates within particle
    particle.UpdateModelMatrix();
    //particle.UpdateColor();

    // Debug
    // printf("Particle y value: %f\n", particle.getPosition().y);
}

void Physics::CalcVelocity(Particle particle) {
    
}

// Setters
void Physics::SetDeltaTime(float dt) {
    delta_time = dt;
}