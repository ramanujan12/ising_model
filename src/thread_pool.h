/*
  simple thread pool implementation

  author  : jannis schürmann
  date    :
  to-do   :
  caution :

  program : copied from stack overflow
*/

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

// standard
#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <condition_variable>

// thread pool class
class thread_pool {
public :
  thread_pool(std::size_t n_threads = std::thread::hardware_concurrency())
    : _busy(0), _stop(false)
  {
    for (std::size_t i = 0; i < n_threads; ++i)
      _v_workers.emplace_back(std::bind(&thread_pool::thread_proc, this));
  }
  
  ~thread_pool(void) {
    // set stop-condition
    std::unique_lock<std::mutex> latch(_mutex);
    _stop = true;
    _cv_task.notify_all();
    latch.unlock();
    
    // terminate all threads
    for (auto& t : _v_workers)
      t.join();
  }
  
  template <class _task>
  void add_task(_task&& task) {
    std::unique_lock<std::mutex> lock(_mutex);
    _v_tasks.emplace_back(std::forward<_task>(task));
    _cv_task.notify_one();
  }
  
  void wait_finished(void) {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv_finished.wait(lock, [this] () { return _v_tasks.empty() && (_busy == 0); });
  }
  
private :
  std::vector <std::thread>          _v_workers; // worker threads
  std::deque <std::function<void()>> _v_tasks;   // tasks to be done
  std::mutex                         _mutex;
  
  // conditionals for "notifications"
  std::condition_variable            _cv_task;  
  std::condition_variable            _cv_finished;
  
  unsigned int _busy;
  bool         _stop;
  
  // worker function for threads
  void thread_proc(void) {
    while (true) {
      std::unique_lock<std::mutex> latch(_mutex);
      _cv_task.wait(latch, [this] () { return _stop || !_v_tasks.empty(); });
      if (!_v_tasks.empty()) {
	// got work. set busy.
	++_busy;
	
	// pull from queue
	auto fn = _v_tasks.front();
	_v_tasks.pop_front();
	
	// release lock. run async
	latch.unlock();
	
	// run function outside context
	fn();
	
	latch.lock();
	--_busy;
	_cv_finished.notify_one();
      } else if (_stop) {
	break;
      }
    }
  }
};

#endif //_THREAD_POOL_H_
