#include <iostream>

#include <app.hpp>
#include <shader.hpp>
#include <renderer.hpp>
#include <particle.hpp>

int main(void) {
  std::string vertexFilePath = "./include/shader/vert.glsl";
  std::string fragmentFilePath = "./include/shader/frag.glsl";

  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();

  Shader shader(vertexFilePath, fragmentFilePath);
  Renderer renderer(shader);

  Particle particle(1.0f, 36, 18);
  particle.Initialize();

  renderer.AddParticle(particle);

  while (app.isActive()) {
    app.update();
    renderer.PrepareDraw();
    // renderer.TestSquare();
    renderer.DrawObject(particle.getVertices(), particle.getIndices());
  }

  renderer.CleanUp();
  app.Terminate();

  std::cout << "Goodbye!" << std::endl;
  return 0;
}