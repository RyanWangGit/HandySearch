#include <iostream>
#include <catch.hpp>
#include <handysearch/handysearch.h>

TEST_CASE("Test initialization of core", "[HandySearch]") {
  handysearch::search_core core;
  REQUIRE(true);
}

TEST_CASE("Test load dictionary", "[HandySearch]") {
  handysearch::search_core core;
  std::uint64_t test_current = 0;
  std::uint64_t test_total = 0;
  core.load_dictionary("", [&test_current, &test_total](std::uint64_t current, std::uint64_t total) {
    REQUIRE(current > test_current);
    test_current = current;
    test_total = total;
  });
  REQUIRE(test_current != 0);
  REQUIRE(test_total != 0);
}
