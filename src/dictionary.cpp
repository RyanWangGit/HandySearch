#include <handysearch/dictionary.h>
#include <fstream>
#include <iostream>
#include <unordered_map>


class handysearch::dictionary::impl {
public:
  impl(): m_total_items(0) { };
  ~impl() = default;
  std::unordered_map<std::string, std::uint64_t> m_internal_map;
  std::uint64_t m_total_items;
};

handysearch::dictionary::dictionary(): m_impl(new handysearch::dictionary::impl()) { }

handysearch::dictionary::~dictionary() = default;

void handysearch::dictionary::load(std::string const &dictionary_file, std::function<void(uint64_t, uint64_t)> report_hook) {
  std::fstream fs;
  fs.open(dictionary_file, std::fstream::in);
  if(!fs.is_open())
  {
    std::cerr << "Cannot open dictionary file" << dictionary_file << std::endl;
    return;
  }
  // prepare the internal map
  fs >> this->m_impl->m_total_items;
  this->m_impl->m_internal_map.reserve(this->m_impl->m_total_items * sizeof(std::string));

  std::uint64_t current = 1;
  while(!fs.eof())
  {
    std::string word;
    std::uint64_t freq;
    fs >> word >> freq;
    this->m_impl->m_internal_map[word] = freq;
    if(report_hook)
      report_hook(current, this->m_impl->m_total_items);
    current ++;
  }
}

bool handysearch::dictionary::has_word(std::string const &word) const{
  return this->m_impl->m_internal_map.find(word) != this->m_impl->m_internal_map.end();
}

std::uint64_t handysearch::dictionary::get_frequency(std::string const &word) const{
  if(!this->has_word(word))
    return 0;
  else
    return this->m_impl->m_internal_map.at(word);
}

std::uint64_t handysearch::dictionary::total_items() const {
  return this->m_impl->m_total_items;
}


