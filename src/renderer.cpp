#include <renderer.hpp>

Renderer::Renderer(int value) {}

void Renderer::PrepareDraw(Shader& shader) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
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
<<<<<<< HEAD
=======
    shader.use();
    GLint viewLoc = glGetUniformLocation(shader.ID, "view");
    GLint projLoc = glGetUniformLocation(shader.ID, "projection");
    GLint modelLoc = glGetUniformLocation(shader.ID, "model");
    std::cout << "view: " << viewLoc << ", proj: " << projLoc << ", model: " << modelLoc << std::endl;
    shader.setUniformVec3("pos", particle.getPosition());
>>>>>>> 2eb5ab5756161f74a86365c636d80261f639f0c3
    shader.setUniformMat4("model", particle.getModelMatrix());

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