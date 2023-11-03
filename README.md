# xorlit
> Compile time string literal encryptor for C++.

[![license][badge.license]][license]
[![release][badge.release]][release]
[![godbolt][badge.godbolt]][godbolt]

[badge.license]: https://img.shields.io/badge/license-mit-green.svg
[license]: https://github.com/igozdev/xorlit/blob/main/LICENSE
[badge.release]: https://img.shields.io/github/release/igozdev/xorlit.svg
[release]: https://github.com/igozdev/xorlit/releases/latest
[badge.godbolt]: https://img.shields.io/badge/try_it-on_godbolt-indigo.svg
[godbolt]: https://godbolt.org/z/Ksd36MMo7

* [Usage](#usage)
* [Example](#example)

# Usage
## XORLIT
The `XORLIT` macro can be used to create an encrypted string which evaluates to its original contents (e.g. `XORLIT("foo")` would evalute as the string "foo", but be stored as some other encrypted value). As it wraps `xorlit::make_str`, one can pass either one argument, a string literal, or two arguments, a string literal and a key. In the case that one argument is passed, the key used will default to `xorlit::seed`.
> [!WARNING]
> Exercise caution when using `XORLIT`, `xorlit::make_str(__VA_ARGS__).rexor()`, or `xorlit::string`s in any context where they are not explicitly stored, as some compilers, such as MSVC, may sporadically encrypt strings non-constexpr-ly depending on the string literal's content or the context of its usage. It may be advisable to search binary contents for any unencrypted string literals in this case.

## xorlit::seed
`xorlit::seed` is a `constexpr std::uint_least32_t` which is based on the predefined `__TIME__` macro to generate values. It is the default value used by `xorlit::make_str` and by extension `XORLIT` for the key. Since `xorlit::seed` or `static_cast<char>(xorlit::seed)` may evaluate to 0, which, in such a case, would lead to an unencrypted string, by defining `XORLIT_SEED_STATIC_ASSERT` before including the `xorlit.hpp` header, a static assertion by the compiler will be enabled to check if `xorlit::seed` is equal to 0.

## xorlit::string
`xorlit::string` is the provided type for storing encrypted strings. A `xorlit::string` should always be decorated with the `constexpr` specifier to ensure it is stored in its encrypted form. 
`xorlit::string` defines the following member functions:
* `xorlit::string::rexor()`: xors the data of the string and returns it.
* `xorlit::string::xor_data()`: returns a new `const char*` with a copy of the string's data xored.
* `xorlit::string::data`: returns the data stored in the string. `const` and non-`const` variants are provided.
* `xorlit::string::key`: returns the key stored in the string.
> [!IMPORTANT]
> Results of `xorlit::string::xor_data()` must be managed manually, whether that be via `delete`, smart pointers, etc.

# Example
```c++
#include <string>
#include <iostream>
#include <format>
#define XORLIT_SEED_STATIC_ASSERT // enable compiler check if xorlit::seed is 0
#include <xorlit.hpp>

int main()
{
    std::cout
        << XORLIT("Default, xorlit::seed is passed as the key\n")
        << XORLIT("Or you can use xorlit::seed yourself\n", xorlit::seed + __LINE__) // when passing your own seeds, you should check that they aren't 0
        << XORLIT("Or you can choose your own key\n", 12) << std::endl;

    constexpr auto s = xorlit::make_str("You can manually store a xorlit::string"); // always declare the results of xorlit::make_str constexpr
    const std::string raw(s.data(), decltype(s)::size);
    const char* original_data = s.xor_data(); // results of xorlit::string::xor_data must be manually managed, smart pointers are recommended
    const std::string original(original_data, decltype(s)::size);
    delete[] original_data;
    std::cout << std::format("Raw: {}\nOriginal: {}", raw, original) << std::endl;
}

////////////////// Possible output: //////////////////
// Default, xorlit::seed is passed as the key
// Or you can use xorlit::seed yourself
// Or you can choose your own seed
//
// Raw:    ?%p31>p=1>%1<<)p#$?"5p1p(?"<9$jj#$"9>7P
// Original: You can manually store a xorlit::string
```
