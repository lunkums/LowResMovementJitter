#ifndef GAME_H
#define GAME_H

#include "game_state.h"

#include <SDL3/SDL.h>

class Game {
  public:
    using State = GameState;

    Game();
    ~Game();

    auto run() -> int;
    // TODO: Make this method a friend of the emscripten struct that requires it.
    auto tick(State& current) -> void;
    auto quit() -> void;

  private:
    auto current_time() -> double;

    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* render_texture{};

    double accumulator{};
    double previous_time{};

    bool initialized{false};
    bool running{false};
};

#endif // GAME_H
