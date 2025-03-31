#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <SDL.h>

#include <app.hpp>
#include <camera.hpp>
#include <renderer.hpp>
#include <physics.hpp>
#include <particle.hpp>


class Input {
public:
    static void PollEvents(App& app, Camera& camera, Renderer& renderer, Physics& physics, float dt);

private:
    static void SpawnParticle(Camera& camera, Renderer& renderer);
    static void HandleMovement(const bool* keystates, Camera& camera, float dt);
    static void HandlePollAction(SDL_Event& event, Camera& camera, Renderer& renderer);
};

#endif