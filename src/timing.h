/*
  simple module to time things

  author  : archer
  date    :
  to-do   :
  caution :
  
  program :
*/

#ifndef _TIMING_H_
#define _TIMING_H_

// c++ standard headers
#include <vector>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <iostream>

//_________________________________________________________________________________________________
// classes in this module
class stopwatch;
class time_table;

//_________________________________________________________________________________________________
// functions contained in this module
std::string date_and_time_string(void);
std::string time_to_hh_mm_ss(std::size_t time_in_seconds);

//_________________________________________________________________________________________________
// stopwatch class
class stopwatch
{
public :
  stopwatch() = default;
  ~stopwatch() {}

  void start(void) { _t_last = std::chrono::system_clock::now(); }
  void stop (void) { _t_dur  = std::chrono::system_clock::now() - _t_last; }

  double s (void) { return std::chrono::duration_cast<std::chrono::seconds>     (_t_dur).count(); }
  double ms(void) { return std::chrono::duration_cast<std::chrono::milliseconds>(_t_dur).count(); }
  double us(void) { return std::chrono::duration_cast<std::chrono::microseconds>(_t_dur).count(); }
  double ns(void) { return std::chrono::duration_cast<std::chrono::nanoseconds> (_t_dur).count(); }
  
protected :
  std::chrono::time_point <std::chrono::system_clock> _t_last = std::chrono::time_point <std::chrono::system_clock>::max();
  std::chrono::duration <double> _t_dur;
};

//_________________________________________________________________________________________________
// stopping different sections
class time_table
{
public :
  time_table() { _t_last = std::chrono::system_clock::now(); }
  ~time_table() {}
  
  void add(std::string name_section) {
    if (name_section.length() > _max_length) {
      _max_length = name_section.length();
    }
    
    _v_p.push_back(std::make_pair(name_section, std::chrono::duration_cast<std::chrono::seconds>(_t_last - std::chrono::system_clock::now()).count()));
    _t_last = std::chrono::system_clock::now();
  }
  
  friend std::ostream& operator <<(std::ostream& out, const time_table& tt) {
    if (tt._v_p.size() != 0) {
      // header
      out << std::setw(tt._max_length) << "section" << " |" << std::setw(tt._max_length) << "time\n";
      for (std::size_t i = 0; i < 2*(tt._max_length + 1); i++) {
	out << "-";
      }
      out << "\n";
      
      // section times
      std::size_t sum_times = 0;
      for (auto &p : tt._v_p) {
	out << std::setw(tt._max_length) << p.first << " |"
	    << std::setw(tt._max_length) << time_to_hh_mm_ss(p.second) << "\n";
	sum_times += p.second;
      }

      // complete time
      out << std::setw(tt._max_length) << "complete" << " |"
	  << std::setw(tt._max_length) << time_to_hh_mm_ss(sum_times) << "\n";
    }
    return out;
  }
  
protected :
  std::chrono::time_point <std::chrono::system_clock> _t_last = std::chrono::time_point <std::chrono::system_clock>::max();
  std::vector <std::pair <std::string, std::size_t>> _v_p; // storing names and times in seconds
  std::size_t _max_length = 12; // 12 because i like trouble
};

//_________________________________________________________________________________________________
// construct a string with the date and time
std::string date_and_time_string(void)
{
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  return ss.str();
}

//_________________________________________________________________________________________________
// convert a time given in seconds to a string format of
// hh:mm:ss
std::string time_to_hh_mm_ss(std::size_t time_in_seconds)
{
  std::size_t time_hh = time_in_seconds / 3600;
  std::size_t time_mm = (time_in_seconds - time_hh*3600) / 60;
  std::size_t time_ss = time_in_seconds - time_hh*3600 - time_mm*60;

  std::string time_hh_mm_ss = std::to_string(time_hh) + "h";
  if (time_mm < 10)
    time_hh_mm_ss += "0";
  time_hh_mm_ss += std::to_string(time_mm) + "m";
  if (time_ss < 10)
    time_hh_mm_ss += "0";
  time_hh_mm_ss += std::to_string(time_ss) + "s";
  return time_hh_mm_ss;
}

#endif //_TIMER_H_
