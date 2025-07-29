#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <memory>
#include "../controller/game_event.h"

class View;

class MessageQueue
{
private:
    static std::shared_ptr<MessageQueue> instance;
    static std::mutex instanceMutex;

    std::queue<GameEvent> eventQueue;
    std::vector<View *> subscribers;
    mutable std::mutex queueMutex;
    mutable std::mutex subscribersMutex;
    std::condition_variable cv;
    std::thread workerThread;
    std::atomic<bool> running;

    MessageQueue();
    ~MessageQueue();
    MessageQueue(const MessageQueue &) = delete;
    MessageQueue &operator=(const MessageQueue &) = delete;

    void processEvents();

public:
    static std::shared_ptr<MessageQueue> getInstance();

    void subscribe(View *view);
    void unsubscribe(View *view);
    void enqueueEvent(const GameEvent &event);
    void start();
    void stop();

private:
    friend struct _Sp_make_shared_tag;
};

#endif // MESSAGE_QUEUE_H