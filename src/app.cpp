 // I knowt this looks ugly ash i dont know why its not working
#include <app.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>

App::App(int width, int height, const char* title) {
    screenWidth = width;
    screenHeight = height;
    windowTitle = title;
}

void App::Initialize() {
    // int numDrivers = SDL_GetNumVideoDrivers();
    // std::cout << "Available SDL Video Drivers:\n";
    // for (int i = 0; i < numDrivers; ++i) {
    //     std::cout << "- " << SDL_GetVideoDriver(i) << std::endl;
    // }

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Log("SDL3 Init Error: %s\n", SDL_GetError());
        exit(1);
    } else {
        SDL_Log("Current video backend: %s", SDL_GetCurrentVideoDriver());
    }

    // Set GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window in SDL3
    window = SDL_CreateWindow(
        windowTitle,            // title name
        screenWidth,                // width
        screenHeight,               // height
        SDL_WINDOW_OPENGL        // Unit32 flags
    );

    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        exit(1);
    }

    Uint32 flags = SDL_GetWindowFlags(window);
    SDL_Log("Window Flags: 0x%X", flags);

    // Create context
    openGLContext = SDL_GL_CreateContext(window);
    SDL_Log("Using OpenGL Version: %d.%d", SDL_GL_CONTEXT_MAJOR_VERSION, SDL_GL_CONTEXT_MINOR_VERSION);

    if (!openGLContext) {
        SDL_Log("Could not create open GL Context: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize glad
    if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
        SDL_Log("Could not initialize glad: %s\n", SDL_GetError());
        exit(1);
    }

    // Set application to active
    active = true;
}

void App::Terminate() {
    SDL_GL_DestroyContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Setters
void App::setActive(bool value) {
    active = value;
}

// TODO: Move to input class 
void App::update() {
    // std::cout << "HEY MAN!!";
    SDL_GL_SwapWindow(window);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
                exit(1);
        }
    }
}

void App::setWidth(int value) {
    screenWidth = value;
}

void App::setHeight(int value) {
    screenHeight = value;
}

// Getters
SDL_Window* App::getWindow() { return window; }
SDL_GLContext App::getContext() { return openGLContext; }
bool App::isActive() { return active; }
int App::getWidth() { return screenWidth; }
int App::getHeight() { return screenHeight; }