#include <iostream>
#include <catch2/catch.hpp>
#include <handysearch/core.h>

TEST_CASE("Test initialization of core", "[HandySearch]") {
  handysearch::search_core core;
  REQUIRE(true);
}
