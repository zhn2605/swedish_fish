#include <iostream>
#include <chrono>

#include <app.hpp>
#include <input.hpp>
#include <shader.hpp>
#include <renderer.hpp>
#include <particle.hpp>
#include <physics.hpp>
#include <camera.hpp>

// Settings
std::string vertexFilePath = "./include/shader/vert.glsl";
std::string fragmentFilePath = "./include/shader/frag.glsl";

// Particle
float radius = 0.15f;
float mass = 1.0f;
float smoothing_radius = 1.0f;

// Physics
float delta_time = 0.0f;
float gravity = -9.81f;
float surface_tension = 72.0f;
float target_density = 5.0f;
float pressure_multiplier = 5.0f;

int main(void) {
  // Time
  auto last_frame_time = std::chrono::high_resolution_clock::now();

  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();
  app.setRelativeMode(true);

  Shader shader(vertexFilePath, fragmentFilePath);

  Camera camera(1.0f); // sensitivity
  camera.SetAspectRatio(app.getAspectRatio());
  camera.SetEyePosition(glm::vec3(0.0f, 0.0f, -10.0f));

  // Rendering
  Renderer renderer;
  renderer.CreateContainer(20.0f, 6.0f, 10.0f);
  renderer.SetFrameMode(true);

  // Create sample particle
  Particle sample_particle(radius, mass, 36, 18, smoothing_radius);
  sample_particle.setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
  renderer.SetDefaultParticle(sample_particle);
  renderer.SpawnParticleGrid(1000, 0.5f, 0.0f); // particle count, spacing, y position
  // renderer.AddParticle(sample_particle);
  
  // Physics
  Physics physics(delta_time, gravity, surface_tension, target_density, pressure_multiplier);
  physics.SetBounds(renderer.container_min_bound, renderer.container_max_bound);
  physics.ToggleTopCollision(true);
  renderer.UpdateParticles(physics);

  // Input
  Input input;

  while (app.isActive()) {
    // Calculate delta time
    auto current_time = std::chrono::high_resolution_clock::now();
    delta_time = std::chrono::duration<float>(current_time - last_frame_time).count();
    last_frame_time = current_time;

    physics.SetDeltaTime(delta_time);

    input.PollEvents(app, camera, renderer, physics, delta_time);

    // Prepares gl buffers etc.
    renderer.PrepareDraw(shader);
    camera.UpdateMatrices();

    // Update Renderer / physics
    renderer.UpdateCamera(shader, camera);
    renderer.UpdateParticles(physics);

    // Draw
    renderer.DrawContainer(shader);
    renderer.DrawParticles(shader);

    // swap front & back buffers
    app.update();
  }

  renderer.CleanUp();
  shader.destroy();
  app.Terminate();

  std::cout << "Goodbye!" << std::endl;
  return 0;
}