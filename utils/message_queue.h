#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "game_event.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class GameEvent;

class MessageSubscriber {
public:
    virtual ~MessageSubscriber() = default;
    virtual void notify(GameEvent &event) = 0;
};

class MessageQueue {
private:
  static std::shared_ptr<MessageQueue> instance;
  static std::mutex instanceMutex;

  std::queue<GameEvent> eventQueue;
  std::vector<MessageSubscriber *> subscribers;
  mutable std::mutex queueMutex;
  mutable std::mutex subscribersMutex;
  std::condition_variable cv;
  std::thread workerThread;
  std::atomic<bool> running;

  void processEvents();

public:
  MessageQueue();
  ~MessageQueue();
  MessageQueue(const MessageQueue &) = delete;
  MessageQueue &operator=(const MessageQueue &) = delete;
  static std::shared_ptr<MessageQueue> getInstance();

  void subscribe(MessageSubscriber *subscriber);
  void unsubscribe(MessageSubscriber *subscriber);
  void enqueueEvent(const GameEvent &event);
  void start();
  void stop();
};

#endif // MESSAGE_QUEUE_H