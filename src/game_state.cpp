#include "game_state.h"

#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_pixels.h"

#include <algorithm>
#include <cmath>

auto GameState::process_input(const SDL_Event& event) -> void {
    switch (event.type) {
        // TODO: Implement actions on key press (attack, use item)
        default:
            break;
    }

    auto* const state{SDL_GetKeyboardState(nullptr)};

    gamepad =
        {state[SDL_SCANCODE_W], state[SDL_SCANCODE_S], state[SDL_SCANCODE_A], state[SDL_SCANCODE_D]
        };
}

auto GameState::update() -> void {
    previous_pos = current_pos;

    auto x{gamepad.right - gamepad.left};
    // auto y{x ? 0 : (gamepad.down - gamepad.up)};
    auto y{gamepad.down - gamepad.up};
    player.vel_x = static_cast<float>(x) * player.speed;
    player.vel_y = static_cast<float>(y) * player.speed;

    current_pos.x += player.vel_x;
    current_pos.y += player.vel_y;

    current_pos.x = std::max(current_pos.x, 0.0f);
    current_pos.x = std::min(current_pos.x, 240.0f);
    current_pos.y = std::max(current_pos.y, 0.0f);
    current_pos.y = std::min(current_pos.y, 224.0f);
}

auto lerp(float a, float b, float t) -> float {
    return (1.0f - t) * a + t * b;
}

auto GameState::render(SDL_Renderer* renderer, double alpha) -> void const {
    float t{static_cast<float>(alpha)};
    Vec2 interp_pos{
        lerp(previous_pos.x, current_pos.x, t),
        lerp(previous_pos.y, current_pos.y, t),
    };
    SDL_FRect r{std::round(interp_pos.x), std::round(interp_pos.y), 16, 16};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &r);
}
