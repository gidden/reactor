
#ifndef BASIC_MATERIAL_H_
#define BASIC_MATERIAL_H_

#include <math.h>

#include "phys/material.h"

namespace phys {

// all probabilities given per unit length
class BasicMaterial : public Material {
 public:
  BasicMaterial(double pa, double pf, double ps, double sf, double y)
    : uniform_(0, 2 * kPi),
      p_absorb(pa),
      p_fiss(pf),
      p_scatter(ps),
      scat_frac(sf),
      yield(y) { };

  virtual double absorb_prob(double speed) {
    return p_absorb;
  };

  virtual double fiss_prob(double speed) {
    return p_fiss;
  };

  virtual double scatter_prob(double speed) {
    return p_scatter;
  };

  virtual Neutron::V scat_v(Neutron::V v, double speed) {
    double theta = uniform_(rand_gen_);
    double vy = speed * std::sin(theta);
    double vx = speed * std::cos(theta);

    if (speed > 5) {
      vx *= scat_frac;
      vy *= scat_frac;
    }
    return Neutron::V {vx, vy};
  };

  virtual int fiss_yield() {
    return yield;
  };

  double p_absorb;
  double p_fiss;
  double p_scatter;
  double scat_frac;
  int yield;

 private:
  std::ranlux48_base rand_gen_;
  std::uniform_real_distribution<> uniform_;
};

} // namespace phys

#endif
