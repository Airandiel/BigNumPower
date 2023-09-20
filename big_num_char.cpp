#include "big_num.h"

#include <iostream>
#include <vector>

#include "tools.cpp"

BigNum::BigNum() { digits.push_back(0); }

BigNum::BigNum(std::string str) {
    std::cout << "num constructor:" << std::endl;
    for (int i = str.length() - 1; i >= 0; i--) {
        digits.push_back((uint8_t)str[i] - (uint8_t)'0');
        // std::cout << (int)digits[i];
    }
    // std::cout << std::endl;
}

BigNum::BigNum(const std::vector<uint8_t> vec) {
    digits.assign(vec.begin(), vec.end());
}

BigNum::BigNum(int num) {
    if (num == 0) {
        digits.push_back(0);
    } else {
        while (num != 0) {
            digits.push_back((uint8_t)(num % 10));
            num /= 10;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const BigNum& num) {
    bool zero_trim_flag = true;
    for (int i = num.digits.size() - 1; i >= 0; i--) {
        if (zero_trim_flag && num.digits[i] != 0) {
            zero_trim_flag = false;
            os << (int)num.digits[i];
        } else if (!zero_trim_flag || num.digits[i] != 0 || i == 0) {
            os << (int)num.digits[i];
        }
    }
    return os;
}

BigNum BigNum::operator*(BigNum& other_num) {
    return karatsubami_multiply(*this, other_num);
}

BigNum BigNum::operator+(const BigNum& other) const {
    BigNum result;
    uint8_t carry = 0;
    uint maxSize = std::max(digits.size(), other.digits.size());

    for (uint i = 0; i < maxSize || carry != 0; i++) {
        uint8_t sum = carry;
        if (i < digits.size()) {
            sum += digits[i];
        }
        if (i < other.digits.size()) {
            sum += other.digits[i];
        }
        carry = sum / 10;
        sum %= 10;
        if (i == 0) {
            result.digits[0] = sum;
        } else {
            result.digits.push_back(sum);
        }
    }

    return result;
}

BigNum BigNum::operator-(const BigNum& other) const {
    // Assuming both BigNum objects are positive and other is less than or equal
    // to the current object
    BigNum result;
    int borrow = 0;
    uint maxSize = digits.size();
    if (maxSize < other.get_length()) {
        maxSize = other.get_length();
    }

    for (uint i = 0; i < maxSize; i++) {
        int diff = borrow;
        if (i < digits.size()) {
            diff += digits[i];
        }
        if (i < other.digits.size()) {
            diff -= other.digits[i];
        }
        if (diff < 0) {
            diff += 10;
            borrow = -1;
        } else {
            borrow = 0;
        }
        if (i == 0) {
            result.digits[0] = diff;
        } else {
            result.digits.push_back((uint8_t)diff);
        }
    }

    while (result.digits.size() > 1 && result.digits.back() == 0) {
        result.digits.pop_back();
    }

    return result;
}

BigNum BigNum::karatsubami_multiply(BigNum x, BigNum y) {
    int len_x = x.get_length();
    int len_y = y.get_length();
    if (len_x < 5 && len_y < 5) {
        return BigNum((int)x * (int)y);
    }

    if (len_x < len_y) {
        x.add_zero_padding(len_y - len_x, false);
    } else if (len_y < len_x) {
        y.add_zero_padding(len_x - len_y, false);
    }
    len_x = x.get_length();
    int j = (int)len_x / 2;
    int BZeroPadding = len_x - j;
    if ((len_x % 2) != 0) {
        // std::cout << "ODD " << j << std::endl;
        BZeroPadding--;
    }
    int AZeroPadding = BZeroPadding * 2;
    BigNum b(x.get_sub_vector(0, j));               // x[:j]
    BigNum a(x.get_sub_vector(j, x.get_length()));  // x[j:]
    BigNum d(y.get_sub_vector(0, j));               // y[:j]
    BigNum c(y.get_sub_vector(j, y.get_length()));  // y[j:]
    // std::cout << "a " << a << std::endl;
    // std::cout << "b " << b << std::endl;
    // std::cout << "c " << c << std::endl;
    // std::cout << "d " << d << std::endl;

    // recursively calculate
    BigNum ac = karatsubami_multiply(a, c);
    BigNum bd = karatsubami_multiply(b, d);
    BigNum k = karatsubami_multiply(a + b, c + d);
    // std::cout << "ac " << ac << std::endl;
    // std::cout << "bd " << bd << std::endl;
    // std::cout << "k " << k << std::endl;
    BigNum subtraction = k - ac;
    subtraction = subtraction - bd;
    // std::cout << "subtraction " << subtraction << std::endl;
    ac.add_zero_padding(AZeroPadding, true);
    // std::cout << "A " << ac << std::endl;
    subtraction.add_zero_padding(BZeroPadding, true);
    // std::cout << "B " << subtraction << std::endl;
    return ac + subtraction + bd;
}

BigNum BigNum::pow_by_squaring(BigNum x, int n) {
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

uint BigNum::get_length() const { return digits.size(); }

// Add paddding made of zeroes on one side of the number, if right is true, the
// zeroes are put on the right, otherwise on the left
void BigNum::add_zero_padding(int padding_size, bool right) {
    if (padding_size > 0) {
        if (right) {
            digits.insert(digits.begin(), padding_size, 0);

        } else {
            digits.resize(digits.size() + padding_size, 0);
        }
    }
}

std::vector<uint8_t> BigNum::get_sub_vector(int begin, int end) {
    return std::vector<uint8_t>(digits.begin() + begin, digits.begin() + end);
}

int BigNum::vec_to_int() {
    int num = 0;
    int exponent = 1;
    for (uint i = 0; i < digits.size(); i++) {
        num += digits[i] * exponent;
        exponent *= 10;
    }
    // std::cout << "num: " << num << std::endl;
    return num;
}
