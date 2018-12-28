#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace handysearch {

class search_core {
public:
  search_core();
  search_core(const std::string &database);
  search_core(const std::string &database, const std::string &dictionary_file);
  search_core(search_core &other);
  ~search_core();
  search_core& operator=(search_core const &other);
  void load_dictionary(const std::string &dictionary_file, std::function< void(std::uint64_t, std::uint64_t) > report_hook = nullptr);
  void load_database(const std::string &database, std::function< void(std::uint64_t, std::uint64_t) > report_hook = nullptr);
  std::uint64_t total_items() const;
};

}