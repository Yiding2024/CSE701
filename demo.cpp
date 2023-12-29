/**
 * @file demo.cpp
 * @author Yiding Li
 * @brief An presentation of how an arbitrary length integer (bigint.hpp) works.
 * @version 0.1
 * @date 2023-12-28
 */
#include "bigint.hpp"

int main()
{
    // The program can store and print very large integers.
    Int int_a("990000009999990099999999");
    cout << "The number 990000009999990099999999 is restored to " << int_a << ".\n";

    // The program converts numeric strings losslessly.
    cout << "The stored number " 
            << ((int_a.to_str() == "990000009999990099999999") ? "matches" : "does not match")
            << " the original value.\n";

    // Because of the algorithm to convert the internal boolean representation to strings,
    //      which makes use of additions beyond int32_t, converting large numbers to decimal
    //      strings is slow. I was able to find faster implementations, but could not
    //      use them for this project, due to constraints.

    // Displaying the boolean representation takes much less time.
    // Storing and manipulation of binary numbers are reasonably fast.
    Int int_b("9999999999999999999999999999999999999999999999999999999999999999999999999");
    cout << "The number 9999999999999999999999999999999999999999999999999999999999999999999999999 is stored as "
             << int_b.to_str_bools() << ".\n";

    cout << "The stored number " 
            << ((int_b.to_str_bools() == "101101010001110100010011101011101010010010100100100010001101110101101011101010111010101101100011100110001011110110111110010000010100001101000000101100111100001000011001001111111111111111111111111111111111111111111111111111111111111111111111111") ? "matches" : "does not match") << " the original value (in binary).\n";

    // Test arithmetic operators
    Int int_c("99999999");
    Int int_d("-900009");

    cout << "Arithmetic operations in binary, then convert to decimal representations.\n";
    cout << "Addition: " << int_c << "+" << int_d << "=" << (int_c + int_d) << "\n";
    cout << "Subtraction: " << int_c << "-" << int_d << "=" << (int_c - int_d) << "\n";
    cout << "Multiplication: " << int_c << "*" << int_d << "=" << (int_c * int_d) << "\n";
    cout << "Division: " << int_c << "/" << int_d << "=" << (int_c / int_d) << "\n";

    // Representing numbers in binary allows me to use larger numbers.
    cout << "Arithmetic operations in binary.\n";
    // 
    Int int_c_large("8000000000000000000000000000000000000000000000000000000");
    Int int_d_large("-450000000045454500000000000000000");
    Int c_plus_d = int_c_large + int_d_large;    

    // int_c_large + int_d_large = 7999999999999999999999549999999954545500000000000000000
    cout << "Addition: " << int_c_large.to_str_bools() << "+" << int_d_large.to_str_bools() << "=" << (c_plus_d).to_str_bools() << "\n";
    // int_c_large - int_d_large = 8000000000000000000000450000000045454500000000000000000
    cout << "Subtraction: " << int_c_large.to_str_bools() << "-" << int_d_large.to_str_bools() << "=" << (int_c_large - int_d_large).to_str_bools() << "\n";
    // int_c_large * int_d_large = -3600000000363636000000000000000000000000000000000000000000000000000000000000000000000000
    cout << "Multiplication: " << int_c_large.to_str_bools() << "*" << int_d_large.to_str_bools() << "=" << (int_c_large * int_d_large).to_str_bools() << "\n";
    // int_c_large / int_d_large = -17777777775982044444625 (truncated)
    cout << "Division: " << int_c_large.to_str_bools() << "/" << int_d_large.to_str_bools() << "=" << (int_c_large / int_d_large).to_str_bools() << "\n";

    Int int_e("90");
    Int int_f("45");
    Int int_g("90");
    Int int_h("0");
    Int int_i("0");
    Int int_j("-0");
    cout << "Compare: " << int_e << ((int_e < int_f)? "<" : ">=") << int_f << "\n";
    cout << "Compare: " << int_h << ((int_h < int_g)? "<" : ">=") << int_g << "\n";
    cout << "Compare: " << int_i << ((int_i == int_j)? "==" : "!=") << int_j << "\n";
}