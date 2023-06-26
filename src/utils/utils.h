#pragma once

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