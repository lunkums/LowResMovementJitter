#include "game.h"

#include <SDL3/SDL.h>

#include <chrono>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

namespace {
    struct EmMainLoopArg {
        Game* game{};
        GameState* state{};
    };

    auto emscripten_main_loop(void* loop_arg) -> void {
        auto* arg{static_cast<EmMainLoopArg*>(loop_arg)};
        arg->game->tick(*arg->state);
    }
}
#endif

Game::Game() {
    // TODO: Configure metadata at build-time using CMake
    constexpr auto name{"mg1"};
    constexpr auto version{"0.0.1"};
    constexpr auto id{"games.dogshit.mg1"};
    std::cout << name << " version " << version << '\n';

    constexpr auto compiled{SDL_VERSION};
    std::cout << "Compiled with SDL version " << SDL_VERSIONNUM_MAJOR(compiled) << '.'
              << SDL_VERSIONNUM_MINOR(compiled) << '.' << SDL_VERSIONNUM_MICRO(compiled) << '\n';
    auto linked{SDL_GetVersion()};
    std::cout << "Linking against SDL version " << SDL_VERSIONNUM_MAJOR(linked) << '.'
              << SDL_VERSIONNUM_MINOR(linked) << '.' << SDL_VERSIONNUM_MICRO(linked) << '\n';

    std::cout << "Starting up\n";

    if (!SDL_SetAppMetadata(name, version, id)) {
        std::cerr << "warning: " << SDL_GetError() << '\n';
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "fatal: " << SDL_GetError() << '\n';
        return;
    }

    // TODO: Configure window parameters at run-time
    constexpr auto title{"mg1"};
    constexpr auto width{256 * 4};
    constexpr auto height{240 * 4};
    if (!SDL_CreateWindowAndRenderer(title, width, height, 0, &window, &renderer)) {
        std::cerr << "fatal: " << SDL_GetError() << '\n';
        return;
    }

    render_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        width,
        height
    );
    if (!render_texture) {
        std::cerr << "fatal: " << SDL_GetError() << '\n';
        return;
    }
    SDL_SetTextureScaleMode(render_texture, SDL_SCALEMODE_NEAREST);

    initialized = true;
}

Game::~Game() {
    std::cout << "Shutting down\n";

    if (render_texture) {
        std::cout << "Destroying the render texture\n";
        SDL_DestroyTexture(render_texture);
    }

    if (renderer) {
        std::cout << "Destroying the renderer\n";
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        std::cout << "Destroying the window\n";
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

auto Game::run() -> int {
    if (!initialized) {
        return 1;
    }

    std::cout << "Running the game\n";
    running = true;

    // TODO: Keep a copy of the previous state to integrate with:
    // https://www.gafferongames.com/post/fix_your_timestep/
    // State previous{};
    State current{};

#ifdef __EMSCRIPTEN__
    EmMainLoopArg arg{this, &current};
    emscripten_set_main_loop_arg(emscripten_main_loop, &arg, 0, 1);
#else
    while (running) {
        tick(current);
    }
#endif

    return 0;
}

// TODO: Refactor this method if it becomes unweildy, but only after previous-state integration has
// been implemented.
auto Game::tick(State& state) -> void {
    double current{current_time()};
    double elapsed{std::min(current - previous_time, 0.25)};
    previous_time = current;

    accumulator += elapsed;

    SDL_Event e{};
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            std::cout << "Quitting the game\n";
            running = false;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
            return;
        }
        state.process_input(e);
    }

    // Apply fixed update
    constexpr auto seconds_per_update{1.0 / 60};
    while (accumulator >= seconds_per_update) {
        state.update();
        accumulator -= seconds_per_update;
    }

    // Render to target texture
    {
        SDL_SetRenderTarget(renderer, render_texture);
        // TODO: Configure clear color at run-time
        SDL_SetRenderDrawColor(renderer, 16, 16, 16, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        state.render(renderer, accumulator / seconds_per_update);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    // Render target texture to window
    {
        int w{};
        int h{};
        SDL_GetWindowSize(window, &w, &h);
        SDL_FRect src{0, 0, 256, 240};
        SDL_FRect dst{0, 0, static_cast<float>(w), static_cast<float>(h)};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, render_texture, &src, &dst);
        SDL_RenderPresent(renderer);
    }
}

auto Game::quit() -> void {
    std::cout << "Pushing quit event\n";
    SDL_Event e{SDL_EVENT_QUIT};
    SDL_PushEvent(&e);
}

auto Game::current_time() -> double {
    using namespace std::chrono;
    static auto start{high_resolution_clock::now()};
    auto now{high_resolution_clock::now()};
    return duration<double>(now - start).count();
}
