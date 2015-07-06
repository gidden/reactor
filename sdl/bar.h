
#ifndef BAR_H_
#define BAR_H_

#include "SDL2/SDL.h"

#include "color.h"
#include "error.h"
#include "renderer.h"
#include "texture.h"

namespace sdl {

class Bar {
 public:
  typedef SDL_Rect Rect;

  Bar() : surf_(nullptr) { };
  
  Bar(Rect bounds, sdl::Color c = sdl::Color::white()) {
    Init(bounds, c);
  };

  Bar(const Bar* obj) {
    r_ = obj->r_;
    color_ = obj->color_;
    surf_ = new sdl::Surface(r_.w, r_.h);
    surf_->FillRect(NULL, color_);
  };

  void Init(Rect bounds, sdl::Color c = sdl::Color::white()) {
    r_ = bounds;
    color_ = c;
    surf_ = new sdl::Surface(bounds.w, bounds.h);
    surf_->FillRect(NULL, c);
  };
  
  Rect rect() {
    return r_;
  }

  virtual sdl::Surface* surface() {
    return surf_;
  }

 protected:
  Rect r_;
  sdl::Surface* surf_;
  sdl::Color color_;
};

} // namespace sdl

#endif
