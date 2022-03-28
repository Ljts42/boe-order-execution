#pragma once

#include "codec.h"

#include <cmath>

/*
 * Fields
 *  Account : Text(16)
 *  Capacity : Alpha(1)
 *  ClOrdID : Text(20)
 *  MaxFloor : Binary(4)
 *  OrderQty : Binary(4)
 *  OrdType : Alphanum(1)
 *  Price : BinaryPrice(8)
 *  Side : Alphanum(1)
 *  Symbol : Alphanum(8)
 *  TimeInForce : Alphanum(1)
 */
inline unsigned char * encode_text(unsigned char * start, const std::string & str, const size_t field_size)
{
    return encode(start, str, field_size);
}

inline unsigned char * encode_char(unsigned char * start, const char ch)
{
    return encode(start, static_cast<uint8_t>(ch));
}

inline unsigned char * encode_binary4(unsigned char * start, const uint32_t value)
{
    return encode(start, value);
}

inline unsigned char * encode_price(unsigned char * start, const double value)
{
    const double order = 10000;
    const double epsilon = 1e-5;
    // beware: no precision loss check
    return encode(start, static_cast<int64_t>(value * order + std::copysign(epsilon, value)));
}

inline constexpr size_t char_size = 1;
inline constexpr size_t binary4_size = 4;
inline constexpr size_t price_size = 8;

#define FIELD(name, protocol_type, ctype)                                                \
    inline unsigned char * encode_field_##name(unsigned char * start, const ctype value) \
    {                                                                                    \
        return encode_##protocol_type(start, value);                                     \
    }

#define VAR_FIELD(name, size)                                                                  \
    inline unsigned char * encode_field_##name(unsigned char * start, const std::string & str) \
    {                                                                                          \
        return encode_text(start, str, size);                                                  \
    }

#include "fields.inl"

#define FIELD(name, protocol_type, _) inline constexpr size_t name##_field_size = protocol_type##_size;
#define VAR_FIELD(name, size) inline constexpr size_t name##_field_size = size;

#include "fields.inl"

inline void set_opt_field_bit(unsigned char * bitfield_start, unsigned bitfield_num, unsigned bit)
{
    *(bitfield_start + bitfield_num - 1) |= bit;
}

/*
 * decode
 */

inline std::string decode_text(const std::vector<unsigned char> & message, const size_t offset, const size_t size)
{
    return decode(message, offset, size);
}

inline std::string decode_base36(const std::vector<unsigned char> & message, const size_t offset, const size_t size)
{
    return decode(decode(message.cbegin() + offset + size - 1, size));
}

inline double decode_price(const std::vector<unsigned char> & message, const size_t offset, const size_t size)
{
    return static_cast<double>(decode(message.cbegin() + offset + size - 1, size)) / 1e4;
}

inline double decode_double(const std::vector<unsigned char> & message, const size_t offset, const size_t size)
{
    return decode(message.cbegin() + offset + size - 1, size);
}

inline unsigned char decode_char(const std::vector<unsigned char> & message, const size_t offset)
{
    return message[offset];
}

/*
 * Fields Execution
 *  ClOrdID : Text(20)
 *  ExecID : Binary(8)  Base36
 *  LastShares : Binary(4)
 *  LastPx : BinaryPrice(8)
 *  LeavesQty : Binary(4)
 *  BaseLiquidityIndicator : Alphanum(1)
 *  Symbol : Alphanum(8)
 *  LastMkt : Alphanum(4)
 *  FeeCode : Alphanum(2)
 */

#define FIELD(name, protocol_type, offset, size)                                            \
    inline double decode_field_execution_##name(const std::vector<unsigned char> & message) \
    {                                                                                       \
        return decode_##protocol_type(message, offset, size);                               \
    }

#define VAR_FIELD(name, protocol_type, offset, size)                                             \
    inline std::string decode_field_execution_##name(const std::vector<unsigned char> & message) \
    {                                                                                            \
        return decode_##protocol_type(message, offset, size);                                    \
    }

#define CHAR_FIELD(name, offset)                                                                   \
    inline unsigned char decode_field_execution_##name(const std::vector<unsigned char> & message) \
    {                                                                                              \
        return decode_char(message, offset);                                                       \
    }

#include "order_execution.inl"

/*
 * Fields Restatement
 *  ClOrdID : Text(20)
 *  RestatementReason : Alphanum(1)
 *  LeavesQty : Binary(4)
 *  SecondaryOrderID : Binary(8)  Base36
 */

#define FIELD(name, protocol_type, offset, size)                                              \
    inline double decode_field_restatement_##name(const std::vector<unsigned char> & message) \
    {                                                                                         \
        return decode_##protocol_type(message, offset, size);                                 \
    }

#define VAR_FIELD(name, protocol_type, offset, size)                                               \
    inline std::string decode_field_restatement_##name(const std::vector<unsigned char> & message) \
    {                                                                                              \
        return decode_##protocol_type(message, offset, size);                                      \
    }

#define CHAR_FIELD(name, offset)                                                                     \
    inline unsigned char decode_field_restatement_##name(const std::vector<unsigned char> & message) \
    {                                                                                                \
        return decode_char(message, offset);                                                         \
    }

#include "order_restatement.inl"
