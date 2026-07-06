#include <stdx/test.hpp>

#include <catch2/catch_test_macros.hpp>

struct S {
    friend constexpr auto operator==(S, S) -> bool = default;
};

TEST_CASE("type-erased value", "[test]") {
    [[maybe_unused]] auto v = stdx::te_value{S{}};
}
