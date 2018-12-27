#pragma once
#include <cstdint>
#include <string>

namespace handysearch {

class search_core {
public:
  search_core();
  search_core(const std::string &database);
  search_core(const std::string &database, const std::string &dictionary_file);
  search_core(search_core &other);
  ~search_core();
  search_core& operator=(search_core const &other);
  void load_dictionary(const std::string &dictionary_file, void (*report_hook)(std::uint64_t current, std::uint64_t total) = nullptr);
  void load_database(const std::string &database, void (*report_hook)(std::uint64_t current, std::uint64_t total) = nullptr);
  std::uint64_t total_items() const;
};

}