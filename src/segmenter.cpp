#include <handysearch/segmenter.h>
#include <vector>

class handysearch::segmenter::impl {
public:
  explicit impl(handysearch::dictionary &dict): m_dictionary(dict) { };
  ~impl() = default;
  handysearch::dictionary const &m_dictionary;
};


typedef struct _chunk {
  std::string_view first_word, second_word, third_word;
} chunk;


handysearch::segmenter::segmenter(handysearch::dictionary &dict): m_impl(new impl(dict)) { }

handysearch::segmenter::~segmenter() = default;

std::vector<std::string> handysearch::segmenter::segment(std::string const &content) const {
  std::vector<std::string> result;
  result.push_back(content);
  return result;
}
