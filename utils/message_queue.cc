#include "message_queue.h"
#include <algorithm>

std::shared_ptr<MessageQueue> MessageQueue::instance = nullptr;
std::mutex MessageQueue::instanceMutex;

MessageQueue::MessageQueue() : running(false) {}

MessageQueue::~MessageQueue() { stop(); }

std::shared_ptr<MessageQueue> MessageQueue::getInstance() {
  std::lock_guard<std::mutex> lock(instanceMutex);
  if (!instance) {
    instance = std::make_shared<MessageQueue>();
  }
  return instance;
}

void MessageQueue::subscribe(MessageSubscriber *subscriber) {
  std::lock_guard<std::mutex> lock(subscribersMutex);
  subscribers.push_back(subscriber);
}

void MessageQueue::unsubscribe(MessageSubscriber *subscriber) {
  std::lock_guard<std::mutex> lock(subscribersMutex);
  subscribers.erase(
      std::remove(subscribers.begin(), subscribers.end(), subscriber),
      subscribers.end());
}

void MessageQueue::enqueueEvent(const GameEvent &event) {
  std::lock_guard<std::mutex> lock(queueMutex);
  eventQueue.push(event);
  cv.notify_one();
}

void MessageQueue::start() {
  if (!running) {
    running = true;
    workerThread = std::thread(&MessageQueue::processEvents, this);
  }
}

void MessageQueue::stop() {
  if (running) {
    running = false;
    cv.notify_all();
    if (workerThread.joinable()) {
      workerThread.join();
    }
  }
}

void MessageQueue::processEvents() {
  while (running) {
    std::unique_lock<std::mutex> lock(queueMutex);
    cv.wait(lock, [this] { return !eventQueue.empty() || !running; });

    while (!eventQueue.empty() && running) {
      GameEvent event = eventQueue.front();
      eventQueue.pop();
      lock.unlock();

      // Notify all subscribers
      {
        std::lock_guard<std::mutex> subLock(subscribersMutex);
        for (MessageSubscriber *subscriber : subscribers) {
          if (subscriber != nullptr) {
            subscriber->notify(event);
          }
        }
      }

      lock.lock();
    }
  }
}
