#include <iostream>
#include <chrono>

#include <app.hpp>
#include <shader.hpp>
#include <renderer.hpp>
#include <particle.hpp>
#include <physics.hpp>
#include <camera.hpp>

// Settings
std::string vertexFilePath = "./include/shader/vert.glsl";
std::string fragmentFilePath = "./include/shader/frag.glsl";
float delta_time = 0.0f;
float gravity = -9.81f;
float surface_tension = 72.0f;

int main(void) {
  // Time
  auto last_frame_time = std::chrono::high_resolution_clock::now();

  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();

  Shader shader(vertexFilePath, fragmentFilePath);

  Camera camera(1.0f); // sensitivity
  camera.SetAspectRatio(app.getAspectRatio());
  camera.SetEyePosition(glm::vec3(0.0f, 0.0f, -5.0f));

  // Particles
  Renderer renderer;
  Particle particle(1.0f, 36, 18);
  particle.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
  renderer.AddParticle(particle);
  
  Physics physics(delta_time, gravity, surface_tension);
  renderer.UpdateParticles(physics);

  while (app.isActive()) {
    // Calculate delta time
    auto current_time = std::chrono::high_resolution_clock::now();
    delta_time = std::chrono::duration<float>(current_time - last_frame_time).count();
    last_frame_time = current_time;

    physics.SetDeltaTime(delta_time);

    app.update();

    // Prepares gl buffers etc.
    renderer.PrepareDraw(shader);
    camera.UpdateMatrices();

    // Update Renderer / physics
    renderer.UpdateCamera(shader, camera);
    renderer.UpdateParticles(physics);

    // Draw
    renderer.DrawParticles(shader);
  }

  renderer.CleanUp();
  shader.destroy();
  app.Terminate();

  std::cout << "Goodbye!" << std::endl;
  return 0;
}