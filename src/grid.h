/*
  multidimensional grid with N points in each direction

  author  : jannis schürmann
  date    :
  to-do   :
  caution : 

  program :
*/

#ifndef _GRID_H_
#define _GRID_H_

// standard
#include <cmath>
#include <string>
#include <cstddef>
#include <iostream>
#include <stdexcept>

// own
#include "matrix.h"

enum class sum_type { all,    // sum all neighbours
		      prev,   // sum previous neighbours
		      next }; // sum next neighbours

template <typename T> class spin_config;

class grid {
public :
  grid(std::size_t dim, std::size_t n_points)
    : _dim(dim), _n_points(n_points), _m_neigh(std::pow(n_points, dim), 2*dim), _v_powers(dim, 1)
  { init_grid(); }
  
  std::size_t dim     (void) const { return _dim;                       }
  std::size_t size    (void) const { return _v_powers.back()*_n_points; }
  std::size_t n_points(void) const { return _n_points;                  }
  
  template <typename T, sum_type st = sum_type::all>
  T sum_neighbours(const std::size_t& idx, const spin_config <T>& sc) const {
    T sum(0);
    std::size_t n_start = (st == sum_type::next) ? 1 : 0;
    std::size_t n_plus  = (st == sum_type::all)  ? 1 : 2;
    for (std::size_t n = n_start; n < 2*_dim; n += n_plus)
      sum += sc[_m_neigh.at(idx, n)];
    return sum;
  }
  
protected :
  std::size_t               _dim, _n_points; // grid defining parameters
  matrix <std::size_t>      _m_neigh;        // matrix to store the neighbours
  std::vector <std::size_t> _v_powers;       // powers of _n_points
   
  std::size_t crd2idx(const std::vector <std::size_t>& v_coord) {
    std::size_t idx = 0;
    for (std::size_t i = 0; i < v_coord.size(); ++i)
      idx += v_coord[i] * _v_powers[i];
    return idx;
  }

  void init_grid(void) {
    // compute powers for crd2idx
    for (std::size_t i = 1; i < _v_powers.size(); ++i)
      _v_powers[i] = _v_powers[i-1] * _n_points;
    
    // compute neighbours indices for a coordinate vector
    auto compute_neighbours = [&] (const std::vector <std::size_t>& v_coord) {
      std::size_t idx = crd2idx(v_coord);
      for (std::size_t d = 0; d < _dim; ++d) {
	// previous
	auto v_neigh = v_coord;
	if (v_coord[d] == 0) v_neigh[d] = _n_points  - 1;
	else                 v_neigh[d] = v_coord[d] - 1;
	_m_neigh.at(idx, 2*d) = this->crd2idx(v_neigh);
	
	// next
	v_neigh = v_coord;
	if (v_coord[d] == _n_points-1) v_neigh[d] = 0;
	else                           v_neigh[d] = v_coord[d] + 1;
	_m_neigh.at(idx, 2*d+1) = this->crd2idx(v_neigh);
      }
    };
    
    std::vector <std::size_t> v_coord(_dim, 0);
    point_recursion(v_coord, compute_neighbours);
  }
  
  template <typename _func_type>
  void point_recursion(std::vector <std::size_t>& v_coord, _func_type f, std::size_t depth = 0) {
    if (depth == v_coord.size()) {
      f(v_coord);
      return;
    }
    
    for (std::size_t n = 0; n < _n_points; ++n) {
      v_coord[depth] = n;
      point_recursion(v_coord, f, depth+1);
    }
  }
};

#endif // _GRID_H_
