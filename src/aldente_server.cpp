#include "aldente_server.h"

#include "timer.h"
#include <chrono>
#include <iostream>

void AldenteServer::start() {
    Timer timer(GAME_TICK);
    Timer::provide(&timer);

    Timer::get()->do_after(std::chrono::seconds(1), [](std::chrono::duration<double> d) {
        std::cout << "print me once after one second" << std::endl;
    });

    const std::function<void()> cancel = Timer::get()->do_every(std::chrono::milliseconds(500),
                                                               [&cancel](std::chrono::duration<double> d) {
        static int count = 0;
        std::cout << "print me every 0.5 seconds: (" << ++count << " / 5)" << std::endl;
        if (count >= 5) cancel();
    });

    while (true) {
        Timer::get()->wait();
    }
}
