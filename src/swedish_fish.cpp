#include <iostream>
#include <app.hpp>

int main(void) {
  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();

  while (app.isActive()) {
    app.update();
  }

  std::cout << "Goodbye!" << std::endl;
  return 0;
}