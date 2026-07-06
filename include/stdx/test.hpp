#pragma once

#include <stdx/concepts.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

namespace stdx {
inline namespace v1 {

namespace detail {
struct type_id_t {};
template <typename T> constexpr auto type_id_v = type_id_t{};
template <typename T> constexpr auto type_id = &type_id_v<T>;
} // namespace detail

struct te_value {
    using type_id_t = detail::type_id_t const *;

    template <typename T>
    // NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
    explicit(true) te_value(T &&t)
        : p{new derived<std::remove_cvref_t<T>>(std::forward<T>(t))} {}

    template <typename T> [[nodiscard]] auto get() const -> std::optional<T> {
        if (auto ptr = p->get(detail::type_id<T>); ptr) {
            return *static_cast<T const *>(ptr);
        }
        return {};
    }

    [[nodiscard]] auto operator==(te_value const &rhs) const -> bool {
        return p->compare(rhs);
    }
    [[nodiscard]] auto hash() const -> std::size_t { return p->hash(); }

    // NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
    struct base {
        virtual ~base() = default;
        [[nodiscard]] virtual auto get(type_id_t) const -> void const * = 0;
        [[nodiscard]] virtual auto compare(te_value const &) const -> bool = 0;
        [[nodiscard]] virtual auto hash() const -> std::size_t = 0;
    };

    template <typename T> struct derived : base {
        explicit(true) constexpr derived(T t) : val{std::move(t)} {}

        [[nodiscard]] auto get(type_id_t ti) const -> void const * override {
            if (ti != detail::type_id<T>) {
                return nullptr;
            }
            return std::addressof(val);
        }

        [[nodiscard]] auto compare(te_value const &v) const -> bool override {
            static_assert(equality_comparable<T>,
                          "The type contained in te_value must model "
                          "equality_comparable");
            auto o = v.get<T>();
            return o.has_value() and *o == val;
        }

        [[nodiscard]] auto hash() const -> std::size_t override {
            static_assert(
                requires { std::hash<T>{}(val); },
                "A std::hash specialization must be defined for the type "
                "contained in te_value");
            return std::hash<T>{}(val);
        }

        T val;
    };

    std::unique_ptr<base> p;
};

template <typename T> auto get(te_value const &v) -> std::optional<T> {
    return v.template get<T>();
}

} // namespace v1
} // namespace stdx

template <> struct std::hash<stdx::te_value> {
    [[nodiscard]] auto operator()(stdx::te_value const &v) const
        -> std::size_t {
        return v.hash();
    }
};
