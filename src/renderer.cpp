#include <renderer.hpp>

Renderer::Renderer() {}

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
        // Top face
        3, 2, 6,  6, 7, 3,  
        // Bottom face
        0, 1, 5,  5, 4, 0   
    };
}

void Renderer::UpdateCamera(Shader& shader, Camera& camera) {
    shader.setUniformMat4("view", camera.GetViewMatrix());
    shader.setUniformMat4("projection", camera.GetProjectionMatrix());
}

void Renderer::AddParticle(Particle& particle) {
    particles.push_back(particle);
}

void Renderer::DrawContainer(Shader& shader, glm::vec3 pos, glm::vec3 angle, glm::vec3 scale) {
    // Initialize the transformation matrix
    glm::mat4 model = glm::mat4(1.0f);
    
    // Apply transformations
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f)); // X rotation
    model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Y rotation
    model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z rotation
    model = glm::scale(model, scale);
    
    shader.setUniformMat4("model", model);  // Fix: use the actual model matrix
    shader.setUniformVec3("color", glm::vec3(1.0f));

    // Bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO - Send vertex data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, container_vertices.size() * sizeof(float), container_vertices.data(), GL_STATIC_DRAW);

    // Vertex Attribute Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (toggle_frame) {
        // EBO - Send wireframe indices
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, container_line_indices.size() * sizeof(unsigned int), container_line_indices.data(), GL_STATIC_DRAW);
    
        // Draw wireframe
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, container_line_indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        // EBO - Send triangle indices
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, container_indices.size() * sizeof(unsigned int), container_indices.data(), GL_STATIC_DRAW);

        // Draw solid container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, container_indices.size(), GL_UNSIGNED_INT, 0);
    }

    // Cleanup
    glBindVertexArray(0);
}

void Renderer::DrawParticle(Particle& particle, Shader& shader) {
    shader.setUniformMat4("model", particle.getModelMatrix());
    shader.setUniformVec3("color", particle.getColor());

    std::vector<float> vertices = particle.getVertices();
    std::vector<unsigned int> indices = particle.getIndices();

    // Bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // Set Up EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    // TODO: make stride of 3 a variable that changes accordingly with future additions of normal/tex/binorm
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::DrawParticles(Shader& shader) {
    for (Particle& currParticle : particles) {
        DrawParticle(currParticle, shader);
    }
}

void Renderer::UpdateParticles(Physics& physics) {
    for (Particle& currParticle: particles) {
        physics.Update(currParticle);
    }
}

// Setters
void Renderer::SetFrameMode(bool value) {
    toggle_frame = value;
}

void Renderer::CleanUp() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}