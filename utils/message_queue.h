#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "game_event.h"
#include "message_subscriber.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class GameEvent;

class MessageQueue {
  static std::shared_ptr<MessageQueue> instance;
  static std::mutex instanceMutex;

  std::queue<GameEvent> eventQueue;
  std::vector<MessageSubscriber *> subscribers;
  mutable std::mutex queueMutex;
  mutable std::mutex subscribersMutex;
  std::condition_variable cv;
  std::thread workerThread;
  bool running;

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
