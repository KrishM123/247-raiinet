#include "message_queue.h"
#include <algorithm>

std::shared_ptr<MessageQueue> MessageQueue::instance = nullptr;
std::mutex MessageQueue::instanceMutex;

MessageQueue::MessageQueue() : running(false) {}

MessageQueue::~MessageQueue() { stop(); }

// Singleton pattern
std::shared_ptr<MessageQueue> MessageQueue::getInstance() {
  std::lock_guard<std::mutex> lock(instanceMutex);
  if (!instance) {
    instance = std::make_shared<MessageQueue>();
  }
  return instance;
}

void MessageQueue::subscribe(MessageSubscriber *subscriber) {
  // Add subscriber to the list
  std::lock_guard<std::mutex> lock(subscribersMutex);
  // Only after the lock is acquired, add the subscriber to the list
  subscribers.push_back(subscriber);
}

void MessageQueue::unsubscribe(MessageSubscriber *subscriber) {
  // Remove subscriber from the list
  std::lock_guard<std::mutex> lock(subscribersMutex);
  // Only after the lock is acquired, remove the subscriber from the list
  subscribers.erase(
      std::remove(subscribers.begin(), subscribers.end(), subscriber),
      subscribers.end());
}

void MessageQueue::enqueueEvent(const GameEvent &event) {
  // Add event to the queue
  std::lock_guard<std::mutex> lock(queueMutex);
  eventQueue.push(event);
  cv.notify_one();
}

void MessageQueue::start() {
  // Start the worker thread
  if (!running) {
    running = true;
    workerThread = std::thread(&MessageQueue::processEvents, this);
  }
}

void MessageQueue::stop() {
  // Stop the worker thread
  if (running) {
    running = false;
    cv.notify_all();
    if (workerThread.joinable()) {
      workerThread.join();
    }
  }
}

void MessageQueue::processEvents() {
  // Process events
  while (running) {
    // Wait for an event to be added to the queue
    std::unique_lock<std::mutex> lock(queueMutex);
    cv.wait(lock, [this] { return !eventQueue.empty() || !running; });

    // Process events
    while (!eventQueue.empty() && running) {
      GameEvent event = eventQueue.front();
      eventQueue.pop();
      lock.unlock();

      // Notify all subscribers of the event
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
