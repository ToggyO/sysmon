#pragma once

#include <algorithm> // std::find_if
#include <cmath> // std::pow, std::round
#include <sstream> // std::stringstream
#include <stdexcept> // std::runtime_error
#include <string>

/** @brief Check if string is number
 * @return bool
 */
bool is_number(const std::string &);

/** @brief Executes provided command
 * @return result of command executing
 */
std::string exec(std::string &command);

// TODO: add descr
void elapsed_time(std::string &formatted_output, size_t seconds);