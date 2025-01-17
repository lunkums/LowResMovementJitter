#include "game.h"

#include <SDL3/SDL_main.h>

auto main(int argc, char* argv[]) -> int {
    auto status{Game{}.run()};
    return status;
}
