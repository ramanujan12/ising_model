/*
  run the ising model

  author  : jannis schürmann
  date    :
  to-do   :
  caution : 1. filenames are hardcoded -> bad idea, but i don't want to invent some filenaming system
               thing between c++ and python -> not the task
	    2. hardcoded precision's -> bad idea
	    3. everything is hardcoded in this script.

  program : 
*/

// standard
#include <fstream>

// own
#include "grid.h"
#include "ising.h"
#include "range.h"
#include "timing.h"
#include "thread_pool.h"

template <typename T, typename U>
void write_data(std::string file, const std::vector <T>& v_x, const std::vector <U>& v_y) {
  std::ofstream f_out(file);
  if (!f_out) {
    throw std::runtime_error(std::string(__FUNCTION__)
			     + ": file {" + file + "} can't be openend.");
  }
  f_out << std::setprecision(8);
  for (std::size_t i = 0; i < v_x.size(); ++i)
    f_out << std::setw(20) << v_x[i] << " "
	  << std::setw(20) << v_y[i] << "\n";
}

template <typename T>
T mean(const std::vector <T>& v) {
  T sum(0);
  for (auto &val : v)
    sum += val;
  return sum / v.size();
}

void test_t0(void) {

  std::size_t n_steps = 1e3;
  std::vector <std::size_t> v_t(n_steps);
  std::iota(v_t.begin(), v_t.end(), 1);

  // ranges for the parameters to test
  range <std::size_t> r_d   (std::vector <std::size_t> {1, 2});
  range <std::size_t> r_n   (std::vector <std::size_t> {4, 10, 100, 500});
  range <float>       r_beta(std::vector <float>       {0.1, 0.5, 1.5, 3.0});
  range <float>       r_b   (std::vector <float>       {0.2});
  
  std::vector <ising> v_ising;
  for (auto &b : r_b.points())
    for (auto &dim : r_d.points())
      for (auto &beta : r_beta.points())
	for (auto &n_points : r_n.points())
	  v_ising.push_back(ising(dim, n_points, beta, b, 0));
  
  auto run_and_write = [&] (ising is) {
    auto result = is.run(n_steps);
    write_data("out_t/magnet_" + is.file_name() + ".dat", v_t, result.first);
    write_data("out_t/energy_" + is.file_name() + ".dat", v_t, result.second);
  };

  thread_pool tp;
  for (auto is : v_ising)
    tp.add_task(std::bind(run_and_write, is));
  tp.wait_finished();
}

void run_exb(void) {
  std::size_t n_steps = 1e5;
  
  //          d,  N, beta,   B,   t0
  ising is_m0(1,  4,  0.5, 0.2, 1e3);
  ising is_m1(1, 20,  0.5, 0.2, 1e3);
  ising is_m2(2,  4,  0.4, 0.2, 1e3);
  ising is_m3(2, 10,  0.4, 0.2, 1e3);
  
  std::cout << std::right << std::setprecision(4) << std::scientific;
  std::cout << std::setw(12) << "d"
	    << std::setw(12) << "N"
	    << std::setw(12) << "beta"
	    << std::setw(12) << "B"
	    << std::setw(12) << "<M>/V"
	    << std::setw(12) << "<E>/V"
	    << std::endl;

  auto out_res = [] (std::size_t d, std::size_t N, float beta, float B,
		     std::pair <std::vector<float>, std::vector <float>> res) {
    std::cout << std::setw(12) << d
    << std::setw(12) << N
    << std::setw(12) << beta
    << std::setw(12) << B
    << std::setw(12) << mean(res.first)
    << std::setw(12) << mean(res.second)
    << std::endl;
  };
  
  //      d,  N, beta,   B, result after n_steps
  out_res(1,  4,  0.5, 0.2, is_m0.run(n_steps));
  out_res(1, 20,  0.5, 0.2, is_m1.run(n_steps));
  out_res(2,  4,  0.4, 0.2, is_m2.run(n_steps));
  out_res(2, 10,  0.4, 0.2, is_m3.run(n_steps));
}

void run_exc(void) {
  std::size_t dim      = 2;
  std::size_t n_steps  = 1e4;
  std::size_t n_points = 250;
  std::size_t t0       = 1e3;
  float       b        = 0.2;
  range <float> r_kbt (1.5, 3.0, 96);
  
  std::vector <ising> v_ising;
  for (auto kbt : r_kbt.points())
    v_ising.push_back(ising(dim, n_points, b, 1./kbt, t0));

  std::vector <float> v_ma(r_kbt.n());
  auto run_ising = [&] (std::size_t i) {
    auto result = v_ising[i].run(n_steps);
    v_ma[i] = mean(result.first);
  };
  
  thread_pool tp;
  for (std::size_t i = 0; i < r_kbt.n(); ++i)
    tp.add_task(std::bind(run_ising, i));
  tp.wait_finished();

  write_data("exc.dat", r_kbt.points(), v_ma);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "input : (string) mode (-test_t0 / -exb / -exc / -draw)" << std::endl;
    return EXIT_FAILURE;
  }
  
  std::string mode(argv[1]);
  stopwatch sw;
  sw.start();
  if (mode == "-exb") {
    run_exb();
  } else if (mode == "-exc"){
    run_exc();
  } else if (mode == "-test_t0") {
    test_t0();
  } else if (mode == "-draw") {
    range <float> r_beta(1.5, 3.0, 100);
    for (auto beta : r_beta.points()) {
      ising is(2, 100, 0.2, beta);
      is.run_draw(500, 100);
    }
  } else if (mode == "-test_draw") {
    grid   g(2, 1);
    drawer d(g, 1);
    d.draw_test();
  } else {
    throw std::runtime_error(std::string(__FUNCTION__)
			     + ": choosen mode {" + + "} not available.");
  }
  sw.stop();
  std::cout << "time [s] : " << sw.s() << std::endl;
}
