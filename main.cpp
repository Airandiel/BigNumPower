#include <algorithm>  // for reversing string
#include <iostream>
#include <string>
#include <vector>

#include "big_num.h"

using namespace std;

bool input_in_range(string input) {
    int num = std::stoull(input);
    if (num < 0 || num > 99999) {
        cout << "Input not in range." << endl;
        return false;
    } else {
        return true;
    }
}

bool input_is_number(string input) {
    // checks also if the number is in range
    if (input.size()) {
        for (unsigned int i = 0; i < input.size(); i++) {
            int char_value = (int)input[i] - '0';
            if (char_value < 0 || char_value > 9) {
                cout << "Input is not a positive number." << endl;
                return false;
            }
        }
        return input_in_range(input);
    } else {
        return false;
    }
}

int main() {
    string numA;
    string numB;

    while (!input_is_number(numA)) {
        cout << "Enter number A from 0 to 99999:" << endl;
        cin >> numA;
    }

    numB = numA;
    reverse(numB.begin(), numB.end());
    BigNum A(numA);
    BigNum B(numB);

    cout << "A ^ B = " << A << " ^ " << B << " = " << endl;

    BigNum result = A ^ B;
    cout << result << endl;
}