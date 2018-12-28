#include <handysearch/dictionary.h>


void handysearch::dictionary::load(const std::string &dictionary_file, std::function<void(uint64_t, uint64_t)> report_hook) {
  if(report_hook)
    report_hook(1, 100);
}


