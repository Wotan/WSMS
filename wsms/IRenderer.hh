#ifndef _IRENDERER_H_
# define _IRENDERER_H_

# include <stdint.h>

namespace WSMS {

class IRenderer {
public:
  virtual ~IRenderer() {}
  virtual bool init() = 0;
  virtual void render(uint32_t const* screen) = 0;
  virtual void update() = 0;
  virtual void shutdown() = 0;
};

} // !WSMS

#endif /* !_IRENDERER_H_ */
