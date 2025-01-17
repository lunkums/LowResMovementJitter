#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL3/SDL.h>

struct Vec2 {
    float x{256 / 2.0};
    float y{240 / 2.0};
};

struct Player {
    float speed{2.3829};

    float vel_x{0};
    float vel_y{0};
};

struct Gamepad {
    int up{};
    int down{};
    int left{};
    int right{};

    // TODO: Add button status here using a bit set
};

struct Inventory {};

struct Pickup {};

class GameState {
  public:
    auto process_input(const SDL_Event& event) -> void;
    auto update() -> void;
    auto render(SDL_Renderer* renderer, double alpha) -> void const;

  private:
    Player player{};
    Gamepad gamepad{};
    Inventory inventory{};
    Pickup pickup{};

    Vec2 current_pos{};
    Vec2 previous_pos{};
};

#endif // GAME_STATE_H
