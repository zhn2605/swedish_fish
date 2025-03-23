#include <iostream>

#include <app.hpp>
#include <shader.hpp>
#include <renderer.hpp>
#include <particle.hpp>
#include <physics.hpp>
#include <camera.hpp>

int main(void) {
  std::string vertexFilePath = "./include/shader/vert.glsl";
  std::string fragmentFilePath = "./include/shader/frag.glsl";

  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();

  Shader shader(vertexFilePath, fragmentFilePath);
  Camera camera(1.0f); // sensitivity
  camera.SetAspectRatio(app.getAspectRatio());

  printf("%f", app.getAspectRatio());

  Renderer renderer(shader, camera);

  Particle particle(1.0f, 36, 18);
  particle.Initialize();
  particle.setPosition(0.0f, 0.0f, 25.0f);

  renderer.AddParticle(particle);
  renderer.UpdateParticles();

  while (app.isActive()) {
    app.update();

    // Prepares gl buffers etc.
    renderer.PrepareDraw();
    camera.UpdateMatrices();

    // 
    renderer.UpdateCamera();


    // renderer.TestSquare();
    renderer.UpdateParticles();
    renderer.DrawParticles();
  }

  renderer.CleanUp();
  app.Terminate();

  std::cout << "Goodbye!" << std::endl;
  return 0;
}