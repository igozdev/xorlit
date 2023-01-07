/*
* MIT License
* 
* Copyright (c) 2023 IGOZ
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

namespace xorlit
{
    constexpr uint32_t time =
        static_cast<uint32_t>(__TIME__[7] - '0') +
        static_cast<uint32_t>(__TIME__[6] - '0') * 10U +
        static_cast<uint32_t>(__TIME__[4] - '0') * 100U +
        static_cast<uint32_t>(__TIME__[1] - '0') * 1000U +
        static_cast<uint32_t>(__TIME__[3] - '0') * 10000U +
        static_cast<uint32_t>(__TIME__[0] - '0') * 20000U;

    template <size_t Index>
    constexpr void XorString(char* d, char key, const char* s)
    {
        d[Index] = s[Index] ^ key;
        XorString<Index - 1>(d, key, s);
    }
    template <>
    constexpr void XorString<0>(char* d, char key, const char* s)
    {
        d[0] = s[0] ^ key;
    }

    template <size_t Size>
    struct String
    {
    public:
        constexpr String(const char (&s)[Size], char key)
            : m_Data(), m_Key(key)
        {
            XorString<Size - 1>(m_Data, m_Key, s);
        }

        const char* Xor()
        {
            for (char& c : m_Data)
                c ^= m_Key;
            return m_Data;
        }

    private:
        const char m_Key;
        char m_Data[Size];
    };

    template <size_t Size>
    constexpr String<Size> CreateString(const char (&s)[Size], char key)
    {
        return String<Size>(s, key);
    }
}

#define XORLITSTR(s) xorlit::CreateString(s, static_cast<char>((xorlit::time + __LINE__ * 100000) % 255)).Xor()
