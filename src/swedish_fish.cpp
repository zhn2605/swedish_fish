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
float delta_time = 0.0f;
float gravity = -9.81f;
float surface_tension = 72.0f;

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

  Particle sample_particle(0.2f, 36, 18);
  sample_particle.setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
  renderer.InitializeParticleMesh(sample_particle);
  renderer.AddParticle(sample_particle);
  
  Physics physics(delta_time, gravity, surface_tension);
  physics.SetBounds(renderer.container_min_bound, renderer.container_max_bound);
  renderer.UpdateParticles(physics);

  while (app.isActive()) {
    // Calculate delta time
    auto current_time = std::chrono::high_resolution_clock::now();
    delta_time = std::chrono::duration<float>(current_time - last_frame_time).count();
    last_frame_time = current_time;

    physics.SetDeltaTime(delta_time);

    Input::PollEvents(app, camera, renderer, physics, delta_time);

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