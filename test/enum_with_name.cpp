#include <stdx/ct_format.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace stdx::ct_string_literals;

enum struct value_t {};

template <stdx::ct_string Name> struct e_name_t {
    constexpr static auto name = Name;
    constexpr explicit(false) operator value_t() const { return value; }
    value_t value{};
};

template <stdx::ct_string Name> struct undefined_e_name_t {};

template <stdx::ct_string Name>
constexpr inline auto e_name_v = undefined_e_name_t<Name>{};

template <>
constexpr inline auto e_name_v<"A"> = e_name_t<"A">{.value = value_t{17}};

template <stdx::ct_string S> constexpr auto format_as(e_name_t<S>) { return S; }

TEST_CASE("runtime convert to value_t", "[enum_with_name]") {
    auto e = e_name_v<"A">;
    [[maybe_unused]] value_t v = e;
}

TEST_CASE("compile-time convert to value_t", "[enum_with_name]") {
    auto l = []<value_t V>() {};
    l.template operator()<e_name_v<"A">>();
}

TEST_CASE("pass to template placeholder", "[enum_with_name]") {
    auto l = []<e_name_t V>() -> value_t { return V; };
    STATIC_CHECK(l.template operator()<e_name_v<"A">>() == value_t{17});
}

TEST_CASE("format as string", "[enum_with_name]") {
    STATIC_CHECK(STDX_CT_FORMAT("Hello {}", e_name_v<"A">) ==
                 "Hello A"_fmt_res);
}
