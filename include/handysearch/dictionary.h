#pragma once
#include <string>
#include <functional>
#include <memory>

namespace handysearch
{

class dictionary {
public:
  dictionary();
  ~dictionary();
  void load(std::string const &dictionary_file, std::function< void(std::uint64_t, std::uint64_t) > report_hook = nullptr);
  bool has_word(std::string const &word) const;
  std::uint64_t get_frequency(std::string const &word) const;
  std::uint64_t total_items() const;

private:
  class impl;
  std::unique_ptr<impl> m_impl;
};

}
