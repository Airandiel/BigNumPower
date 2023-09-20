#ifndef BIGNUM_H
#define BIGNUM_H

#include <iostream>
#include <vector>

#define MAX_DIGITS_IN_HUNK 9
#define MAX_NUM_IN_HUNK 1000000000

class BigNum {
    // The number is saved as vector of uint64_t where each hunk stores 9 digits
    // - limited by max number of uint64_t and multiplication of this type of
    // data, the hunks are stored in increasing order of signifficance - at
    // index 0, the hunk is least signifficant, at the last index, the hunk has
    // highest signifficance
   private:
    std::vector<uint64_t> _hunks;

   public:
    BigNum();
    BigNum(std::string str);
    BigNum(const std::vector<uint64_t> vec);
    BigNum(int num);
    BigNum(uint64_t num);
    BigNum(const BigNum& num);

    friend std::ostream& operator<<(std::ostream& os, const BigNum& num);
    BigNum& operator=(const BigNum& other);
    BigNum operator+(const BigNum& other) const;
    BigNum operator-(const BigNum& other) const;
    BigNum operator*(const BigNum& other_num) const;
    BigNum operator^(const BigNum& other_num) const;

    explicit operator int() const {
        // use with care, only converts number properly if it doesn't exceeds
        // the one hunk
        if (_hunks.size() > 1) {
            std::cout << "Warning: Improper int conversion, the number is "
                         "bigger than conversion allows to.";
        }
        return (int)(_hunks[_hunks.size() - 1] - MAX_NUM_IN_HUNK);
    }
    explicit operator uint64_t() const {
        if (_hunks.size() > 1) {
            std::cout << "Warning: Improper uint64_t conversion, the number is "
                         "bigger than conversion allows to.";
        }
        return (_hunks[_hunks.size() - 1] - MAX_NUM_IN_HUNK);
    }

    unsigned int get_length() const;
    std::vector<uint64_t> get_hunks() const;
    std::string to_string() const;

   private:
    BigNum pow_by_squaring(const BigNum x, int n) const;
    BigNum karatsubami_multiply(BigNum x, BigNum y) const;
    void trim_zeroes();
    void add_zero_padding(int padding_size, bool left);
    std::vector<uint64_t> get_sub_vector(int begin, int end) const;
};

#endif  // BIGNUM_H
