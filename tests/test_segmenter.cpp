#include <iostream>
#include <catch.hpp>
#include <handysearch/segmenter.h>

TEST_CASE("Test segmenter functionality", "[HandySearch::Segmenter]") {
  handysearch::dictionary dictionary;
  std::uint64_t test_current = 0, test_total = 0;
  dictionary.load("./sample_dictionary.txt");

  handysearch::segmenter segmenter(dictionary);
  std::string test_string = "This is a test, 这是一个测试";
  std::vector<std::string> result = segmenter.segment(test_string);
  REQUIRE(result.size() > 0);
}


