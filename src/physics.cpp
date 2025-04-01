#include <physics.hpp>

Physics::Physics(float dt=0.1f, float a=-9.81f, float surfTens=72.0f, float tg_density=1.0f, float pm=1.0f) 
{
    delta_time = dt;
    accel = a;
    surface_tension = surfTens;

    target_density = tg_density;
    pressure_multiplier = pm;
}

/*
===============================================================================================
Collision
===============================================================================================
*/ 
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

/* 
===============================================================================================
Fluid Calculations
===============================================================================================
*/

float SmoothingKernel(float r, float dst) {
    // directly from Sebastion Lague thank u good sir
    float volume = M_PI * std::pow(r, 8) / 4;
    float value = std::max((float)0, r * r - dst * dst);
    return std::pow(value, 3) / volume;
}

float Physics::CalculateDensity(Particle& particle, std::vector<Particle>& particles) {
    float density = 0.0f;

    for (Particle& other : particles) {
        glm::vec3 distance_vec = particle.getPosition() - other.getPosition();
        float distance = glm::length(distance_vec);

        if (distance < particle.getSmoothingRadius()) {
            float influence = SmoothingKernel(particle.getSmoothingRadius(), distance);
            density += other.getMass() * influence;
        }
    }

    return density;
}

std::vector<float> Physics::CalculateAllDensities(std::vector<Particle>& particles) {
    std::vector<float> densities(particles.size(), 0.0f);
        
        for (size_t i = 0; i < particles.size(); i++) {
            densities[i] = CalculateDensity(particles[i], particles);
        }
        
    return densities;
}

glm::vec3 Physics::CalculatePressureForce(Particle& particle, std::vector<Particle>& particles, std::vector<float>& densities, size_t particle_index) {
    glm::vec3 pressure_force(0.0f);

    float particle_density = densities[particle_index];
    float pressure = pressure_multiplier * (particle_density - target_density);
    
    if (particle_density < target_density) { return pressure_force; }

    for (size_t i = 0; i < particles.size(); i++) {
        if (i == particle_index) continue;
            
        Particle& other = particles[i];
        glm::vec3 distance_vec = particle.getPosition() - other.getPosition();
        float distance = glm::length(distance_vec);
        glm::vec3 direction = glm::normalize(distance_vec);
            
        if (distance == 0 || distance < 0.0001f) { 
            float rand_x = ((rand() % (100 - (-100) + 1)) - 100) / 100;
            float rand_y = ((rand() % (100 - (-100) + 1)) - 100) / 100;
            float rand_z = ((rand() % (100 - (-100) + 1)) - 100) / 100;
            
            glm::vec3 direction = glm::normalize(glm::vec3(rand_x, rand_y, rand_z));
        }

        // TODO: Implement newton's third law of motion
        if (distance < particle.getSmoothingRadius()) {
            // Use pre-calculated density for the neighbor
            float other_density = densities[i];
            float other_pressure = pressure_multiplier * (other_density - target_density);
            
            float kernel = SmoothingKernel(particle.getSmoothingRadius(), distance);
        
            // Negate (originally +=) to repel
            // or not??????????
            pressure_force += direction * ((pressure + other_pressure) / 2.0f) * kernel;
        }
    }    
    return pressure_force;
}

// Updates
void Physics::UpdateParticle(Particle& particle, const glm::vec3& pressure_force) {
    glm::vec3 initial_pos = particle.getPosition();
    glm::vec3 initial_vel = particle.getVelocity();

    glm::vec3 dv_vector = initial_vel;

    // Gravity / acceleration
    dv_vector.y += accel * delta_time;
    
    // Fluid forces
    float mass_inverse = 1.0f / particle.getMass();
    // dv_vector += pressure_force * mass_inverse * delta_time;
    dv_vector += pressure_force * mass_inverse * delta_time;
    
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
}

void Physics::UpdateSystem(std::vector<Particle>& particles) {
    // Precalculate all densities
    std::vector<float> densities = CalculateAllDensities(particles);

    for (size_t i = 0; i < particles.size(); i++) {
        Particle& particle = particles[i];

        // Get cached density
        float density = densities[i];
        glm::vec3 pressure_force = CalculatePressureForce(particle, particles, densities, i);

        UpdateParticle(particle, pressure_force);
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