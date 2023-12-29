/**
 * @file bigint.cpp
 * @author Yiding Li
 * @brief An implementation of arbitrary-length integers.
 * @version 0.1
 * @date 2023-12-28
 */

#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
using std::cout;
using std::domain_error;
using std::int8_t;
using std::ostream;
using std::out_of_range;
using std::reverse;
using std::string;
using std::strong_ordering;
using std::to_string;
using std::vector;

/**
 * @brief Convert a character to an integer.
 *
 * @param a A character
 * @return int8_t 
 * @throw domain_error if the input is out of the range (48, 57)
 */
int8_t char_to_int(const char &a)
{
    if (a < 48 || a > 57)
    {
        throw domain_error("Cannot cast, character out of range");
    }
    else
    {
        return a - 48;
    }
}

/**
 * @brief Convert a integer to a single digit.
 *
 * @param a A integer that can be converted to a single digit.
 * @return double
 * @throw domain_error if the input is out of range (0, 9)
 */
char dig_to_char(const int8_t &a)
{
    if (a > 9)
    {
        string error_msg = "Cannot cast, integer " + to_string(unsigned(a)) + " out of range";
        throw domain_error(error_msg);
    }
    else
    {
        return (char)a + 48;
    }
}

/**
 * @brief Raise 2 to the power of a large base, then return the value. Fails if the result cannot be represented.
 *
 * @param a_pow The power to be raised to
 * @return int64_t
 */
int64_t int64_t_pow_2(int64_t a_pow)
{
    if (a_pow < 1)
    {
        return 1;
    }
    else
    {
        return (2 * int64_t_pow_2(a_pow - 1));
    }
}

/**
 * @brief Sum two strings of digits, then return the result.
 *
 * @param a_s1 the first operand
 * @param a_s2 the second operand
 * @return string
 * @throw domain_error if either input contains non-numeric characters
 */
string sum_strs(const string &a_s1, const string &a_s2)
{
    string longer_str = (a_s1.length() > a_s2.length()) ? a_s1 : a_s2;
    string shorter_str = (a_s1.length() > a_s2.length()) ? a_s2 : a_s1;
    vector<char> result;

    size_t len_dif = longer_str.length() - shorter_str.length();
    bool carry = false;

    for (size_t i = longer_str.length(); i-- > 0;)
    {
        char longer_chr = longer_str[i];
        char shorter_chr = (i >= len_dif) ? shorter_str[i - len_dif] : '0';

        int8_t longer_dig = char_to_int(longer_chr);
        int8_t shorter_dig = char_to_int(shorter_chr);
        int8_t sum_of_digs = (int8_t)(longer_dig + shorter_dig + ((carry) ? 1 : 0)); // there is a warning "conversion from 'int' to 'int8_t' {aka 'signed char'} may change value ".
        // I don't know why.
        if (sum_of_digs >= 10)
        {
            result.push_back(dig_to_char(sum_of_digs - 10));
            carry = true;
        }
        else
        {
            result.push_back(dig_to_char(sum_of_digs));
            carry = false;
        }
    }
    if (carry)
    {
        result.push_back('1');
    }
    reverse(result.begin(), result.end());
    string result_string(result.begin(), result.end());

    return result_string;
}

/**
 * @brief Convert a vector of bools (0s and 1s) to a string of decimal digits.
 *
 * @param a_bools
 * @return double
 * @todo Performance can be improved.
 */
string bin_to_string(vector<bool> a_bools)
{
    string result;
    size_t int64_t_len = (sizeof(int64_t) * 8) - 2;
    int64_t accu = 0;
    bool deposited = false;

    for (size_t i = a_bools.size(); i-- > 0;)
    {
        size_t current_significance = a_bools.size() - i - 1;
        // This piece of code is the main bottleneck of converting from binary to decimal string.
        // cout << "## iteration start!\n" <<
        //     "    at this point, accu = " << accu << ", i = " << i << ", sig = " << current_significance << ",\n" <<
        //     "    and the numerals has" << ((deposited)? " " : " not ") <<"been released.\n";
        if (current_significance <= int64_t_len)
        {
            // // Guards against overflow
            if (a_bools[i] == true)
            {
                accu = accu + int64_t_pow_2((int64_t)current_significance);
            }
        }
        else
        {
            if (!deposited)
            {
                // cout << accu;
                result = to_string(accu);
                // cout << "deposit!\n";
                deposited = true;
            }
            if (a_bools[i] == true)
            {
                // cout << "    significance " << current_significance << " exceeds " << int64_t_len << ". Add the number " <<
                //     int64_t_pow_2(2, (int64_t_len)) << "to the string for " << int64_t_pow_2(2, current_significance - int64_t_len ) <<" times.\n";
                for (size_t ii = 0; ii < unsigned(int64_t_pow_2((long)current_significance - (long)int64_t_len)); ii++)
                {
                    // cout << "   -- add " << to_string(int64_t_pow_2(2, (int64_t_len))) << " to " << result << " yields:";

                    result = sum_strs(result, to_string(int64_t_pow_2((int64_t)int64_t_len)));
                    // cout << result << ".\n";
                }
            }
        }
    }
    if (!deposited)
    {
        result = to_string(accu);
    }
    return result;
}

/**
 * @brief Check of the divisor can divide the divident, if their least significant
 *      digits are matched to the same index.
 *
 * @param a_dvd The dividend
 * @param a_dvs The divisor
 * @param a_dvd_offet The offset from which the divisor divides
 * @return If the divisor can divide the divident starting at the offset
 */
bool can_divide(const vector<bool> &a_dvd,
                const vector<bool> &a_dvs,
                const size_t &a_dvd_offset)
{
    if (a_dvd_offset >= 1 && a_dvd[a_dvd_offset - 1] == 1)
    {

        return true;
    }

    for (size_t i = 0; i < a_dvs.size(); i++)
    {

        if (a_dvd[i + a_dvd_offset] > a_dvs[i])
        {
            return true;
        }
        else if (a_dvd[i + a_dvd_offset] < a_dvs[i])
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Given two vectors of boolean values, check if the first one is bigger than the second.
 * @param a_opr_1 The first argument
 * @param a_opr_2 The second argument
 * @return If the first value is bigger than the second
 */

bool is_the_first_bigger(
    const vector<bool> &a_opr_1,
    const vector<bool> &a_opr_2)
{
    vector<bool> opr_1 = a_opr_1;
    vector<bool> opr_2 = a_opr_2;

    while (!opr_1.empty() && opr_1[opr_1.size() - 1] == 0)
    {
        opr_1.pop_back();
    }
    while (!opr_2.empty() && opr_2[opr_2.size() - 1] == 0)
    {
        opr_2.pop_back();
    }

    if (a_opr_1.size() != a_opr_2.size())
    {
        return a_opr_1.size() > a_opr_2.size();
    }
    else
    {
        for (size_t i = 0; i < a_opr_1.size(); i++)
        {
            if (a_opr_1[i] > a_opr_2[i])
            {
                return true;
            }
            else if (a_opr_1[i] < a_opr_2[i])
            {
                return false;
            }
        }
    }
    return false;
}

/**
 * @brief Subtract two boolean vectors. Subtract the smaller one from the larger one, then return the result.
 *
 * @param a_opt_1 The first operand
 * @param a_opt_2 The second operand
 * @return The result of subtraction
 */
vector<bool> sub_bool_vectors(const vector<bool> &a_opr_1,
                              const vector<bool> &a_opr_2,
                              size_t offset = 0)
{

    bool opr_1_is_bigger = is_the_first_bigger(a_opr_1, a_opr_2);
    vector<bool> opr_longer = (opr_1_is_bigger) ? a_opr_1 : a_opr_2;
    vector<bool> opr_shorter = (opr_1_is_bigger) ? a_opr_2 : a_opr_1;

    size_t len_opt_1 = opr_longer.size();
    size_t len_opt_2 = opr_shorter.size();

    size_t len_dif = len_opt_1 - len_opt_2;

    bool borrow = false;

    if (len_dif < offset)
    {
        throw out_of_range("Offset greater than length");
    }

    for (size_t i = opr_longer.size() - offset; i-- > 0;)
    {
        size_t i_opr_1 = i;
        size_t i_opr_2 = i - len_dif + offset;

        size_t i_opr_2_c = ((i + offset) >= len_dif) ? opr_shorter[i_opr_2] : 0;

        bool set_opr_1;
        if (opr_longer[i_opr_1] == 1 && i_opr_2_c == 1)
        {

            if (borrow)
            {
                set_opr_1 = 1;
                borrow = true;
            }
            else
            {
                set_opr_1 = 0;
                borrow = false;
            }
        }
        else if (opr_longer[i_opr_1] == 1 && i_opr_2_c == 0)
        {
            if (borrow)
            {
                set_opr_1 = 0;
                borrow = false;
            }
            else
            {
                set_opr_1 = 1;
                borrow = false;
            }
        }
        else if (opr_longer[i_opr_1] == 0 && i_opr_2_c == 1)
        {
            if (borrow)
            {
                set_opr_1 = 0;
                borrow = true;
            }
            else
            {
                set_opr_1 = 1;
                borrow = true;
            }
        }
        else
        {
            if (borrow)
            {
                set_opr_1 = 1;
                borrow = true;
            }
            else
            {
                set_opr_1 = 0;
                borrow = false;
            }
        }
        opr_longer[i_opr_1] = set_opr_1;
    }
    return opr_longer;
}

/**
 * @brief Check if a bool vector contains only "zero"s.
 *
 * @param a_vec The vector to be checked
 * @return If the vector contains only "zero"s
 */
bool is_zero_vector(const vector<bool> &a_vec)
{
    for (bool bol : a_vec)
    {
        if (bol == true)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Auxillary function. Divide two values then return the result, as well as the remainder, through its arguments
 *
 * @param a_dvd The dividend
 * @param a_dvs The divisor
 * @param result The vector that accepts the result
 * @param rem The vector that accepts the remainder
 */
void div_bool_vectors_bare(const vector<bool> &a_dvd, const vector<bool> &a_dvs, vector<bool> &result, vector<bool> &rem)
{
    if (!is_the_first_bigger(a_dvd, a_dvs))
    {
        result = vector<bool>();
        rem = a_dvd;
    }
    if (is_zero_vector(a_dvs))
    {
        throw domain_error("Cannot divide by zero");
    }

    vector<bool> dvd_copy = a_dvd;

    size_t len_dvd = a_dvd.size();
    size_t len_dvs = a_dvs.size();

    size_t len_dif = len_dvd - len_dvs;

    for (size_t i = 0; i < len_dif + 1; i++)
    {
        if (can_divide(
                dvd_copy, a_dvs, i))
        {
            vector<bool> tmp = sub_bool_vectors(dvd_copy, a_dvs, len_dvd - (i + len_dvs));
            dvd_copy = tmp;
            result.push_back(1);
        }
        else
        {
            result.push_back(0);
        }
    }
    rem = dvd_copy;
}

/**
 * @brief Divide two vectors of bools.
 *
 * @param a_dvd The dividend
 * @param a_dvs the divisor
 * @return The result of the division, truncated.
 */
vector<bool> div_bool_vectors(const vector<bool> &a_dvd, const vector<bool> &a_dvs)
{
    vector<bool> result;
    vector<bool> rem;
    div_bool_vectors_bare(a_dvd, a_dvs, result, rem);
    return result;
}

/**
 * @brief Shift the given bool vector by 1 to the left
 *
 * @param a_dvd The given bool vector
 */
void lsh_bool_vector(vector<bool> &a_dvd)
{
    a_dvd.push_back(0);
}

/**
 * @brief Add two bool vectors, then return the result.
 * @param a_opr_1 The first operand
 * @param a_opr_2 The second operand
 * @return The result of addition
 */
vector<bool> add_bool_vectors(const vector<bool> &a_opr_1, const vector<bool> &a_opr_2)
{

    vector<bool> result;

    bool opr_1_is_bigger = is_the_first_bigger(a_opr_1, a_opr_2);

    vector<bool> opr_longer = (opr_1_is_bigger) ? a_opr_1 : a_opr_2;
    vector<bool> opr_shorter = (opr_1_is_bigger) ? a_opr_2 : a_opr_1;
    size_t len_dif = opr_longer.size() - opr_shorter.size();
    bool carry = false;

    for (size_t i = opr_longer.size(); i-- > 0;)
    {
        bool opt_longer_bol = opr_longer[i];
        bool opt_shorter_bol = (i >= len_dif) ? opr_shorter[i - len_dif] : 0;
        if (opt_longer_bol == 1 && opt_shorter_bol == 1)
        {
            if (carry)
            {
                result.push_back(1);
            }
            else
            {
                result.push_back(0);
            }
            carry = true;
        }
        else if ((opt_longer_bol == 1 && opt_shorter_bol == 0) || (opt_longer_bol == 0 && opt_shorter_bol == 1))
        {
            if (carry)
            {
                result.push_back(0);
                carry = true;
            }
            else
            {
                result.push_back(1);
                carry = false;
            }
        }
        else
        {
            if (carry)
            {
                result.push_back(1);
            }
            else
            {
                result.push_back(0);
            }
            carry = false;
        }
    }
    if (carry)
    {
        result.push_back(1);
    }
    reverse(result.begin(), result.end());
    return result;
}

/**
 * @brief Multiply two bool vectors, then return the result.
 *
 * @param a_mnd The number to be multiplied
 * @param a_mer The multiplier
 * @return The result of multiplication
 */
vector<bool> mul_bool_vectors(const vector<bool> &a_mnd, const vector<bool> &a_mer)
{

    vector<bool> result = {0};
    vector<bool> addrant = a_mnd;

    for (size_t i = 0; i < a_mer.size(); i++)
    {

        if (a_mer[a_mer.size() - i - 1])
        {
            result = add_bool_vectors(result, addrant);
            ;
        };
        lsh_bool_vector(addrant);
    }
    return result;
}

/**
 * @brief Arbitrary-length integer. Its length is technically limited to the smaller of int64_t and size_t, though this limitation is unlikely to come up.
 *      The largest number it can contain is 2^4294967296.
 */
class Int
{
public:
    Int(const Int &);
    Int(const string &);
    Int(const bool &, const vector<bool> &);

    bool is_positive = true;
    vector<bool> bools;

    Int operator=(const Int &);

    friend ostream &operator<<(const ostream &, const Int &);
    Int operator-() const;
    Int operator+(const Int &) const;
    void operator+=(const Int &);
    Int operator-(const Int &a_that) const;
    void operator-=(const Int &a_that);
    Int operator*(const Int &a_that) const;
    void operator*=(const Int &a_that);
    Int operator/(const Int &a_that) const;
    void operator/=(const Int &a_that);
    bool operator==(const Int &a_that) const;
    bool operator!=(const Int &a_that) const;
    bool operator>(const Int &a_that) const;
    bool operator<(const Int &a_that) const;
    bool operator>=(const Int &a_that) const;
    bool operator<=(const Int &a_that) const;
    string to_str() const;
    string to_str_bools() const;

private:
    void consume_str_to_bools(const string &);
};

/**
 * @brief Return a binary string representaiton of this integer.
 *
 * @return A binary string representation of this integer.
 */
string Int::to_str_bools() const
{
    if (is_zero_vector(this->bools)) return "0";
    string result;
    bool is_1_met = false;
    result += (this->is_positive ? "" : "-");
    for (const bool &bol : this->bools)
    {
        if (bol || is_1_met)
        {
            result += (bol) ? '1' : '0';
            is_1_met = true;
        }
    }

    return result;
}

/**
 * @brief Return a string representation of this integer.
 *
 * @return A string representation of this integer.
 */
string Int::to_str() const
{
    if (is_zero_vector(this->bools)) return "0";
    return (this->is_positive ? "" : "-") + bin_to_string(this->bools);
}

/**
 * @brief Constructor from a string of digits.
 *
 * @param a_in a string to construct from
 * @throw domain_error if the input contains non-digit characters.
 */
Int::Int(const string &a_in)
{
    this->is_positive = a_in[0] != '-';
    string body = a_in.substr(this->is_positive ? 0 : 1);
    body.erase(0, body.find_first_not_of('0'));
    consume_str_to_bools(body);
};

/**
 * @brief Copy constructor.
 *
 * @param a_int another instance of Int
 */
Int::Int(const Int &a_int)
{
    this->is_positive = a_int.is_positive;
    this->bools = a_int.bools;
};

/**
 * @brief Constructor from attributes.
 *
 * @param a_in attributes of Int to construct from
 */
Int::Int(const bool &a_is_positive, const vector<bool> &a_bools)
{
    this->is_positive = a_is_positive;
    this->bools = a_bools;
};

/**
 * @brief Private method. Take a string, analuze it, then load the result into the
 *      bool field of this object.
 *
 * @param a_dvd the string to be read
 * @throw domain_error if the input contains non-digit characters.
 */
void Int::consume_str_to_bools(const string &a_dvd)
{
    if (a_dvd.length() == 0)
    {
        bools.push_back((bool)0);
        return;
    }

    string dvd = a_dvd;
    while (true)
    {
        bool carry = false;
        for (char &chr : dvd)
        {
            if (chr == 0)
                break;
            int8_t digit = char_to_int(chr);
            bool carry_old = carry;
            carry = digit % 2 == 0 ? false : true;
            // Mystery. Why is this happening?
            int8_t divide_me = (int8_t)(digit + (carry_old ? 10 : 0));
            chr = dig_to_char(divide_me / 2);
        }
        bools.push_back((bool)carry);
        if (dvd[0] == '0')
        {
            dvd.erase(0, 1);
            if (dvd.empty())
                break;
        }
    }
    reverse(bools.begin(), bools.end());
}


ostream &operator<<(ostream &a_os, const Int &a_int)
{
    a_os << a_int.to_str();
    return a_os;
}

Int Int::operator=(const Int &a_opr_2)
{
    return Int(a_opr_2);
};

Int Int::operator+(const Int &a_that) const
{
    bool my_bools_is_bigger = is_the_first_bigger(this->bools, a_that.bools);
    vector<bool> result_bools;
    bool result_is_positive;
    if (this->is_positive != a_that.is_positive)
    {
        result_bools = sub_bool_vectors(this->bools, a_that.bools);
        result_is_positive = (my_bools_is_bigger) ? this->is_positive : a_that.is_positive;
    }
    else
    {
        result_bools = add_bool_vectors(this->bools, a_that.bools);
        result_is_positive = this->is_positive;
    }
    return Int(result_is_positive, result_bools);
}

void Int::operator+=(const Int &a_that)
{
    bool my_bools_is_bigger = is_the_first_bigger(this->bools, a_that.bools);
    vector<bool> result_bools;
    bool result_is_positive;
    if (this->is_positive != a_that.is_positive)
    {
        result_bools = sub_bool_vectors(this->bools, a_that.bools);
        result_is_positive = (my_bools_is_bigger) ? this->is_positive : a_that.is_positive;
    }
    else
    {
        result_bools = add_bool_vectors(this->bools, a_that.bools);
        result_is_positive = this->is_positive;
    }
    this->is_positive = result_is_positive;
    this->bools = result_bools;
}

Int Int::operator-(const Int &a_that) const
{
    Int neg_that = a_that;
    neg_that.is_positive = !neg_that.is_positive;
    return (Int(*this) + neg_that);
}

void Int::operator-=(const Int &a_that)
{
    Int result = Int(*this) - a_that;
    this->is_positive = result.is_positive;
    this->bools = result.bools;
}

Int Int::operator*(const Int &a_that) const
{
    bool result_is_positive = (this->is_positive == a_that.is_positive);
    vector<bool> result_bools = mul_bool_vectors(this->bools, a_that.bools);
    return Int(result_is_positive, result_bools);
}

void Int::operator*=(const Int &a_that)
{
    Int result = Int(*this) * a_that;
    this->is_positive = result.is_positive;
    this->bools = result.bools;
}

Int Int::operator/(const Int &a_that) const
{
    bool result_is_positive = (this->is_positive == a_that.is_positive);
    vector<bool> result_bools = div_bool_vectors(this->bools, a_that.bools);
    return Int(result_is_positive, result_bools);
}

void Int::operator/=(const Int &a_that)
{
    Int result = Int(*this) / a_that;
    this->is_positive = result.is_positive;
    this->bools = result.bools;
}

bool Int::operator==(const Int &a_that) const
{
    if (is_zero_vector(this->bools) && is_zero_vector(a_that.bools)) return true;
    return this->bools == a_that.bools && this->is_positive == a_that.is_positive;
}

bool Int::operator!=(const Int &a_that) const
{
    return !(Int(*this) == a_that);
}

bool Int::operator>(const Int &a_that) const
{
    if (is_zero_vector(this->bools) && is_zero_vector(a_that.bools)) return false;
    if (this->is_positive && !a_that.is_positive)
    {

        return true;
    }
    else if (!this->is_positive && a_that.is_positive)
    {
        return false;
    }
    return (!is_the_first_bigger(a_that.bools, this->bools));
}
bool Int::operator<(const Int &a_that) const
{
    return a_that > Int(*this);
}

bool Int::operator>=(const Int &a_that) const
{
    return (Int(*this) > a_that || Int(*this) == a_that);
}
bool Int::operator<=(const Int &a_that) const
{
    return (Int(*this) < a_that || Int(*this) == a_that);
}

Int Int::operator-() const
{
    Int result = Int(*this);
    result.is_positive = !(result.is_positive);
    return result;
};
