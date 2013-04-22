#ifndef _SDLRENDERER_H_
# define _SDLRENDERER_H_

# include "IRenderer.hh"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class SDLRenderer : public WSMS::IRenderer {
public:
  SDLRenderer();
  virtual ~SDLRenderer();

  virtual bool init();
  virtual void update();
  virtual void render(uint32_t const* screen);
  virtual void shutdown();

private:
  SDL_Window* _win;
  SDL_Renderer* _renderer;
  SDL_Texture* _screenTexture;
};

#endif /* !_SDLRENDERER_H_ */
