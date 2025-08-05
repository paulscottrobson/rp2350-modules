#include <SDL.h>

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* Window = SDL_CreateWindow(
        "Hello Window", 0, 0, 800, 300, 0);

  SDL_Event event;
  bool exit = false;
  while (!exit) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) exit = true;
  }
  SDL_Quit();
  return 0;
}