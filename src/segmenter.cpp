#include <handysearch/segmenter.h>
#include <vector>


handysearch::segmenter::segmenter(handysearch::dictionary &dict) {
  this->m_dictionary = dict;
}


std::vector<std::string> handysearch::segmenter::segment(const std::string &content) const {
  std::vector<std::string> result;
  result.push_back(content);
  return result;
}
