#pragma once

#include <fstream>
#include <string>

/** @brief Prints error when unable to collect info from source.
 * @param info_source_name information source name
 */
void print_collection_error(const std::string& info_source_name);

/** @brief Check if provided ifstream was opened or throw exception if not
 * @param std::ifstream reference to instance of ifstream
 * @param std::string name of source for ifstream
 */
void check_fs_is_open_or_throw(const std::ifstream &, const std::string &);