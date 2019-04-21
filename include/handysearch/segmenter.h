#pragma once
#include "handysearch/dictionary.h"
#include <memory>

namespace handysearch{

class segmenter {
public:
  explicit segmenter(handysearch::dictionary &dict);
  ~segmenter();
  std::vector<std::string> segment(std::string const &content) const;

private:
  class impl;
  std::unique_ptr<impl> m_impl;
};

}



