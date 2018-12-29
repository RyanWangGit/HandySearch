#include <iostream>
#include <catch.hpp>
#include <handysearch/dictionary.h>

TEST_CASE("Test dictionary functionality", "[HandySearch::Dictionary]") {
  handysearch::dictionary dictionary;
  std::uint64_t test_current = 0, test_total = 0;
  dictionary.load("./sample_dictionary.txt", [&test_current, &test_total](std::uint64_t current, std::uint64_t total) {
    test_current = current;
    test_total = total;
  });
  REQUIRE(test_current == test_total);
  REQUIRE(test_total == dictionary.total_items());
  REQUIRE(dictionary.total_items() != 0);
  REQUIRE(dictionary.has_word("huffman"));
  REQUIRE(!dictionary.has_word("huffman_no"));
  REQUIRE(dictionary.get_frequency("huffman") == 5);
  REQUIRE(dictionary.has_word("土豆"));
  REQUIRE(dictionary.get_frequency("土豆") == 2);
}

