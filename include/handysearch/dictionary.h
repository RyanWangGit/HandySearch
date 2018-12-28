#pragma once
#include <string>
#include <functional>
#include <unordered_map>

namespace handysearch
{

class dictionary {
public:
  void load(const std::string &dictionary_file, std::function< void(std::uint64_t, std::uint64_t) > report_hook = nullptr);

private:
  //std::unordered_map
};

}
