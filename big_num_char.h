#ifndef BIGNUMCHAR_H
#define BIGNUMCHAR_H

#include <iostream>
#include <vector>

class BigNum {
   private:
    std::vector<uint8_t> digits;

   public:
    BigNum();
    BigNum(std::string str);
    BigNum(const std::vector<uint8_t> vec);
    BigNum(int num);

    friend std::ostream& operator<<(std::ostream& os, const BigNum& num);

    BigNum operator+(const BigNum& other) const;
    BigNum operator-(const BigNum& other) const;
    BigNum operator*(BigNum& other_num);

    explicit operator int() { return vec_to_int(); }

    BigNum pow_by_squaring(BigNum x, int n);

    uint get_length() const;
    std::vector<uint8_t> get_digits() const;
    void add_zero_padding(int padding_size, bool left);
    std::vector<uint8_t> get_sub_vector(int begin, int end);

   private:
    BigNum karatsubami_multiply(BigNum x, BigNum y);
    int vec_to_int();
};

#endif  // BIGNUMCHAR_H
