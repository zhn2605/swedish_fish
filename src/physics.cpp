#include <physics.hpp>

Physics::Physics(float dt=0.1f, float a=-9.81f, float surfTens=72.0f, float tg_density=1.0f, float pm=1.0f, float visc_str=0.1f) 
{
    accel = a;
    surface_tension = surfTens;
    target_density = tg_density;
    pressure_multiplier = pm;
    viscosity_strength = visc_str;
    
    delta_time = dt;
    is_paused = false;
    toggle_gravity = true;
    simulation_speed = 1.0f;
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
    if (pos.y - radius <= container_min.y + EPSILON) { // Bottom collision
        particle.setPosition(glm::vec3(pos.x, container_min.y + radius, pos.z));
        collision = true;
    }

    if (pos.y + radius >= container_max.y - EPSILON && toggle_top_collision) { // Top collision
        particle.setPosition(glm::vec3(pos.x, container_max.y - radius, pos.z));
        collision = true;
    }

    if (pos.x - radius <= container_min.x + EPSILON) { // Left collision
        particle.setPosition(glm::vec3(container_min.x + radius, pos.y, pos.z));
        collision = true;
    }
    if (pos.x + radius >= container_max.x - EPSILON) { // Right collision
        particle.setPosition(glm::vec3(container_max.x - radius, pos.y, pos.z));
        collision = true;
    }
    if (pos.z - radius <= container_min.z + EPSILON) { // Front collision
        particle.setPosition(glm::vec3(pos.x, pos.y, container_min.z + radius));
        collision = true;
    }
    if (pos.z + radius >= container_max.z - EPSILON) { // Back collision
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

    // Top collision
    if (pos.y + radius >= container_max.y - EPSILON && toggle_top_collision) { // Top collision
        if (vel.y > 0) {
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
        if (vel.x > 0) {
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
        if (vel.z > 0) {
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
    float volume = 64 * M_PI * std::pow(r, 9) / 315;
    float value = std::max((float)0, r * r - dst * dst);
    return std::pow(value, 3) / volume;
}

float SmoothingKernelDerivative(float r, float dst) {
    // if (dst >= r) { return 0; }
    float volume = 64 * M_PI * std::pow(r, 9) / 315;
    float f = r * r - dst * dst;
    return (-6 * dst * f * f) / volume * r;
}

float Physics::CalculateDensity(Particle& particle, std::vector<Particle>& particles) {
    float density = 0.0f;

    for (Particle& other : particles) {
        glm::vec3 distance_vec = particle.getPosition() - other.getPosition();
        float distance = glm::length(distance_vec);

        if (distance < particle.getSmoothingRadius()) {
            float influence = SmoothingKernelDerivative(particle.getSmoothingRadius(), distance);
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
    glm::vec3 viscosity_force(0.0f);

    float particle_density = densities[particle_index];
    float pressure = pressure_multiplier * (particle_density - target_density);
    
    // if (particle_density < target_density) { return pressure_force; }

    for (size_t i = 0; i < particles.size(); i++) {
        if (i == particle_index) continue;
            
        Particle& other = particles[i];
        glm::vec3 distance_vec = particle.getPosition() - other.getPosition();
        float distance = glm::length(distance_vec);
        glm::vec3 direction = glm::normalize(distance_vec);
            
        if (distance == 0 || distance < EPSILON) { 
            float rand_x = ((rand() % (100 - (-100) + 1)) - 100) / 100;
            float rand_y = ((rand() % (100 - (-100) + 1)) - 100) / 100;
            float rand_z = ((rand() % (100 - (-100) + 1)) - 100) / 100;
            
            direction = glm::normalize(glm::vec3(rand_x, rand_y, rand_z));
        }

        if (distance < particle.getSmoothingRadius()) {
            // Use pre-calculated density for the neighbor
            float other_density = densities[i];
            float other_pressure = pressure_multiplier * (other_density - target_density);
            
            float pressure_kernel = SmoothingKernelDerivative(particle.getSmoothingRadius(), distance);


            // Calculate viscosity force
            float viscosity_influence = SmoothingKernel(particle.getSmoothingRadius(), distance);
            viscosity_force += (other.getVelocity() - particle.getVelocity()) * viscosity_influence;
        
            // Negate (originally +=) to repel
            // or not??????????
            pressure_force += direction * ((pressure + other_pressure) / 2.0f) * pressure_kernel;
        }
    }

    viscosity_force *= viscosity_strength;
    return pressure_force + viscosity_force;
}

// Near Wall hanedling
glm::vec3 Physics::ApplyNearWallPressure(Particle& particle) {
    glm::vec3 repulsion_force(0.0f);
    glm::vec3 pos = particle.getPosition();
    float smoothing_radius = particle.getSmoothingRadius();
    float wall_density = target_density * 2.0f;  // Higher than fluid to push particles away
    float pressure = pressure_multiplier * (wall_density - target_density);

    std::vector<glm::vec3> wall_positions = {
        {container_min.x, pos.y, pos.z},  // Left Wall
        {container_max.x, pos.y, pos.z},  // Right Wall
        {pos.x, pos.y, container_min.z},  // Front Wall
        {pos.x, pos.y, container_max.z},  // Back Wall
    };

    for (const auto& wall_pos : wall_positions) {
        glm::vec3 distance_vec = pos - wall_pos;
        float distance = glm::length(distance_vec);
        glm::vec3 direction = glm::normalize(distance_vec);

        if (distance < smoothing_radius) {
            float kernel = SmoothingKernelDerivative(smoothing_radius / 2.0f, distance);  // Half-radius for walls
            repulsion_force += direction * pressure * kernel;
        }
    }

    return repulsion_force;
}
// Updates
void Physics::UpdateParticle(Particle& particle, const glm::vec3& pressure_force) {
    glm::vec3 initial_pos = particle.getPosition();
    glm::vec3 initial_vel = particle.getVelocity();

    glm::vec3 dv_vector = initial_vel;

    // Gravity / acceleration
    if (toggle_gravity) {
        dv_vector.y += accel * delta_time;
    }
    
    // Fluid forces
    float mass_inverse = 1.0f / particle.getMass();
    // dv_vector += pressure_force * mass_inverse * delta_time;
    dv_vector += pressure_force * mass_inverse * delta_time;
    
    // Vec changes
    particle.setVelocity(dv_vector);
    glm::vec3 final_pos = initial_pos + (dv_vector * delta_time);

    Particle temp_particle(particle);
    temp_particle.setPosition(final_pos);

    // Collision
    if (CheckContainerCollision(temp_particle)) {
        final_pos = temp_particle.getPosition();

        ResolveContainerCollision(particle);
    }
    // ApplyNearWallPressure(particle);
    particle.setPosition(final_pos);

    // Apply necessary updates within particle
    particle.UpdateModelMatrix();
    particle.UpdateColor();

    // debug
    // printf("pos: <%f, %f, %f>\tvel: <%f, %f, %f>\n", final_pos.x, final_pos.y, final_pos.z, dv_vector.x, dv_vector.y, dv_vector.z);
}

void Physics::UpdateSystem(std::vector<Particle>& particles) {
    // Precalculate all densities
    std::vector<float> densities = CalculateAllDensities(particles);

    for (size_t i = 0; i < particles.size(); i++) {
        Particle& particle = particles[i];
        glm::vec3 pressure_force = CalculatePressureForce(particle, particles, densities, i);

        UpdateParticle(particle, pressure_force);
    }
}

// Setters
void Physics::SetBounds(glm::vec3& min, glm::vec3& max) {
    container_min = min;
    container_max = max;
}

void Physics::SetSimulationSpeed(float value) { 
    simulation_speed = value; 

    // simple clamp
    if (simulation_speed <= -1.0f) { simulation_speed = -1.0f; }
    if (simulation_speed >= 4.0f) { simulation_speed = 4.0f; }

    // debug
    printf("Simulation speed: %f\n", simulation_speed);
}

void Physics::SetDeltaTime(float dt) {
    delta_time = dt * simulation_speed;
}

void Physics::SetPause(bool value) { is_paused = value; }
void Physics::SetGravity(bool value) { toggle_gravity = value; }
void Physics::ToggleTopCollision(bool value) { toggle_top_collision = value; }

// Getters
bool Physics::IsPaused() { return is_paused; }
bool Physics::GetGravity() { return toggle_gravity; }
float Physics::GetSimulationSpeed() { return simulation_speed; }