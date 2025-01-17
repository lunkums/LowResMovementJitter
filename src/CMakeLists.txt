add_executable(mg1 main.cpp game.cpp game_state.cpp)
target_compile_features(mg1 PRIVATE cxx_std_17)
target_link_libraries(mg1 PRIVATE SDL3::SDL3)

if(EMSCRIPTEN)
  set_target_properties(
    mg1
    PROPERTIES
      LINK_FLAGS
      "-sUSE_SDL=3 -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -sALLOW_MEMORY_GROWTH=1"
  )
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()
