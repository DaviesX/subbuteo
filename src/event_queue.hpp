#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace subbuteo {

template <typename Event> class EventQueue {
public:
  EventQueue() = default;

  void Push(Event const &event) {
    std::unique_lock<std::mutex> lock(mutex_);
    events_.push(event);
    cond_not_empty_.notify_one();
  }

  Event Pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_not_empty_.wait(lock, [this]() { return !events_.empty(); });

    Event event = std::move(events_.front());
    events_.pop();
    return event;
  }

  void Clear() {
    std::unique_lock<std::mutex> lock(mutex_);
    events_.clear();
  }

private:
  std::queue<Event> events_;
  std::mutex mutex_;
  std::condition_variable cond_not_empty_;
};

} // namespace subbuteo
