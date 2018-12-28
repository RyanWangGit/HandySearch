#include <handysearch/handysearch.h>
#include <cstdint>
#include <string>


std::uint64_t handysearch::search_core::total_items() const {
  return 0;
}

handysearch::search_core::search_core() {

}

handysearch::search_core::search_core(const std::string &database) {

}

handysearch::search_core::search_core(const std::string &database, const std::string &dictionary_file) {

}

handysearch::search_core::search_core(handysearch::search_core &other) {

}

handysearch::search_core::~search_core() {

}

handysearch::search_core &handysearch::search_core::operator=(handysearch::search_core const &other) {
  if (this != &other) {
    
  }
  return *this;
}

void handysearch::search_core::load_dictionary(const std::string &dictionary_file, std::function< void(std::uint64_t, std::uint64_t) > report_hook) {
  if(report_hook)
    report_hook(1, 100);
}

void handysearch::search_core::load_database(const std::string &database, std::function< void(std::uint64_t, std::uint64_t) > report_hook) {
  if(report_hook)
    report_hook(1, 100);
}
