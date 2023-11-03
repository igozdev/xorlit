/*
* MIT License
*
* Copyright (c) 2023 igozdev
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

#include <cstdint>
#include <cstddef>

namespace xorlit
{
	constexpr std::uint_least32_t seed =
		static_cast<std::uint_least32_t>(__TIME__[0] - '0') |
		static_cast<std::uint_least32_t>(__TIME__[1] - '0') << 4 |
		static_cast<std::uint_least32_t>(__TIME__[3] - '0') << 8 |
		static_cast<std::uint_least32_t>(__TIME__[4] - '0') << 12 |
		static_cast<std::uint_least32_t>(__TIME__[6] - '0') << 16 |
		static_cast<std::uint_least32_t>(__TIME__[7] - '0') << 20;
#if defined(XORLIT_SEED_STATIC_ASSERT)
	#if defined(__cpp_static_assert) && __cpp_static_assert >= 200410L
	static_assert(static_cast<char>(seed) != 0, "xorlit::seed acts as zero! Default usage of xorlit::make_str will result in an unchanged string!");
	#endif
#endif

	template <std::size_t I>
#if defined(_MSC_VER)
	__forceinline
#else
	__attribute__((always_inline))
#endif
	constexpr void xor_str(char* d, char key, const char* s)
	{
		d[I] = s[I] ^ key;
		xor_str<I - 1>(d, key, s);
	}
	template <>
#if defined(_MSC_VER)
	__forceinline
#else
	__attribute__((always_inline))
#endif
	constexpr void xor_str<0>(char* d, char key, const char* s)
	{
		d[0] = s[0] ^ key;
	}

	template <std::size_t Size>
	struct string
	{
	public:
#if defined(_MSC_VER)
		__forceinline
#else
		__attribute__((always_inline))
#endif
		constexpr string(const char(&s)[Size], char key)
			: m_data(), m_key(key)
		{
			xor_str<Size - 1>(m_data, m_key, s);
		}

#if defined(_MSC_VER)
		__forceinline
#else
		__attribute__((always_inline))
#endif
		const char* rexor()
		{
			for (char& c : m_data) {
				c ^= m_key;
			}
			return m_data;
		}
#if defined(__cpp_attributes) && __cpp_attributes >= 200809L
		[[nodiscard]]
#endif
#if defined(_MSC_VER)
		__forceinline
#else
		__attribute__((always_inline))
#endif
		const char* xor_data() const
		{
			char* data = new char[Size];
			for (std::size_t i = 0; i < Size; i++) {
				data[i] = m_data[i] ^ m_key;
			}
			return data;
		}
		
#if defined(__cpp_attributes) && __cpp_attributes >= 200809L
		[[nodiscard]]
#endif
#if defined(_MSC_VER)
		__forceinline
#else
		__attribute__((always_inline))
#endif
		char* data()
#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L
			noexcept
#endif
		{
			return m_data;
		}
#if defined(__cpp_attributes) && __cpp_attributes >= 200809L
		[[nodiscard]]
#endif
#if defined(_MSC_VER)
		__forceinline
#else
		__attribute__((always_inline))
#endif
		const char* data() const
#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L
			noexcept
#endif
		{
			return m_data;
		}

#if defined(__cpp_attributes) && __cpp_attributes >= 200809L
		[[nodiscard]]
#endif
#if defined(_MSC_VER)
		__forceinline
#else
		__attribute__((always_inline))
#endif
		char key() const
#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L
			noexcept
#endif
		{
			return m_key;
		}

		constexpr static std::size_t size = Size;

	private:
		const char m_key;
		char m_data[Size];
	};

	template <std::size_t Size>
#if defined(__cpp_attributes) && __cpp_attributes >= 200809L
	[[nodiscard]]
#endif
#if defined(_MSC_VER)
	__forceinline
#else
	__attribute__((always_inline))
#endif
	constexpr string<Size> make_str(const char(&s)[Size], char key = static_cast<char>(xorlit::seed))
	{
		return string<Size>(s, key);
	}
}

#define XORLIT(...) ::xorlit::make_str(__VA_ARGS__).rexor()
