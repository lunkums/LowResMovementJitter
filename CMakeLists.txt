cmake_minimum_required(VERSION 3.15...3.31)
project(
  fibonacci-game-dev
  VERSION 0.0.1
  DESCRIPTION "Fibonacci Game Development"
  LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG preview-3.1.8)
FetchContent_MakeAvailable(SDL3)

add_subdirectory(src)
