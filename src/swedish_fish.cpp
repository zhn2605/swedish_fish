#include <iostream>

#include <app.hpp>
#include <shader.hpp>
#include <renderer.hpp>

int main(void) {
  std::string vertexFilePath = "./include/shader/vert.glsl";
  std::string fragmentFilePath = "./include/shader/frag.glsl";

  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();

  Shader shader(vertexFilePath, fragmentFilePath);
  Renderer renderer(shader);

  while (app.isActive()) {
    app.update();
    renderer.PrepareDraw();
    renderer.TestSquare();
  }

  renderer.CleanUp();
  app.Terminate();

  std::cout << "Goodbye!" << std::endl;
  return 0;
}