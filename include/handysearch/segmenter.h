#pragma once
#include "handysearch/dictionary.h"

namespace handysearch{

class segmenter {
public:
  segmenter(handysearch::dictionary &dict);
  std::vector<std::string> segment(std::string &content) const;

private:
  handysearch::dictionary m_dictionary;
};

}



