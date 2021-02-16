/*
  spin config class

  author  : jannis schürmann
  date    :
  to-do   :
  caution :
  
  program : 
*/

#ifndef _SPIN_CONFIG_H_
#define _SPIN_CONFIG_H_

#include <random>

#include "grid.h"

template <typename T>
class spin_config
{
public :
  spin_config(std::size_t size) : _v_data(size, -1) { init_config(); }

  T  operator [](const std::size_t& i) const { return _v_data[i]; }
  T& operator [](const std::size_t& i)       { return _v_data[i]; }

  float magnet(void) const {
    float sum = 0;
    for (auto &spin : _v_data)
      sum += spin;
    return sum / _v_data.size();
  }

  float energy(const grid& g, const float& b) const {
    float sum = 0;
    for (std::size_t i = 0; i < g.size(); ++i)
      sum -= _v_data[i] * (b + g.sum_neighbours<int, sum_type::prev>(i, *this));
    return sum / _v_data.size();
  }
  
protected :
  std::vector <T> _v_data;

  void init_config(void) {
    std::mt19937 prng {(unsigned int) time(NULL)};
    std::uniform_int_distribution <> dist {0, 1};

    for (auto &spin : _v_data)
      if (dist(prng))
	spin = 1;
  }
};

#endif // _SPIN_CONFIG_H_
