#pragma once

#include <fstream>
#include <string>

/** @brief Prints error when unable to collect info from source.
 * @param info_source_name information source name
 */
void print_collection_error(const std::string& info_source_name);

/** @brief Check if provided fstream was opened or throw exception if not
 * @param std::fstream reference to instance of fstream
 * @param std::string name of source for fstream
 */
void check_fs_is_open_or_throw(const std::fstream &, const std::string &);