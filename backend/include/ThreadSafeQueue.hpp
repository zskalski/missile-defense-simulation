#pragma once
#include <queue>
#include <mutex>
#include <optional>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
    public:
        ThreadSafeQueue();
        bool push(const T& value);
        std::optional<T> pop();
        std::optional<T> popOrWait();
        void close();
    private:
        std::queue<T> queue;
        std::mutex mut;
        std::condition_variable condition;
        bool open;
};

template <typename T>
ThreadSafeQueue<T>::ThreadSafeQueue():open(true) {}

template <typename T>
bool ThreadSafeQueue<T>::push(const T& value) {
    {
        std::lock_guard<std::mutex> lock(mut);          // uses lock for entire scope
        if (!open)
            return false;
        queue.push(value);
    }
    condition.notify_one();     // notify thread when message is placed in queue
    return true;
}

template <typename T>
std::optional<T> ThreadSafeQueue<T>::pop() {
    std::lock_guard<std::mutex> lock(mut);
    if (queue.empty())
        return std::nullopt;
    
    T value = std::move(queue.front());
    queue.pop();
    return value;
}

template <typename T>
std::optional<T> ThreadSafeQueue<T>::popOrWait() {
    std::unique_lock<std::mutex> lock(mut);     // has lock and unlock functions

    // sleep thread until queue is not empty
    condition.wait(lock, [this] {
        return !queue.empty() || !open;
    });

    // if queue is not empty after close, empty it to ensure graceful shutdown
    // not useful for now, but it can be if save feature is added in the future
    if(!open && queue.empty())
        return std::nullopt;

    T value = std::move(queue.front());
    queue.pop();
    return value;
}

template <typename T>
void ThreadSafeQueue<T>::close() {
    {
        std::lock_guard<std::mutex> lock(mut);
        open = false;
    }
    condition.notify_all();
}