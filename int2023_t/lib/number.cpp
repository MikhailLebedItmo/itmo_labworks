#include "number.h"

#include <cstring>
#include <iostream>

int2023_t::int2023_t() {
    memset(data, 0, kDataSize);
}

int2023_t from_int(int32_t i) {
    auto result = int2023_t();
    bool is_negative = i < 0;
    i = abs(i);
    size_t digit_cnt = int2023_t::kDataSize;
    while(i != 0) {
        result.data[--digit_cnt] = i % int2023_t::kSystemBase;
        i /= int2023_t::kSystemBase;
    }

    if (is_negative) {
        result = -result;
    }

    return result;
}

int2023_t from_string(const char* buff) {
    auto result = int2023_t();
    bool is_negative = false;
    if (buff[0] == '-') {
        is_negative = true;
    }
    size_t buff_len = std::strlen(buff);
    for (size_t i = static_cast<size_t>(is_negative); i < buff_len; ++i) {
        result *= static_cast<uint8_t>(10);
        result += static_cast<uint8_t>(buff[i] - '0'); 
    }
    if (is_negative) {

        return -result;
    }

    return result;
}

int2023_t operator+(int2023_t lhs, const int2023_t& rhs) {
    lhs += rhs;
    return lhs;
}

int2023_t& operator+=(int2023_t& lhs, const int2023_t& rhs) {
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs.data[i];
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase); 
        carry = sum / int2023_t::kSystemBase;
    }

    return lhs;
}

int2023_t operator+(int2023_t lhs, uint8_t rhs) {
    lhs += rhs;
    return lhs;
}

int2023_t& operator+=(int2023_t& lhs, uint8_t rhs) {
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs;
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase);
        carry = sum / int2023_t::kSystemBase;
        rhs = 0;
        if (carry == 0) {

            return lhs;
        }
    }

    return lhs;
}

int2023_t& operator++(int2023_t& rhs) {
    return rhs += 1;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    return lhs + (-rhs);
}

int2023_t operator-(int2023_t rhs) {
    for (size_t i = 0; i < int2023_t::kDataSize; ++i) {
        rhs.data[i] = ~rhs.data[i];
    }
    ++rhs;

    return rhs;
}

int2023_t shift_digits_to_left(int2023_t value, size_t shift) {
    // return value * 256^shift
    for (size_t t = 0; t < int2023_t::kDataSize - shift; ++t) {
            value.data[t] = value.data[t + shift];
    }
    for (size_t t = int2023_t::kDataSize - shift; t < int2023_t::kDataSize; ++t) {
        value.data[t] = static_cast<uint8_t>(0);
    }

    return value;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    auto result = int2023_t();
    for(int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        size_t shift = int2023_t::kDataSize - 1 - i;
        result += shift_digits_to_left(lhs * rhs.data[i], shift);
    }

    return result;
}

int2023_t operator*(int2023_t lhs, uint8_t rhs) {
    lhs *= rhs;
    return lhs;
}

int2023_t& operator*=(int2023_t& lhs, uint8_t rhs) {
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - 1; i >= 0; --i) {
        uint32_t sum = carry + static_cast<uint32_t>(rhs) * lhs.data[i];
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase);
        carry = sum / int2023_t::kSystemBase;
    }

    return lhs;
}

void sum_with_shift(int2023_t& lhs, const int2023_t& rhs, size_t shift) {
    // lhs = lhs + rhs * 256^shift
    uint32_t carry = 0;
    for (int i = int2023_t::kDataSize - static_cast<int>(shift) - 1; i >= 0; --i) {
        uint32_t sum = carry + lhs.data[i] + rhs.data[i + static_cast<int>(shift)];
        lhs.data[i] = static_cast<uint8_t>(sum % int2023_t::kSystemBase); 
        carry = sum / int2023_t::kSystemBase;
    }
}

size_t len(const int2023_t& value) {
    return int2023_t::kDataSize - get_ind_of_first_digit(value);
}

bool is_negative(const int2023_t& value) {
    int char_size = 8;
    return static_cast<bool>((value.data[0] >> (char_size - 1)) & 1);
}

int2023_t abs(const int2023_t& value) {
    if (is_negative(value)) {

        return -value;
    }

    return value;
}

size_t get_ind_of_first_digit(const int2023_t& value) {
    size_t i = 0;
    while (i < int2023_t::kDataSize && value.data[i] == 0) {
        ++i;
    }
    if (i == int2023_t::kDataSize){
        --i;
    }

    return i;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    auto result = int2023_t();
    size_t result_len = 0;
    auto lhs_abs = abs(lhs);
    auto rhs_abs = abs(rhs);
    size_t rhs_abs_len = len(rhs_abs);
    bool skip_first_iteration = lhs_abs.data[0] != 0 &&   //  to avoid overflow on first iteration
                                rhs_abs.data[get_ind_of_first_digit(rhs_abs)] >= static_cast<uint8_t>(128);  
    for (size_t i = len(lhs_abs) - static_cast<size_t>(skip_first_iteration); i > 0; --i) {
        if (rhs_abs_len > i) {
            continue;
        }
        for (uint8_t digit = 1; digit <= static_cast<uint8_t>(int2023_t::kSystemBase - 1); ++digit) {
            sum_with_shift(lhs_abs, -rhs_abs, i - rhs_abs_len);
            if (is_negative(lhs_abs)) {
                sum_with_shift(lhs_abs, rhs_abs, i - rhs_abs_len);
                result.data[result_len++] = digit - 1;
                break;
            }
        }
    }

    // reformat data from [d0, d1, ... , di, 0, ... , 0] to [0, ... , 0, d0, d1, ... , di]
    for (size_t i = static_cast<int>(result_len); i > 0; --i) {
        result.data[int2023_t::kDataSize - result_len + i - 1] = result.data[i - 1];
        result.data[i - 1] = 0;
    }
    if (is_negative(lhs) != is_negative(rhs)) {

        return -result;
    }

    return result;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (size_t i = 0; i < int2023_t::kDataSize; ++i) {
        if (lhs.data[i] != rhs.data[i]) {

            return false;
        }
    }

    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

char get_hex_digit(uint8_t num) {
    if (num >= 0 && num <= 9) {

        return num + '0';
    }

    return num - 10 + 'A';
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    char hex_str[int2023_t::kDataSize * 2 + 1];
    size_t digit_ind = 0;
    uint8_t second_digit_mask = 0b00001111;
    for (size_t i = get_ind_of_first_digit(value); i < int2023_t::kDataSize; ++i) {
        hex_str[digit_ind++] = get_hex_digit((value.data[i] >> 4));
        hex_str[digit_ind++] = get_hex_digit(value.data[i] & second_digit_mask);
    }
    hex_str[digit_ind] = '\0';
    if (hex_str[0] == '0') {
        stream << hex_str + 1;
    } else {
        stream << hex_str;
    }
    
    return stream;
}
