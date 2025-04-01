#include <renderer.hpp>

Renderer::Renderer() : default_particle(0.2f, 1.0f, 38, 16, 1.0f) {}

void Renderer::PrepareDraw(Shader& shader) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
}

void Renderer::CreateContainer(float length, float width, float height) {
    // Clear vertices if not empty
    if (!container_vertices.empty()) {
        container_vertices.clear();
        container_line_indices.clear();
    }

    container_min_bound = glm::vec3(-length/2, -height/2, -width/2);
    container_max_bound = glm::vec3(length/2, height/2, width/2);

    // Create new vertices and indices for container
    container_vertices = {
        // Base coordinates
        -length/2,  -height/2,  -width/2,
        -length/2,  -height/2,  width/2,
        length/2,   -height/2,  width/2,
        length/2,   -height/2,  -width/2,
        
        // Top
        -length/2,   height/2,  -width/2,
        -length/2,   height/2,  width/2,
        length/2,    height/2,  width/2,
        length/2,    height/2,  -width/2
    };

    container_line_indices = {
        // Bottom face edges
        0, 1,  1, 2,  2, 3,  3, 0,  
        // Top face edges
        4, 5,  5, 6,  6, 7,  7, 4,  
        // Vertical edges
        0, 4,  1, 5,  2, 6,  3, 7  
    };

    container_indices = {
        // Front face
        0, 1, 2,  2, 3, 0,  
        // Back face
        4, 5, 6,  6, 7, 4,  
        // Left face
        0, 4, 7,  7, 3, 0,  
        // Right face
        1, 5, 6,  6, 2, 1,  
        // // Top face
        // 3, 2, 6,  6, 7, 3,  
        // Bottom face
        0, 1, 5,  5, 4, 0   
    };

    InitializeContainerMesh();
}

void Renderer::UpdateCamera(Shader& shader, Camera& camera) {
    shader.setUniformMat4("view", camera.GetViewMatrix());
    shader.setUniformMat4("projection", camera.GetProjectionMatrix());
}

void Renderer::AddParticle(Particle& particle) {
    particles.push_back(particle);
}

void Renderer::InitializeContainerMesh() {
    // Generate VAO
    glGenVertexArrays(1, &container_VAO);
    glBindVertexArray(container_VAO);

    // Generate VBOs
    glGenBuffers(1, &container_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, container_VBO);
    glBufferData(GL_ARRAY_BUFFER, container_vertices.size() * sizeof(float), container_vertices.data(), GL_STATIC_DRAW);

    // Vertex Attribute Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // EBO
    glGenBuffers(1, &container_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, container_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, container_line_indices.size() * sizeof(unsigned int), container_line_indices.data(), GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);
}

void Renderer::DrawContainer(Shader& shader) {
    // Initialize the transformation matrix
    glm::mat4 model = glm::mat4(1.0f);
    
    shader.setUniformMat4("model", model);  // Fix: use the actual model matrix
    shader.setUniformVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 0.2f));

    // Bind VAO
    glBindVertexArray(container_VAO);

    if (toggle_frame) {
        shader.setUniformVec4("color", glm::vec4(1.0f));

        // EBO - Send wireframe indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, container_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, container_line_indices.size() * sizeof(unsigned int), container_line_indices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_LINES, container_line_indices.size(), GL_UNSIGNED_INT, 0);
       
    } else {
        shader.setUniformVec4("color", glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));

        // EBO - Send triangle indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, container_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, container_indices.size() * sizeof(unsigned int), container_indices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, container_indices.size(), GL_UNSIGNED_INT, 0);
    }

    // Unbind VAO
    glBindVertexArray(0);
}

/* 
TODO: 
  - Instancing
*/
void Renderer::SetDefaultParticle(Particle& particle) {
    default_particle = particle;
    InitializeParticleMesh(particle);
}

void Renderer::InitializeParticleMesh(Particle& sample_particle) {
    // Particle particle(0.1, 36, 18);
    particle_vertices = sample_particle.getVertices();
    particle_indices = sample_particle.getIndices();

    // Bind VAO
    glGenVertexArrays(1, &particle_VAO);
    glBindVertexArray(particle_VAO);

    // VBO
    glGenBuffers(1, &particle_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, particle_VBO);
    glBufferData(GL_ARRAY_BUFFER, particle_vertices.size() * sizeof(float), particle_vertices.data(), GL_DYNAMIC_DRAW);

    // Set Up EBO
    glGenBuffers(1, &particle_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, particle_indices.size() * sizeof(unsigned int), particle_indices.data(), GL_DYNAMIC_DRAW);

    // TODO: make stride of 3 a variable that changes accordingly with future additions of normal/tex/binorm
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glDrawElements(GL_TRIANGLES, particle_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::SpawnParticleGrid(int count, float spacing, float start_y) {
    // Set all available space
    glm::vec3 available_space = container_max_bound - container_min_bound;

    float particle_radius = default_particle.getRadius();
    glm::vec3 start_pos = container_min_bound + glm::vec3(particle_radius);
    start_pos.y = start_y;

    glm::vec3 end_pos = container_max_bound - glm::vec3(particle_radius);

    // Calculate max particles in each direction
    int max_x = (int)((end_pos.x - start_pos.x) / spacing);
    int max_z = (int)((end_pos.z - start_pos.z) / spacing);
    int particles_per_layer = max_x * max_z;

    int spawned = 0;

    for (int y = 0; spawned < count; y++) {
        for (int z = 0; z < max_z && spawned < count; z++) {
            for (int x = 0; x < max_x && spawned < count; x++) {
                // Calculate position
                glm::vec3 position(start_pos.x + x * spacing, start_pos.y + y * spacing, start_pos.z + z * spacing);

                Particle particle(default_particle);
                particle.setPosition(position);
                particle.setVelocity(glm::vec3(0.0f));
                AddParticle(particle);

                spawned++;
                printf("spawned: %d\n", spawned);
            }
        }
    }
}

void Renderer::DrawParticles(Shader& shader) {
    glBindVertexArray(particle_VAO);
    
    for (Particle& currParticle : particles) {
        shader.setUniformMat4("model", currParticle.getModelMatrix());
        shader.setUniformVec4("color", currParticle.getColor());

        glDrawElements(GL_TRIANGLES, particle_indices.size(), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void Renderer::UpdateParticles(Physics& physics) {
    // for (Particle& currParticle: particles) {
    //     physics.UpdateParticle(currParticle);
    // }

    physics.UpdateSystem(particles);
}

// Setters
void Renderer::SetFrameMode(bool value) {
    toggle_frame = value;
}

void Renderer::SetConstantFlow(bool value) {
    constant_flow = value;
}

// Getters
unsigned int Renderer::GetParticleCount() { return particles.size(); }
bool Renderer::IsConstantFlow() { return constant_flow; }
Particle Renderer::GetDefaultParticle() { return default_particle; }

void Renderer::CleanUp() {
    container_indices.clear();
    container_line_indices.clear();
    particles.clear();

    glDeleteVertexArrays(1, &particle_VAO);
    glDeleteBuffers(1, &particle_VBO);
    glDeleteBuffers(1, &particle_EBO);

    glDeleteVertexArrays(1, &container_VAO);
    glDeleteBuffers(1, &container_VBO);
    glDeleteBuffers(1, &container_EBO);
}