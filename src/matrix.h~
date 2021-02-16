/*
  simple matrix implementation

  author  : jannis schürmann
  date    : 
  to-do   :
  caution : 

  program :
*/

#ifndef _MATRIX_H_
#define _MATRIX_H_

// standard
#include <vector>
#include <cstddef>

template <typename T>
class matrix {
public :
  matrix(std::size_t rows, std::size_t cols) : _rows(rows), _cols(cols), _v_data(rows*cols) {}
  
  T  at(std::size_t row, std::size_t col) const { return _v_data[crd2idx(row, col)]; }
  T& at(std::size_t row, std::size_t col)       { return _v_data[crd2idx(row, col)]; }
  
protected :
  std::size_t _rows, _cols;
  std::vector <T> _v_data;
  
  std::size_t crd2idx(std::size_t row, std::size_t col) const {
    return row * _cols + col;
  }
};

#endif // _MATRIX_H_
