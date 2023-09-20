#include <cassert>
#include <iostream>
#include <string>

#include "big_num.h"

using namespace std;

void test_addition() {
    cout << "Testing addition. ";
    BigNum A(111);
    BigNum B(100);
    BigNum result = A + B;
    assert(result.to_string() == "211" &&
           "Simple addition(111+100) test failed!");

    A = BigNum("111111111111111111111");
    B = BigNum("999999999999999999999");
    result = A + B;
    assert(result.to_string() == "1111111111111111111110" &&
           "Long addition, carry test failed!");

    A = BigNum("0");
    B = BigNum("0");
    result = A + B;
    assert(result.to_string() == "0" && "Zero addition test failed!");

    A = BigNum("999999999");
    B = BigNum("1");
    result = A + B;
    assert(result.to_string() == "1000000000" && "Hunk expansion test failed!");
    cout << "\tPASSED" << endl;
}

void test_subtaction() {
    cout << "Testing subtraction. ";
    BigNum A(111);
    BigNum B(100);
    BigNum result = A - B;
    assert(result.to_string() == "11" &&
           "Simple subtraction(111-100) test failed!");

    A = BigNum("111111111111111111111");
    B = BigNum("99999999999999999999");
    result = A - B;
    assert(result.to_string() == "11111111111111111112" &&
           "Long subtraction, carry test failed!");

    A = BigNum("0");
    B = BigNum("0");
    result = A - B;
    assert(result.to_string() == "0" && "Zero subtraction test failed!");

    A = BigNum("1000000000");
    B = BigNum("1");
    result = A - B;
    assert(result.to_string() == "999999999" && "Hunk shorting test failed!");
    cout << "\tPASSED" << endl;
}

void test_multiplication() {
    cout << "Testing multiplication. ";
    BigNum A(111);
    BigNum B(100);
    BigNum result = A * B;
    assert(result.to_string() == "11100" &&
           "Simple multiplication(111*100) test failed!");

    A = BigNum("222222222222222222222");
    B = BigNum("999999999999999999999");
    result = A * B;
    assert(result.to_string() == "222222222222222222221777777777777777777778" &&
           "Long multiplication, carry test failed!");

    A = BigNum("0");
    B = BigNum("0");
    result = A * B;
    assert(result.to_string() == "0" && "Zero multiplication test failed!");
    cout << "\tPASSED" << endl;
}

void test_power() {
    cout << "Testing power. ";
    BigNum A(2);
    BigNum B(3);
    BigNum result = A ^ B;
    assert(result.to_string() == "8" && "Simple power(2^3) test failed!");

    A = BigNum(0);
    B = BigNum(0);
    result = A ^ B;
    assert(result.to_string() == "1" && "Zero power, test failed!");

    A = BigNum("1234567890");
    B = BigNum(0);
    result = A ^ B;
    assert(result.to_string() == "1" && "Zero power, test failed!");

    A = BigNum(123);
    B = BigNum(321);
    result = A ^ B;
    assert(result.to_string() ==
               "72367033806371673149109894141163778628811792657571658"
               "90601055839039587036379840174409528068615550773640492"
               "16570702849617218289605929779095426370988976972231026"
               "22628566787654091327825453991595140205701412961364188"
               "73240893619789055369971583695156999980043195776921700"
               "67433210262575179327641646623194879149625333027413682"
               "07211189494615326552790667720411285474162636765168907"
               "21192413497337430449601963537666585855994173570392483"
               "64677569172479954695834874677915245821537445221075978"
               "65277798136080074161485280424274076931083994487111719"
               "56224970254036285571291113226596623575435535351670333"
               "90430015061185207603595777378694720186179421205908731"
               "70710805078696371738906375721785723" &&
           "Long power test failed!");
    cout << "\tPASSED" << endl;
}

void test_constructors() {
    cout << "Testing constructors. ";
    int a = 123456789;
    BigNum A(a);
    assert(A.to_string() == "123456789" && "Int constructor test failed!");

    uint64_t b = 1234567890123;
    A = BigNum(b);
    assert(A.to_string() == "1234567890123" &&
           "Uint64_t constructor test failed!");

    string c = "12345678901234567890";
    A = BigNum(c);
    assert(A.to_string() == "12345678901234567890" &&
           "String constructor test failed!");

    std::vector<uint64_t> vec{1000020000, 1000000123};
    A = BigNum(vec);
    assert(A.to_string() == "123000020000" &&
           "Vector constructor test failed!");
    cout << "\tPASSED" << endl;
}

void test_casting() {
    cout << "Testing castings. ";
    BigNum A("123456789");
    assert((int)A == 123456789 && "Int cast test failed!");

    A = BigNum("123456789");
    assert((uint64_t)A == (uint64_t)123456789 && "Uint64_t cast test failed!");

    A = BigNum(123456789);
    assert(A.to_string() == "123456789" && "String conversion test failed!");
    cout << "\tPASSED" << endl;
}

int main() {
    test_constructors();
    test_casting();
    test_addition();
    test_subtaction();
    test_multiplication();
    test_power();
    return 0;
}