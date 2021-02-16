/*
  simple ising model class

  author  : jannis schürmann
  date    :
  to-do   :
  caution :

  program : 
*/

#ifndef _ISING_H_
#define _ISING_H_

#include <iomanip>

#include "drawer.h"

class ising {
public :
  ising(std::size_t dim, std::size_t n_points, float beta, float b, std::size_t t0 = 0)
    : _beta(beta), _b(b), _g(dim, n_points), _sc(_g.size()), _t0(t0), _m_prob(2*dim+1, 2)
  { init_ising(); }

  std::pair <std::vector <float>, std::vector<float>> run(std::size_t n_steps) {
    // thermalization
    for (std::size_t t = 0; t < _t0; ++t)
      monte_carlo_step();
    
    // "simulation"
    std::vector <float> v_en(n_steps), v_ma(n_steps);
    for (std::size_t t = 0; t < n_steps; ++t) {
      monte_carlo_step();
      v_ma[t] = _sc.magnet();
      v_en[t] = _sc.energy(_g, _b);
    }
    return { v_ma, v_en };
  }
  
  void run_draw(std::size_t n_steps, std::size_t fps = 30) {
    drawer d(_g, fps);
    for (std::size_t t = 0; t < (n_steps + _t0); ++t) {
      monte_carlo_step();
      d.draw_config(_sc);
    }
  }
  
  std::string file_name(void) const {
    std::string name = "";
    name += "_dim_"  + std::to_string(_g.dim());
    name += "_L_"    + std::to_string(_g.n_points());
    name += "_beta_" + std::to_string(_beta);
    name += "_B_"    + std::to_string(_b);
    return name;
  }
    
protected :
  float            _beta, _b; // temperature and magnet field
  grid             _g;        // grid for the problem symmetry
  spin_config<int> _sc;       // spin config
  std::size_t      _t0;       // thermalization "time"
  matrix <float>   _m_prob;   // precomputed probabilities
  
  std::mt19937 prng {(unsigned int) time(NULL)};           // prng -> time(NULL) bad (obviously)
  std::uniform_real_distribution<float> dist { 0.0, 1.0 }; // distrib for probs
  
  void init_ising(void) {
    for (std::size_t i = 0; i < (2*_g.dim() + 1); ++i) {
      float b = 2*((int)_g.dim() - (int)i) + _b;
      _m_prob.at(i, 0) = 1. / (1 + std::exp(-2 * _beta * b));
      _m_prob.at(i, 1) = 1. / (1 + std::exp( 2 * _beta * b));
    }
  }
  
  void monte_carlo_step(void) {
    for (std::size_t i = 0; i < _g.size(); ++i) {
      int idx = -_g.sum_neighbours(i, _sc) / 2 + _g.dim();
      if (dist(prng) < _m_prob.at(idx, (_sc[i] == -1) ? 0 : 1))
	_sc[i] *= -1;
    }
  }
};

#endif // _ISING_H_
