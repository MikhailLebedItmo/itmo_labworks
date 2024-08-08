#pragma once
#include <cinttypes>
#include <iostream>


struct int2023_t {
    static const size_t kDataSize = 253;
    static const uint32_t kSystemBase = 256;
    uint8_t data[kDataSize]; 
    int2023_t();
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(int2023_t lhs, const int2023_t& rhs);

int2023_t operator+(int2023_t lhs, uint8_t rhs);

int2023_t& operator+=(int2023_t& lhs, const int2023_t& rhs);

int2023_t& operator+=(int2023_t& lhs, uint8_t rhs);

int2023_t& operator++(int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(int2023_t lhs, uint8_t rhs);

int2023_t& operator*=(int2023_t& lhs, uint8_t rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);

size_t len(const int2023_t& value);

size_t get_ind_of_first_digit(const int2023_t& value);
