#include <handysearch/segmenter.h>
#include <vector>

class handysearch::segmenter::impl {
public:
  explicit impl(handysearch::dictionary &dict): m_dictionary(dict) { };
  ~impl() = default;
  handysearch::dictionary const &m_dictionary;
};


handysearch::segmenter::segmenter(handysearch::dictionary &dict): m_impl(new impl(dict)) { }

handysearch::segmenter::~segmenter() = default;

std::vector<std::string> handysearch::segmenter::segment(std::string const &content) const {
  std::vector<std::string> result;
  result.push_back(content);
  return result;
}
