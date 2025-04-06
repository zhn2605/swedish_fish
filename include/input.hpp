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
    Input();
    void PollEvents(App& app, Camera& camera, Renderer& renderer, Physics& physics, float dt);

private:
    void SpawnParticle(Camera& camera, Renderer& renderer);
    void HandleMovement(const bool* keystates, Camera& camera, float dt);
    void HandleKeyAction(const bool* keystates, Renderer& renderer, Physics& physics);
    void HandlePollAction(SDL_Event& event, Camera& camera, Renderer& renderer);

    bool p_key_down = false;
    bool g_key_down = false;
    bool minus_key_down = false;
    bool equals_key_down = false;
    bool r_key_down = false;
    bool lalt_key_down = false;
};

#endif