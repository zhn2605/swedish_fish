#include <input.hpp>

void Input::PollEvents(App& app, Camera& camera, Renderer& renderer, Physics& physics, float dt) {
    static float mouse_x = app.getWidth()/2.0f;
    static float mouse_y = app.getHeight()/2.0f;

    // Pump events for specific keyboard state handeling
    SDL_PumpEvents();

    // Check Keyboard States
    int num_keys;
    const bool* keystates = SDL_GetKeyboardState(NULL);

    // Check for keys
    HandleMovement(keystates, camera, dt);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        HandlePollAction(e, camera, renderer);
        
        // Quit
        if (e.type == SDL_EVENT_QUIT) {
            app.setActive(false);
        }

        // Mouse Look
        else if (e.type == SDL_EVENT_MOUSE_MOTION) {
            mouse_x += e.motion.xrel;
            mouse_y += e.motion.yrel;
            camera.MouseLook(mouse_x, mouse_y);
        } 
    }
}

void Input::HandleMovement(const bool* keystates, Camera& camera, float dt) {
    float base_speed = camera.GetSpeed() * dt;
    float speed;

    // Speed multipler depending on shift
    speed = (keystates[SDL_SCANCODE_LSHIFT] || keystates[SDL_SCANCODE_RSHIFT]) ? 1.25 * base_speed : base_speed;

    // Movement Check
    if (keystates[SDL_SCANCODE_W]) { camera.MoveForward(speed); }
    if (keystates[SDL_SCANCODE_A]) { camera.MoveLeft(speed); }
    if (keystates[SDL_SCANCODE_S]) { camera.MoveBackward(speed); }
    if (keystates[SDL_SCANCODE_D]) { camera.MoveRight(speed); }
    if (keystates[SDL_SCANCODE_SPACE]) { camera.MoveUp(speed); }
    if (keystates[SDL_SCANCODE_LCTRL]) { camera.MoveDown(speed); }
}

void Input::HandlePollAction(SDL_Event& event, Camera& camera, Renderer& renderer) {
    float force = 13.0f;
    
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            Particle particle(0.2f, 36, 18);
            particle.setPosition(camera.GetEye());
            particle.setVelocity(camera.GetLookDir() * force);

            glm::vec3 vel = particle.getVelocity();

            printf("Velocity: <%f, %f, %f>\n", vel.x, vel.y, vel.z);
            renderer.AddParticle(particle);
        }
    }
}