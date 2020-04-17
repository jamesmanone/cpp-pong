#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>


// must be inline to prevent linker errors
template<class T>
class MessageQueue {
public:
  void send(T &&m) {
    std::unique_lock<std::mutex> l(_mtx);
    _q.emplace_back(m);
    l.unlock();
    _c_v.notify_one();
  }
  T receive() {
    std::unique_lock<std::mutex> l(_mtx);
    _c_v.wait(l, [this] { return !this->_q.empty(); });
    T m = std::move(_q.front());
    _q.pop_front();
    return std::move(m);
  }
private:
  std::deque<T> _q;
  std::mutex _mtx;
  std::condition_variable _c_v;
};

#endif