#include <renderer.hpp>

Renderer::Renderer() {}

void Renderer::PrepareDraw(Shader& shader) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
}

void Renderer::UpdateCamera(Shader& shader, Camera& camera) {
    shader.setUniformMat4("view", camera.GetViewMatrix());
    shader.setUniformMat4("projection", camera.GetProjectionMatrix());
}

void Renderer::AddParticle(Particle& particle) {
    particles.push_back(particle);
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

void Renderer::UpdateParticles(Physics physics) {
    for (Particle& currParticle: particles) {
        physics.Update(currParticle);
    }
}

void Renderer::CleanUp() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}