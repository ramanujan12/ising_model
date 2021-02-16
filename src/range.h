/*
  simple range implementation

  author  : jannis schürmann
  date    :
  to-do   :
  caution :

  program :
*/

#ifndef _RANGE_H_
#define _RANGE_H_

// c++ standard headers
#include <vector>
#include <cstddef>
#include <algorithm>

// simple range class
template <typename T>
class range {
public :
  range(std::vector <T> v_points) {
    std::sort(v_points.begin(), v_points.end());
    auto last = std::unique(v_points.begin(), v_points.end());
    v_points.erase(last, v_points.end());
    _v_points = v_points;
  }
  
  range(T min, T max, std::size_t n = 1) : _v_points(n) {
    if (min > max)
      std::swap(min, max);
    
    if (n > 1) {
      T step = (max - min) / (n - 1);
      for (std::size_t idx = 0; idx < n; idx++)
	_v_points[idx] = min + idx * step;
    }
    else if (n == 0) _v_points.push_back(min);
    else if (n == 1) _v_points[0] = min;
  }
  
  std::size_t     n  (void)            { return _v_points.size();  }
  T               nth(std::size_t idx) { return _v_points[idx];    }
  std::vector <T> points(void) const   { return _v_points;         }

protected :
  std::vector <T> _v_points; 
};

#endif // _RANGE_H_
