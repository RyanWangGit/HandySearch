#include <handysearch/handysearch.h>
#include <cstdint>
#include <string>


class handysearch::search_core::impl {
public:
  impl() = default;
  ~impl() = default;
};

handysearch::search_core::search_core(): m_impl(new handysearch::search_core::impl()) {

}

handysearch::search_core::search_core(std::string const &database) {

}


handysearch::search_core::search_core(handysearch::search_core &other) {

}

handysearch::search_core::~search_core() {

}

std::uint64_t handysearch::search_core::total_items() const {
  return 0;
}

handysearch::search_core &handysearch::search_core::operator=(handysearch::search_core const &other) {
  if (this != &other) {
    
  }
  return *this;
}


void handysearch::search_core::load(std::string const &database,
                                    const std::function<void(std::uint64_t, std::uint64_t)> &report_hook) {
  if(report_hook)
    report_hook(1, 100);
}

