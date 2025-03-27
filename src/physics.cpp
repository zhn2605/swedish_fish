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
    glm::vec3 particle_pos = particle.getPosition();

    // x axis
    if (particle_pos.x - particle.getRadius() < container_min.x) {
        particle_pos.x = container_min.x + particle.getRadius();
        collision = true;
    } else if (particle_pos.x + particle.getRadius() > container_max.x) {
        particle_pos.x = container_max.x + particle.getRadius();
        collision = true;
    }

    // y axis
    if (particle_pos.y - particle.getRadius() < container_min.y) {
        particle_pos.y = container_min.y + particle.getRadius();
        collision = true;
    } else if (particle_pos.y + particle.getRadius() > container_max.y) {
        particle_pos.y = container_max.y + particle.getRadius();
        collision = true;
    }

    // z axis
    if (particle_pos.z - particle.getRadius() < container_min.z) {
        particle_pos.z = container_min.z + particle.getRadius();
        collision = true;
    } else if (particle_pos.z + particle.getRadius() > container_max.z) {
        particle_pos.z = container_max.z + particle.getRadius();
        collision = true;
    }

    particle.setPosition(particle_pos);
    return collision;
}

void Physics::ResolveContainerCollision(Particle& particle) {
    // Determine the collision axis and create a normal vector
    glm::vec3 normal(0.0f);
    glm::vec3 particle_pos = particle.getPosition();
    
    // x axis
    if (particle_pos.x - particle.getRadius() <= container_min.x || 
        particle_pos.x + particle.getRadius() >= container_max.x) {
            printf("Hit x bount");
            normal.x = (particle_pos.x > (container_min.x + container_max.x) / 2.0f) ? -1.0f : 1.0f;
    }
    // y collision
    else if (particle_pos.y - particle.getRadius() <= container_min.y || 
        particle_pos.y + particle.getRadius() >= container_max.y) {
            printf("Hit y bound");
            normal.y = (particle_pos.y > (container_min.y + container_max.y) / 2.0f) ? -1.0f : 1.0f;
    }
    // z collision
    else if (particle_pos.z - particle.getRadius() <= container_min.z || 
        particle_pos.z + particle.getRadius() >= container_max.z) {
            printf("Hit z bound");
            normal.z = (particle_pos.z > (container_min.z + container_max.z) / 2.0f) ? -1.0f : 1.0f;
    }
    
    // reverse velocity in direction of colision
    if (glm::length(normal) > 0) {
        float normal_velocity_magnitude = glm::dot(particle.getVelocity(), normal);
        particle.setVelocity(normal_velocity_magnitude * normal);
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