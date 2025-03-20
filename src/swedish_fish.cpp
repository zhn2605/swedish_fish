#include <iostream>
#include <app.hpp>

int main(void) {
  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();

  while (app.isActive()) {
    app.update();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  std::cout << "Goodbye!" << std::endl;
  return 0;
}