#pragma once

#include <map>
#include <vector>
#include <chrono>
#include <functional>

#define GAME_TICK std::chrono::milliseconds(30)

// A single-threaded event loop style timer.
class Timer {
typedef std::chrono::time_point<std::chrono::system_clock> Time;
typedef std::chrono::duration<double> Duration;

public:
    Timer(Duration tick);

    // Setter/getter for static instance
    static void provide(Timer *timer);
    static Timer *get();

    // Runs all timed out callbacks and causes the timer to block the thread until the next tick interval
    void wait();

    // Run callback once after the specified duration in seconds.
    // The callback receives as a parameter the number of seconds after the requested callback time that the
    // callback was actually executed.
    // Returns a function that can be called to cancel the operation.
    std::function<void()> do_after(const Duration time, const std::function<void(Duration)> callback);

    // Run callback every specified duration in seconds.
    // Duration will be adjusted to be as close to schedule as possible.
    // The callback receives as a parameter the number of seconds after the requested callback time that the
    // callback was actually executed.
    // Returns a function that can be called to cancel the operation.
    std::function<void()> do_every(const Duration time, const std::function<void(Duration)> callback);

private:
    static Timer *instance;

    Duration tick;
    Time last_tick;

    struct Operation {
        const Duration when; // When to fire
        Time last_check; // Last time this Operation was checked
        Duration remaining; // Mutable time remaining until operation execution
        const std::function<void(Duration)> callback;
    };

    // Callback maps from ID -> function
    std::map<int, Operation> afters;
    std::map<int, Operation> everys;

    // Queue for callback IDs to cancel
    std::vector<int> to_cancel;

    // Helper to register operations
    std::function<void()> add_op(std::map<int, Operation> &to, const Duration time,
                                 const std::function<void(Duration)> callback);

    // Helper to handle operations
    void operate(std::map<int, Operation> &ops, bool remove_when_executed);
};
