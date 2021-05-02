#include <iostream>
#include <splatdotink.hpp>

int main() {
    auto schedules = splatdotink::Schedules();

    for (auto rotation : schedules.turf()) {
        std::cout << rotation.stage_a().name() << ":" << rotation.mode() << std::endl;
    }
    for (auto rotation : schedules.ranked()) {
        std::cout << rotation.stage_a().name() << ":" << rotation.mode() << std::endl;
    }
    for (auto rotation : schedules.league()) {
        std::cout << rotation.stage_a().name() << ":" << rotation.mode() << std::endl;
    }
}
