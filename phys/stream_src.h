#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <map>
#include <set>

#include "phys/object.h"

namespace phys {

static const double kNPS = 5e3;

enum Dir {N, S, E, W};

class StreamSource : public Object {
 public:
  StreamSource() : deltat_(1) {};

  StreamSource(const StreamSource* f) : Object(f) {
    deltat_ = f->deltat_;
  }

  Dir dir(int x, int y) {
    return E;
  }

  virtual bool OnClick(int x, int y) {
    Dir d = dir(x, y);
    if (streaming_.find(d) != streaming_.end()) {
      streaming_.erase(d);
    } else {
      streaming_.insert(d);
    }
    return true;
  }

  std::pair<int, int> Coords(Dir d, int step, int total) {
    Rect r = rect();
    double frac  = (double)step / total;
    int x, y;
    switch (d) {
      case N:
        x = (int)(r.x + frac * w);
        y = r.y;
        break;
        
      case S:
        x = (int)(r.x + frac * w);
        y = r.y - h;
        break;
        
      case E:
        x = r.x - w;
        y = (int)(r.y + frac * h);
        break;

      case W:
        x = r.x;
        y = (int)(r.y + frac * h);
        break;
    }
    return std::make_pair(x, y);
  }
  
  std::pair<int, int> Speed(Dir d) {
    int nom = phys::Neutron::kNomSpeed;
    int vx, vy;
    switch (d) {
      case N:
        vx = 0;
        vy = nom;
        break;
        
      case S:
        vx = 0;
        vy = -nom;
        break;
        
      case E:
        vx = nom;
        vy = 0;
        break;
        
      case W:
        vx = -nom;
        vy = 0;
        break;
    }
    return std::make_pair(vx, vy);
  }
  
  void Stream() {
    int n = kNPS;
    std::set<Dir>::iterator it;
    for (it = streaming_.begin(); it != streaming.end(); ++it) {
      phys::Neutron::Pop ns;
      std::pair<int, int> speed = Speed(d);
      for (int i = 0; i < n; ++i) {
        std::pair<int, int> coords = Coords(d, i, n);
        ns.push_back(phys::Neutron(coords.first, coords.second, speed.first, speed.second));
      }
      sys.AddNeutrons(ns);
    }
  }
  
  virtual void tick_info(double deltat, int n_neutrons) {
    deltat_ = deltat;
    Stream();
  }

 private:
  std::set<Dir> streaming_;
  double deltat_;
};

}

#endif
