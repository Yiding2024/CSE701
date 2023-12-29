# BigInt Project

The file bigint.hpp contains a class Int which is able to represent arbitrary-length integers. Technically, the library should fail for integers larger than 2 ^ (2 ^ 32), though my computer is not able to store a number that large.

The bottleneck is conversion from the internal (binary) representation to a decimal string. The algirithm runs slowly for binary representations larger than 70 bits long, as it must resort to adding numbers for exponentially many times. The file `demo.cpp` contains examples of the program, such as ..
8000000000000000000000000000000000000000000000000000000 + -450000000045454500000000000000000 = 7999999999999999999999549999999954545500000000000000000
8000000000000000000000000000000000000000000000000000000 - -450000000045454500000000000000000 = 8000000000000000000000450000000045454500000000000000000
8000000000000000000000000000000000000000000000000000000 * -450000000045454500000000000000000 = -3600000000363636000000000000000000000000000000000000000000000000000000000000000000000000
8000000000000000000000000000000000000000000000000000000 / -450000000045454500000000000000000 = -17777777775982044444625 (truncated)

The class offers the following constructors:

`    Int::Int(const string &a_in)`
`    Int::Int(const Int &a_int)`
`    Int::Int(const bool &a_is_positive, const vector<bool> &a_bools)`
`    void Int::consume_str_to_bools(const string &a_dvd)`
