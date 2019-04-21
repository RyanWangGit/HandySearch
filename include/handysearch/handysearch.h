#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace handysearch {

class search_core {
public:
  search_core();
  search_core(std::string const &database);
  search_core(std::string const &database, std::string const &dictionary_file);
  search_core(search_core &other);
  ~search_core();
  search_core& operator=(search_core const &other);
  void load_dictionary(std::string const &dictionary_file);
  void load_database(std::string const &database, std::function< void(std::uint64_t, std::uint64_t) > report_hook = nullptr);
  std::uint64_t total_items() const;

private:
  class impl;
  std::unique_ptr<impl> m_impl;
};

}