#pragma once

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
    size_t cnt = 0;
    while (offset + size - cnt - 1 < message.size() && cnt < size && message[offset + size - cnt - 1] == '\0') {
        cnt++;
    }
    std::string result(message.cbegin() + offset, message.cbegin() + offset + size - cnt);
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
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (value == 0)
        return "0";
    std::vector<unsigned char> str;
    while (value > 0) {
        str.push_back(digits[value % 36]);
        value /= 36;
    }
    std::string result(str.rbegin(), str.rend());
    return result;
}
