#pragma once

#include <ostream>
#include <mutex>
#include <string>
#include <utility>

class ThreadSafeOutput {
    public:
        explicit ThreadSafeOutput(std::ostream& outputStream) : output(outputStream) {};

        // variadic template (allows for zero or more types to be passed in)
        template<typename... Args>
        // declares parameter pack (ex. int, string, float) called args
        void write(Args&&... args)
        {
            std::lock_guard<std::mutex> lock(mut);

            // fold expression, applies << to every value in arg pack
            // forward<Args>(args) does perfect forwarding (preserves orginial value)
            // ^ "pass each argument on, just as it was passed into the function"
            (output << ... << std::forward<Args>(args));
        }
    private:
        std::ostream& output;
        std::mutex mut;
};