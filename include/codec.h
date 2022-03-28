#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

/*
 * Encode
 */

inline unsigned char * encode(unsigned char * start, const uint8_t value)
{
    *start = value;
    return start + 1;
}

inline unsigned char * encode(unsigned char * start, const uint16_t value)
{
    *start++ = static_cast<unsigned char>(value & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 8) & 0xFF);
    return start;
}

inline unsigned char * encode(unsigned char * start, const uint32_t value)
{
    *start++ = static_cast<unsigned char>(value & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 8) & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 16) & 0xFF);
    *start++ = static_cast<unsigned char>((value >> 24) & 0xFF);
    return start;
}

inline unsigned char * encode(unsigned char * start, int64_t value)
{
    for (int i = 0; i < 8; ++i) {
        *start++ = static_cast<unsigned char>(value & 0xFF);
        value >>= 8;
    }
    return start;
}

inline unsigned char * encode(unsigned char * start, const std::string & str, const size_t field_size)
{
    size_t i = 0;
    while (i < str.size() && i < field_size) {
        *start++ = str[i];
        ++i;
    }
    while (i < field_size) {
        *start++ = '\0';
        ++i;
    }
    return start;
}

/*
 * Decode
 */

inline std::string decode(const std::vector<unsigned char> & message, const size_t offset, const size_t size)
{
    auto last = std::find(message.begin() + offset, message.begin() + offset + size, '\0');
    std::string result(message.begin() + offset, last);
    return result;
}

inline int64_t decode(std::vector<unsigned char>::const_iterator start, const size_t size)
{
    int64_t result = 0;
    for (size_t i = 0; i < size; ++i) {
        result = (result << 8) + static_cast<int64_t>(*start);
        start--;
    }
    return result;
}

inline std::string decode(int64_t value)
{
    if (value == 0)
        return "0";
    std::vector<unsigned char> str;
    while (value > 0) {
        if (value % 36 < 10) {
            str.push_back('0' + value % 36);
        }
        else {
            str.push_back('7' + (value % 36));
        }
        value /= 36;
    }
    std::string result(str.rbegin(), str.rend());
    return result;
}
