#include <iostream>
#include "../include/app.hpp"

int main(void) {
  // Initialization
  App app(640, 480, "applicationTest");
  app.Initialize();

  while (app.isActive()) {
    // inf loop for testing
  }

  std::cout << "Goodbye!" << std::endl;
  return 0;
}