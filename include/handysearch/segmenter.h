#pragma once
#include "handysearch/dictionary.h"

namespace handysearch{

class segmenter {
public:
  explicit segmenter(handysearch::dictionary &dict);
  std::vector<std::string> segment(std::string const &content) const;

private:
  handysearch::dictionary m_dictionary;
};

}



