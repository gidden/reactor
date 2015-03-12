
#include <iostream>

#include "SDL2/SDL.h"

#include "sdl/error.h"
#include "sdl/window.h"
#include "sdl/renderer.h"
#include "sdl/texture.h"
#include "sdl/sdl_init.h"
#include "sdl/color.h"
#include "sdl/timer.h"

#include "phys/system.h"
#include "phys/basic_material.h"

#include "draw/sys_view.h"

#include "fuel.h"

using sdl::Color;

int main(int argc, char** argv) {
  std::ranlux48_base rand_gen;
  std::uniform_real_distribution<> uniform01;

  try {
    sdl::SDLinit init(SDL_INIT_EVERYTHING);

    // create window and renderer
    int w = 850;
    int h = 650;
    sdl::Window win("Reactor", w, h, 0);
    win.Center();
    win.Show();
    sdl::Renderer ren(win, SDL_RENDERER_ACCELERATED);

    // create material geometry
    phys::BasicMaterial m1{0, 0, .1, 1, 0};
    phys::Object::Rect r1{w/2 - 40, h/2 - 40, 120, 120};
    phys::Object reflector(&m1, r1, sdl::Color::white());
    phys::Object reflector2(reflector);
    phys::Object reflector3(reflector);
    phys::Object reflector4(reflector);

    phys::BasicMaterial m2{.1, 0, 0, 1, 0};
    phys::Object::Rect r2{w/2 - 130, h/2 - 40, 80, 80};
    phys::Object absorber(&m2, r2, sdl::Color::blue());
    phys::Object absorber2(absorber);
    phys::Object absorber3(absorber);
    phys::Object absorber4(absorber);

    phys::BasicMaterial m3{0, 0, .03, .3, 0};
    phys::Object::Rect r3{w/2 + 50, h/2 - 40, 80, 80};
    phys::Object moderator(&m3, r3, sdl::Color::green());
    phys::Object moderator2(moderator);
    phys::Object moderator3(moderator);
    phys::Object moderator4(moderator);

    Fuel m4(0, 0.05, 2);
    phys::Object::Rect r4{w/2 - 20, h/2 + 50, 40, 40};
    phys::Object fuel(&m4, r4, sdl::Color::purple());
    phys::Object fuel2(fuel);
    phys::Object fuel3(fuel);
    phys::Object fuel4(fuel);
    phys::Object fuel5(fuel);
    phys::Object fuel6(fuel);
    phys::Object fuel7(fuel);
    phys::Object fuel8(fuel);
    phys::Object fuel9(fuel);

    // create system and a view for drawing it
    phys::System sys(w, h);
    sys.AddObject(&reflector);
    sys.AddObject(&reflector2);
    sys.AddObject(&reflector3);
    sys.AddObject(&reflector4);
    sys.AddObject(&absorber);
    sys.AddObject(&absorber2);
    sys.AddObject(&absorber3);
    sys.AddObject(&absorber4);
    sys.AddObject(&moderator);
    sys.AddObject(&moderator2);
    sys.AddObject(&moderator3);
    sys.AddObject(&moderator4);
    sys.AddObject(&fuel);
    sys.AddObject(&fuel2);
    sys.AddObject(&fuel3);
    sys.AddObject(&fuel4);
    sys.AddObject(&fuel5);
    sys.AddObject(&fuel6);
    sys.AddObject(&fuel7);
    sys.AddObject(&fuel8);
    sys.AddObject(&fuel9);
    draw::SysView view(&sys, &ren);

    // start up the main loop
    SDL_Event ev;
    sdl::Timer timer;
    timer.Start();
    bool done = false;
    bool dragging = false;
    phys::Object* dragged = nullptr;
    while(!done) {
      // process events
      while(SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
          done = true;
        } else if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_RIGHT) {
          // create some neutrons
          phys::Neutron::Pop ns;
          for (int i = 0; i < 5000; ++i) {
            double theta = uniform01(rand_gen) * 2 * 3.141592654;
            int vx = std::cos(theta) * phys::Neutron::kNomSpeed;
            int vy = std::sin(theta) * phys::Neutron::kNomSpeed;
            ns.push_back(phys::Neutron(ev.button.x, ev.button.y, vx, vy));
          }
          sys.AddNeutrons(ns);
        } else if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
          dragged = sys.ObjectFor(ev.button.x, ev.button.y);
          sys.MoveTop(dragged);
          dragging = true;
        } else if (ev.type == SDL_KEYDOWN) {
          if (dragged == nullptr) {
            continue;
          }
          if (ev.key.keysym.sym == SDLK_DOWN) {
            dragged->Shift(0, 5);
          } else if (ev.key.keysym.sym == SDLK_UP) {
            dragged->Shift(0, -5);
          } else if (ev.key.keysym.sym == SDLK_LEFT) {
            dragged->Shift(-5, 0);
          } else if (ev.key.keysym.sym == SDLK_RIGHT) {
            dragged->Shift(5, 0);
          }
        } else if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
          dragging = false;
        } else if (ev.type == SDL_MOUSEMOTION && dragging) {
          dragged->Shift(ev.motion.xrel, ev.motion.yrel);
        }
      }

      double dt = (double)timer.Mark();
      sys.Tick(dt / 1000);
      view.Render(((double)1000) / dt);
    }

    return 0;
  } catch (sdl::FatalErr err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


