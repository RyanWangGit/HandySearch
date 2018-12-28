#pragma once
#include <string>
#include <functional>
#include <unordered_map>

namespace handysearch
{

class dictionary {
public:
  dictionary();
  void load(const std::string &dictionary_file, std::function< void(std::uint64_t, std::uint64_t) > report_hook = nullptr);
  bool has_word(const std::string &word) const;
  std::uint64_t get_frequency(const std::string &word) const;
  std::uint64_t total_items() const;

private:
  std::unordered_map<std::string, std::uint64_t > m_internal_map;
  std::uint64_t m_total_items;
};

}
