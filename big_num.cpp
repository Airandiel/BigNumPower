#include "big_num.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

BigNum::BigNum() = default;

BigNum::BigNum(std::string str) {
    // Conversion from string to BigNum, separating number into hunks, which
    // size is defined in MAX_DIGITS_IN_HUNK, each hunk is saved as uint64_t,
    // for 32 bit system it should be changed to uint32_t with adjusting
    // MAX_DIGITS_IN_HUNK and MAX_NUM_IN_HUNK accordingly
    int in_hunk_counter = 0;
    for (int i = str.length() - 1; i >= 0; i--) {
        if (in_hunk_counter < MAX_DIGITS_IN_HUNK) {
            in_hunk_counter++;
        } else {
            // pushing number to vector, adding 1 on the front for keeping the
            // zeros
            _hunks.push_back((uint64_t)std::stoull(
                '1' + str.substr(i + 1, in_hunk_counter)));
            in_hunk_counter = 1;
        }
    }
    // if there is some not converted number yet
    if (in_hunk_counter > 0) {
        _hunks.push_back((uint64_t)std::stoull(str.substr(0, in_hunk_counter)) +
                         MAX_NUM_IN_HUNK);
    }
    // if the last hunk is empty, erase it
    if (_hunks.size() > 1 && _hunks[_hunks.size() - 1] == MAX_NUM_IN_HUNK) {
        _hunks.pop_back();
    }
}

BigNum::BigNum(const std::vector<uint64_t> vec) { _hunks = vec; }

BigNum::BigNum(int num) {
    if (num >= 0) {
        if (num < MAX_NUM_IN_HUNK) {
            _hunks.push_back((uint64_t)num + MAX_NUM_IN_HUNK);
        } else {
            _hunks.push_back((uint64_t)(num % MAX_NUM_IN_HUNK) +
                             MAX_NUM_IN_HUNK);
            _hunks.push_back((uint64_t)(num / MAX_NUM_IN_HUNK) +
                             MAX_NUM_IN_HUNK);
        }
    }
}

BigNum::BigNum(uint64_t num) {
    if (num == 0) {
        _hunks.push_back(MAX_NUM_IN_HUNK);
    } else {
        while (num != 0) {
            _hunks.push_back((uint64_t)(num % MAX_NUM_IN_HUNK) +
                             MAX_NUM_IN_HUNK);
            num /= MAX_NUM_IN_HUNK;
        }
    }
}

BigNum::BigNum(const BigNum& num) { _hunks = num.get_hunks(); }

BigNum& BigNum::operator=(const BigNum& other) {
    if (this != &other) {
        // Perform deep copy of relevant data members
        _hunks = other._hunks;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const BigNum& num) {
    os << num.to_string();
    return os;
}

BigNum BigNum::operator*(const BigNum& other_num) const {
    BigNum x(*this);
    BigNum y(other_num);
    return karatsubami_multiply(x, y);
}

BigNum BigNum::operator+(const BigNum& other) const {
    // BigNum result;
    uint64_t carry = 0;
    unsigned int maxSize = std::max(_hunks.size(), other._hunks.size());
    BigNum result;

    for (unsigned int i = 0; i < maxSize; i++) {
        uint64_t sum = carry;
        if (i < _hunks.size()) {
            sum += _hunks[i];
            sum -= MAX_NUM_IN_HUNK;
        }
        if (i < other._hunks.size()) {
            sum += other._hunks[i];
            sum -= MAX_NUM_IN_HUNK;
        }

        carry = sum / MAX_NUM_IN_HUNK;
        sum %= MAX_NUM_IN_HUNK;

        result._hunks.push_back(sum + MAX_NUM_IN_HUNK);
    }

    if (carry > 0) {
        result._hunks.push_back(carry + MAX_NUM_IN_HUNK);
    }

    return result;
}

BigNum BigNum::operator-(const BigNum& other) const {
    // Assuming both BigNum objects are positive and other is less than or equal
    // to the current object, no validation
    BigNum result;
    int borrow = 0;
    unsigned int maxSize = _hunks.size();
    if (maxSize < other.get_length()) {
        maxSize = other.get_length();
    }

    for (unsigned int i = 0; i < maxSize; i++) {
        int64_t diff = borrow;
        if (i < _hunks.size()) {
            diff += _hunks[i];
            diff -= MAX_NUM_IN_HUNK;
        }
        if (i < other._hunks.size()) {
            diff -= other._hunks[i];
            diff += MAX_NUM_IN_HUNK;
        }
        if (diff < 0) {
            diff += MAX_NUM_IN_HUNK;
            borrow = -1;
        } else {
            borrow = 0;
        }
        result._hunks.push_back((uint64_t)diff + MAX_NUM_IN_HUNK);
    }

    while (result._hunks.size() > 1 &&
           result._hunks.back() == MAX_NUM_IN_HUNK) {
        result._hunks.pop_back();
    }

    return result;
}

BigNum BigNum::operator^(const BigNum& other_num) const {
    BigNum x(*this);
    int y = (int)(other_num);
    return pow_by_squaring(x, y);
}

BigNum BigNum::karatsubami_multiply(BigNum x, BigNum y) const {
    /*  Calculates two numbers multiplication.
        Computes equation X*Y = (a*10^(n/2) + b)(c*10^(n/2) + d) =
       a*c*10^n + (a*d+b*c)*10^(n/2) + b*d
    */
    unsigned int len_x = x.get_length();
    unsigned int len_y = y.get_length();
    if (len_x == 1 && len_y == 1) {
        return BigNum((uint64_t)x * (uint64_t)y);
    }
    if (len_x < len_y) {
        x.add_zero_padding(len_y - len_x, false);
    } else if (len_y < len_x) {
        y.add_zero_padding(len_x - len_y, false);
    }
    len_x = x.get_length();
    int j = (int)len_x / 2;
    int subtr_zero_pad = len_x - j;
    if ((len_x % 2) != 0) {
        // if odd length of x decrease the padding
        subtr_zero_pad--;
    }
    int ac_zero_pad = subtr_zero_pad * 2;
    BigNum a(x.get_sub_vector(j, x.get_length()));  // left part of number x
    BigNum b(x.get_sub_vector(0, j));               // right part of number x
    BigNum c(y.get_sub_vector(j, y.get_length()));  // left part of number y
    BigNum d(y.get_sub_vector(0, j));               // right part of number y

    // recursively calculate
    BigNum ac = karatsubami_multiply(a, c);
    BigNum bd = karatsubami_multiply(b, d);
    BigNum k = karatsubami_multiply(a + b, c + d);
    BigNum subtraction = k - ac - bd;
    // padding to replace multiplication by 10
    ac.add_zero_padding(ac_zero_pad, true);
    subtraction.add_zero_padding(subtr_zero_pad, true);
    BigNum result = ac + subtraction + bd;
    result.trim_zeroes();
    return result;
}

BigNum BigNum::pow_by_squaring(BigNum x, int n) const {
    if (n == 0) {
        return BigNum(1);
    }
    BigNum temp(1);
    while (n > 1) {
        if (n % 2 == 0) {
            x = x * x;
            n /= 2;
        } else {
            temp = temp * x;
            x = x * x;
            n = (n - 1) / 2;
        }
    }
    return x * temp;
}

unsigned int BigNum::get_length() const { return _hunks.size(); }

std::vector<uint64_t> BigNum::get_hunks() const {
    return std::vector<uint64_t>(_hunks);
}

std::string BigNum::to_string() const {
    std::string number = "";
    std::string hunk_str = "";
    for (unsigned int i = 0; i < _hunks.size(); i++) {
        hunk_str = std::to_string(_hunks[_hunks.size() - i - 1]);
        number += hunk_str.substr(1, hunk_str.length() - 1);
    }

    number.erase(0, std::min(number.find_first_not_of('0'), number.size() - 1));
    if (number.length() == 0) {
        number = "0";
    }
    return number;
}

// Add paddding made of zeroes on one side of the number, if right is true,
// the zeroes are put on the right, otherwise on the left
void BigNum::add_zero_padding(int padding_size, bool right) {
    if (padding_size > 0) {
        if (right) {
            _hunks.insert(_hunks.begin(), padding_size, MAX_NUM_IN_HUNK);

        } else {
            _hunks.resize(_hunks.size() + padding_size, MAX_NUM_IN_HUNK);
        }
    }
}

void BigNum::trim_zeroes() {
    while (_hunks[_hunks.size() - 1] == MAX_NUM_IN_HUNK) {
        _hunks.pop_back();
    }
}

std::vector<uint64_t> BigNum::get_sub_vector(int begin, int end) const {
    return std::vector<uint64_t>(_hunks.begin() + begin, _hunks.begin() + end);
}
