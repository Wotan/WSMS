#include <iostream>
#include <cassert>
#include <stdint.h> // always include before SDL.h (SDL2 -_-)
#include <SDL.h>

#include "SDLRenderer.hh"

SDLRenderer::SDLRenderer()
{
}

SDLRenderer::~SDLRenderer()
{
}

bool SDLRenderer::init()
{
  int x = 256;
  int y = 192;

  SDL_Init(SDL_INIT_VIDEO);
  int ret = SDL_CreateWindowAndRenderer(x,
                                        y,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL,
                                        &_win,
                                        &_renderer);
  if (ret == -1) {
    std::cerr << SDL_GetError() << std::endl;
    return false;
  }
  _screenTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_STREAMING, x, y);
  assert(_screenTexture);
  return true;
}

void SDLRenderer::update()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;
    default:
      break;
    }
  }
}

void SDLRenderer::render(uint32_t const* screen)
{
  int pitch;
  void* text = 0;

  SDL_LockTexture(_screenTexture, NULL, &text, &pitch);
  for (int i = 0; i < 192 * 256; ++i)
    static_cast<uint32_t*>(text)[i] = screen[i];
  SDL_UnlockTexture(_screenTexture);

  SDL_RenderClear(_renderer);
  SDL_RenderCopy(_renderer, _screenTexture, NULL, NULL);
  SDL_RenderPresent(_renderer);
  //SDL_Delay(1);
}

void SDLRenderer::shutdown()
{
  SDL_Quit();
}
