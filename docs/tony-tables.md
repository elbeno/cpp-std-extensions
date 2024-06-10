Variadic `for_each`
---
<details>
When you have two (or more) collections to iterate over, instead of using
cpp11-range to enumerate the indices, you can directly use a variadic version of
`for_each`.

<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
#include &lt;cpp11-range/range.hpp&gt;
   
using util::lang::indices;
   
std::array<T, N> a{};
std::array<U, N> b{};
   
for (auto i : indices(a)) {
   f(a[i], b[i]);
}
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/algorithm.hpp&gt;
   
std::array<T, N> a{};
std::array<U, N> b{};
   
stdx::for_each(a.begin(), a.end(),
   [] (auto& elem_a, auto& elem_b) {
      f(elem_a, elem_b);
   },
   b.begin());
</pre>
</td>
</tr>
</table>

Also available: `for_each_n`.
</details>

Bit masks
---
<details>
Creating bit masks.
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
auto mask = static_cast&lt;std::uint32_t&gt;(
   static_cast&lt;std::uint32_t&gt;(1u << N) - 1u);
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/bit.hpp&gt;
   
auto mask = stdx::bit_mask&lt;std::uint32_t, N&gt;();
</pre>
</td>
</tr>
</table>

But what if N is equal to the type bit width?

Or at runtime...
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
auto mask = static_cast&lt;std::uint32_t&gt;(
   static_cast&lt;std::uint32_t&gt;(1u << n) - 1u);
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/bit.hpp&gt;
   
auto mask = stdx::bit_mask&lt;std::uint32_t&gt;(n);
</pre>
</td>
</tr>
</table>

Or a bitmask for a bit range...
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
auto mask1 = static_cast&lt;std::uint32_t&gt;(
   static_cast&lt;std::uint32_t&gt;(1u << Msb) - 1u);
auto mask2 = static_cast&lt;std::uint32_t&gt;(
   static_cast&lt;std::uint32_t&gt;(1u << Lsb) - 1u);
auto mask = static_cast&lt;std::uint32_t&gt;(
   mask1 & ~mask2);
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/bit.hpp&gt;
   
auto mask = stdx::bit_mask&lt;std::uint32_t, Msb, Lsb&gt;();
</pre>
</td>
</tr>
</table>

</details>

Bit packing
---
<details>
Packing bytes into words.
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
auto packed = static_cast&lt;std::uint16_t&gt;(
   static_cast&lt;std::uint16_t&gt;(hi_byte << 8)
   | static_cast&lt;std::uint16_t&gt;(lo_byte));
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/bit.hpp&gt;
   
auto packed = stdx::bit_pack&lt;std::uint16_t&gt;(hi_byte, lo_byte);
</pre>
</td>
</tr>
</table>

- 2x `std::uint16_t` -> `std::uint32_t`
- 4x `std::uint16_t` -> `std::uint64_t`
- 2x `std::uint32_t` -> `std::uint64_t`
- 4x `std::uint16_t` -> `std::uint64_t`
- 8x `std::uint8_t` -> `std::uint64_t`

</details>

Optional with a tombstone value
---
<details>
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
#include &lt;optional&gt;
   
enum struct E { A, B, C };
auto o = std::optional&lt;E&gt;{E::A};
// sizeof(o) == 8
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/optional.hpp&gt;
   
enum struct E { A, B, C, Invalid };
auto o = stdx::optional&lt;E, stdx::tombstone_value&lt;E::Invalid&gt;&gt;{E::A};
// sizeof(o) == 4
</pre>
</td>
</tr>
</table>

Same interface as std::optional (including monadic operations).

</details>

Compile-time maps
---
<details>
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
constexpr auto some_config = hana::make_tuple(
   hana::make_pair(key1{}, value1{}),
   hana::make_pair(key2{}, value2{}),
   ...
   hana::make_pair(keyN{}, valueN{}));
   
auto pair = hana::find_if(some_config, ...);
auto value = hana::maybe(default_value, ...);
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/utility.hpp&gt;
   
using some_config = stdx::type_map&lt;
   stdx::tt_pair&lt;key1, value1&gt;,
   stdx::tt_pair&lt;key2, value2&gt;,
   ...
   stdx::tt_pair&lt;keyN, valueN&gt;&gt;;
   
using value_t = stdx::lookup&lt;some_config, K&gt;;
</pre>
</td>
</tr>
</table>

- type-to-type map
- type-to-value map
- value-to-value map
- value-to-type map
</details>

More expressive arguments
---
<details>
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
// what do the args mean?
call_a_function(1, 2, true);
   
// what is element 1?
auto x = get&lt;1&gt;(my_tuple);
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/udls.hpp&gt;
   
call_a_function("arg1"_1, 
                "arg2"_2,
                "arg3"_true);
   
auto x = get&lt;"item"_1&gt;(my_tuple);
</pre>
</td>
</tr>
</table>
</details>

Enumerating tuples
---
<details>
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
auto t = /* a tuple */;
   
constexpr auto sz = std::tuple_size_v&lt;decltype(t)&gt;;
   
auto idxRng = hana::to&lt;hana::tuple_tag&gt;(
   hana::make_range(hana::int_c&lt;0&gt;, hana::int_c&lt;sz&gt;);
   
hana::for_each(t, ...);
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/type_traits.hpp&gt;
   
using L = /* a type list */;
   
stdx::template_for_each&lt;L&gt;(
   [&] &lt;typename T&gt; () { ... });
</pre>
</td>
</tr>
</table>

Where "a type list" is a tuple, or just a `stdx::type_list`, or something else.

If you really need state, use `stdx::enumerate` on a `stdx::tuple`.
</details>

Is a value `one_of` a set?
---
<details>
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
#include &lt;util/anyOf.hpp&gt;
   
if (anyOf(value, 1, 2, 3)) {
   ...
}
   
if (not anyOf(value, 1, 2, 3)) {
   ...
}
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/tuple.hpp&gt;
   
if (value == stdx::one_of{1, 2, 3}) {
   ...
}
   
if (value != stdx::one_of{1, 2, 3}) {
   ...
}
</pre>
</td>
</tr>
</table>
</details>

Bitsets
---
<details>
<table>
<tr>
<th>Before stdx</th>
<th>With stdx</th>
</tr>

<tr>
<td valign="top">
<pre lang="cpp">
/* manual bitset calculations,
   bit shifting, casting, etc
   because std::bitset isn't constexpr
   until C++23... */
</pre>
</td>

<td valign="top">
<pre lang="cpp">
#include &lt;stdx/bitset.hpp&gt;
   
constexpr auto b = stdx::bitset<8>{stdx::place_bits,
                                   2, 4, 6};
// 0b0101'0100u
</pre>
</td>
</tr>
</table>

90% the same interface as `std::bitset`. Extras:

- efficient iteration
- `to<T>()`
- bitwise operators: `&` `|` `~`
- `operator-` (set difference)

</details>
