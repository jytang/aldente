#include "timer.h"

#include <assert.h>
#include <thread>

namespace chrono = std::chrono;
using chrono::system_clock;

Timer::Timer(Duration tick)
        : tick(tick),
          last_tick(system_clock::now()) {}

Timer *Timer::instance = nullptr;

void Timer::provide(Timer *timer) {
    instance = timer;
}

Timer *Timer::get() {
    assert(instance);
    return instance;
}

void Timer::wait() {
    // Handle cancels
    for (int id : to_cancel) {
        // IDs are shared among afters/everys, so just erase from both
        afters.erase(id);
        everys.erase(id);
    }
    to_cancel.clear();

    // Handle callbacks
    operate(afters, true);
    operate(everys, false);

    // Calculate how long to wait
    Time now = system_clock::now();
    Duration delta = now - last_tick;
    Duration to_wait = tick - delta;

    // Block until next tick
    assert(to_wait >= chrono::milliseconds(0));
    std::this_thread::sleep_for(to_wait);
    last_tick = system_clock::now();
}

void Timer::operate(std::map<int, Operation> &ops, bool remove_when_executed) {
    // Iterate and possibly modify map
    for (auto &p : ops) {
        int id = p.first;
        Operation &op = p.second;

        // Update remaining time
        Time now = system_clock::now();
        op.remaining -= now - op.last_check;
        op.last_check = now;

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

std::function<void()> Timer::do_after(const Duration time, const std::function<void(Duration)> callback) {
    return add_op(afters, time, callback);
}

std::function<void()> Timer::do_every(const Duration time, const std::function<void(Duration)> callback) {
    return add_op(everys, time, callback);
}

std::function<void()> Timer::add_op(std::map<int, Operation> &to, const Duration time,
                                    const std::function<void(Duration)> callback) {
    static int id = 0;
    int cur_id = ++id;
    to.emplace(cur_id, Operation{time, system_clock::now(), time, callback});

    // Return a function which will add the corresponding ID to the cancel queue
    return [this, cur_id]() {
        to_cancel.push_back(cur_id);
    };
}
