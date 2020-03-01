#ifndef SNAKE_LOGGER_HPP
#define SNAKE_LOGGER_HPP

#include <game/Util.hpp>

#include <iostream>

namespace snake::logger {
    template <typename ...Args>
    void info(Args&& ...args) {
        std::cout << util::format(
            "[{}] [Logger] [Info]: ",
            util::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }

    template <typename ...Args>
    void warning(Args&& ...args) {
        std::cout << util::format(
            "[{}] [Logger] [Warning]: ",
            util::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }

    template <typename ...Args>
    void error(Args&& ...args) {
        std::cout << util::format(
            "[{}] [Logger] [Error]: ",
            util::get_current_timestamp());

        (((std::cout << args), ...), std::cout << "\n");
    }
} // namespace snake::logger

#endif //SNAKE_LOGGER_HPP
