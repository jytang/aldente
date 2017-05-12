#include "timer.h"

#include <assert.h>
#include <thread>

namespace chrono = std::chrono;
using chrono::system_clock;

Timer::Timer(Duration tick)
        : tick(tick)
        , last_tick(system_clock::now())
        , lag(0) {}

Timer *Timer::instance = nullptr;

void Timer::provide(Timer *timer) {
    instance = timer;
}

Timer *Timer::get() {
    assert(instance);
    return instance;
}

std::chrono::duration<double> Timer::so_far() {
    return system_clock::now() - last_tick;
}

void Timer::catch_up(const std::function<void()> &update) {
    // Calculate how long to wait
    Time now = system_clock::now();
    Duration elapsed = now - last_tick;
    last_tick = now;
    lag += elapsed;

    // Update as many times as we need to catch up
    while (lag >= tick) {
        handle_operations(tick);
        update();
        lag -= tick;
    }
}

void Timer::handle_operations(const Duration &elapsed) {
    // Handle cancels
    for (int id : to_cancel) {
        // IDs are shared among afters/everys, so just erase from both
        afters.erase(id);
        everys.erase(id);
    }
    to_cancel.clear();

    // Handle callbacks
    operate(elapsed, afters, true);
    operate(elapsed, everys, false);
}

void Timer::operate(const Duration &elapsed, std::map<int, Operation> &ops, bool remove_when_executed) {
    // Iterate and possibly modify map
    for (auto &p : ops) {
        int id = p.first;
        Operation &op = p.second;

        // Update remaining time
        op.remaining -= elapsed;

        // If all time elapsed, callback and remove or reset
        if (op.remaining <= chrono::milliseconds(0)) {
            op.callback(-op.remaining);

            if (remove_when_executed) {
                to_cancel.push_back(id);
            } else {
                op.remaining += op.when;
            }
        }
    }
}

std::function<void()> Timer::do_after(const Duration time, const std::function<void(Duration)> &callback) {
    return add_op(afters, time, callback);
}

std::function<void()> Timer::do_every(const Duration time, const std::function<void(Duration)> &callback) {
    return add_op(everys, time, callback);
}

std::function<void()> Timer::add_op(std::map<int, Operation> &to, const Duration time,
                                    const std::function<void(Duration)> &callback) {
    static int id = 0;
    int cur_id = ++id;
    to.emplace(cur_id, Operation{time, time, callback});

    // Return a function which will add the corresponding ID to the cancel queue
    return [this, cur_id]() {
        to_cancel.push_back(cur_id);
    };
}
