#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <string.h>

class App {
public:
    App(int width, int height, const char* windowTitle);

    void Initialize();
    void Terminate();

    // Setters
    void setActive(bool value);
    void setWidth(int value);
    void setHeight(int value);
    void setRelativeMode(bool value);
    void update();

    // Getters
    float getAspectRatio() const;
    int getWidth();
    int getHeight();
    SDL_Window* getWindow();
    SDL_GLContext getContext();
    bool isActive();

private:
    const char* windowTitle;
    int screenWidth;
    int screenHeight;
    SDL_Window* window = nullptr;
    SDL_GLContext openGLContext = nullptr;
    bool active;
};

#endif