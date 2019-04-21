#include <iostream>
#include <catch/catch.hpp>
#include <handysearch/handysearch.h>

TEST_CASE("Test initialization of core", "[HandySearch]") {
  handysearch::search_core core;
  REQUIRE(true);
}
