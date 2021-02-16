/*
  simple class using SDL to draw to the screen

  author  : jannis schürmann
  date    :
  to-do   : 
  caution : 1. only works for configs with x.dim() = y.dim()

  program :
*/

#ifndef _DRAWER_H_
#define _DRAWER_H_

#include <cmath>
#include <SDL/SDL.h>

#include <thread>
#include <chrono>

#include "range.h"
#include "spin_config.h"

enum class color { white,
		   black };

std::size_t to_hex(const color& c) {
  switch (c) {
    case (color::black) : return 0;
    case (color::white) : return 0xFFFFFFFFF;
    default : throw std::logic_error("color not available.");
  }
};

class drawer {
public :
  drawer(const grid& g, std::size_t fps = 60) : _n(g.n_points()), _f_limit(1e3 / fps) {
    if (g.dim() != 2)
      throw std::logic_error("Cant' draw spin configs for grid dimension != 2");
    init_drawer();
  }
  ~drawer(void) { SDL_Quit(); }

  void draw_test(void) {
    range <std::size_t> r_color(0, 0xFFFFFFFFF, 100);
    std::cout << r_color.points().size() << std::endl;
    for (auto col : r_color.points()) {
      std::cout << col << std::endl;
      draw_rectangle(0, 0, col);
      wait_frames();
    }
  }
  
  template <typename T>
  void draw_config(const spin_config<T>& sc) {
    for (std::size_t row = 0; row < _n; ++row)
      for (std::size_t col = 0; col < _n; ++col)
	if (sc[row * _n + col] == -1) draw_rectangle(row, col, color::white);
	else                          draw_rectangle(row, col, color::black);

    wait_frames();
  }
  
protected :
  std::size_t _n, _pn = 1000; // config dimensions and pixel to be drawn
  std::size_t _pps;           // pixel per spin in each dimension
  std::size_t _t_last;        // time from the last frame
  std::size_t _f_limit;       // frame limit
  
  SDL_Surface *screen = NULL;

  void init_drawer(void) {
    // compute values for pps and recompute the total pixel width
    _pps = _pn / _n;
    _pn = _n * _pps;

    // init the screen
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
      throw std::runtime_error("video couldn't be initalized");
    screen = SDL_SetVideoMode(_pn, _pn, 32, 0);
    if (screen == NULL)
      throw std::runtime_error("could not set up screen");

    _t_last = SDL_GetTicks();
  }
  
  void draw_rectangle(const std::size_t& row, const std::size_t& col, color c) {
    for (size_t i = 0; i < _pps; i++)
      for (size_t j = 0; j < _pps; j++)
	draw_pixel(_pps*row + i, _pps*col + j, c);
  }

  void draw_rectangle(const std::size_t& row, const std::size_t& col, std::size_t color) {
    for (size_t i = 0; i < _pps; i++)
      for (size_t j = 0; j < _pps; j++)
	draw_pixel(_pps*row + i, _pps*col + j, color);
  }

  void draw_pixel(const std::size_t& x, const std::size_t& y, std::size_t color) {
    uint32_t *row32;
    row32  = (uint32_t*) (screen->pixels + 4*(x + y * _pn));
    *row32 = (uint32_t) color;
  }
  
  void draw_pixel(const std::size_t& x, const std::size_t& y, color c) {
    uint32_t *row32;
    row32  = (uint32_t*) (screen->pixels + 4*(x + y * _pn));
    *row32 = (uint32_t) to_hex(c);
  }

  void wait_frames(void) {
    if (SDL_Flip(screen) == -1)
      throw std::logic_error("SDL couldn't draw to screen.");

    auto dur = SDL_GetTicks() - _t_last;
    if (dur < _f_limit)
      SDL_Delay(_f_limit - dur);
    _t_last = SDL_GetTicks();
  }
};

#endif // _DRAWER_H_
