#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

namespace mp {
    class bignum {
    public:
        inline bignum();
        inline bignum(uint32_t);
        inline bignum(const bignum&);
        inline explicit bignum(const std::string&);

        inline bignum& operator=(const bignum&);
        inline bignum& operator=(const uint32_t&);

        inline explicit operator uint32_t() const;

        inline explicit operator bool() const;

        inline std::string to_string() const;

        inline bignum& operator+=(bignum const&);
        inline bignum& operator*=(bignum const&);

        inline bignum& operator+=(uint32_t);
        inline bignum& operator*=(uint32_t);

        bignum operator-(const bignum) = delete;
        bignum operator-(const int32_t) = delete;

    private:
        std::vector<uint32_t> value;

        inline uint32_t mod(uint32_t) const;
        inline mp::bignum& div(uint32_t);

        inline bignum& zero_killer();
    };

    class polynomial {
    private:
        std::vector<uint32_t> coef;

    public:
        explicit inline polynomial(std::string);

        inline uint32_t& at(uint32_t);
        inline uint32_t  at(uint32_t) const;

        inline int    operator()(int) const;
        inline double operator()(double) const;
        inline bignum operator()(const bignum&) const;
    };
}

// mp::bignum
inline std::istringstream& operator>>(std::istringstream &in, mp::bignum& smth) {
    std::string s;
    in >> s;
    smth = mp::bignum(s);
    return in;
}

inline std::ostringstream& operator<<(std::ostringstream &out, mp::bignum const& smth) {
    out << smth.to_string();
    return out;
}

mp::bignum& mp::bignum::operator+=(uint32_t x) {
    uint64_t rem = x;
    for (std::size_t i = 0; i < value.size(); i++) {
        uint64_t cur = value[i] + rem;
        value[i] = static_cast<uint32_t>(cur);
        rem = (cur >> 32u);
    }
    if (rem != 0) {
        value.push_back(rem);
    }
    return *this;
}

mp::bignum& mp::bignum::operator*=(uint32_t x) {
    uint64_t rem = 0;
    uint64_t copy_x = x;
    for (std::size_t i = 0; i < value.size(); i++) {
        uint64_t cur = static_cast<uint64_t>(value[i]) * copy_x + rem;
        value[i] = static_cast<uint32_t>(cur);
        rem = (cur >> 32u);
    }
    if (rem != 0) {
        value.push_back(rem);
    }
    return *this;
}

inline mp::bignum operator+(mp::bignum a, uint32_t b) {
    a += b;
    return a;
}

inline mp::bignum operator+(uint32_t b, mp::bignum a) {
    a += b;
    return a;
}

inline mp::bignum operator*(mp::bignum a, uint32_t b) {
    a *= b;
    return a;
}

inline mp::bignum operator*(uint32_t b, mp::bignum a) {
    a *= b;
    return a;
}

mp::bignum::bignum()           : value(1, 0) {}
mp::bignum::bignum(uint32_t x) : value(1, x) {}
mp::bignum::bignum(const bignum &copy) = default;
mp::bignum::bignum(const std::string& s) {
    uint32_t cur;
    for (std::size_t i = 0; i < s.size(); i++) {
        if (i == 0 && s[i] == '+') {
            continue;
        } else if (i == 0 && (s[i] - '0' < 0 || s[i] - '0' > 9)) {
            throw std::runtime_error("the first character of the number must be \"+\" or 0-9");
        }

        *this = *this * static_cast<uint32_t>(10);
        cur = s[i] - '0';
        *this += cur;
    }
}

mp::bignum& mp::bignum::operator=(const bignum& copy) = default;
mp::bignum& mp::bignum::operator=(const uint32_t& copy) {
    *this = mp::bignum(copy);
    return *this;
}

mp::bignum::operator uint32_t() const { return value[0]; }

std::string mp::bignum::to_string() const{
    std::string ans;
    mp::bignum copy = *this;
    while(copy.value.size() != 0) {
        uint32_t x = copy.mod(10);
        ans += std::to_string(x);
        copy = copy.div(10);
    }
    if (ans.empty()) {
        ans = '0';
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

mp::bignum::operator bool() const { return value.back() != 0; }

mp::bignum &mp::bignum::operator+=(mp::bignum const& right) {
    std::size_t size = std::max(value.size(), right.value.size()) + 1;
    for (std::size_t i = 0; i < size - value.size(); ++i) {
        value.push_back(0);
    }

    uint64_t rem = 0;
    std::size_t i;
    for (i = 0; i < right.value.size(); ++i) {
        uint64_t cur = static_cast<uint64_t>(value[i]) + right.value[i] + rem;
        value[i] = static_cast<uint32_t>(cur);
        rem = (cur >> 32u);
    }

    while (rem != 0) {
        if (i == value.size()) {
            value.push_back(rem);
            break;
        }

        uint64_t cur = static_cast<uint64_t>(value[i]) + rem;
        value[i] = static_cast<uint32_t>(cur);
        rem = (cur >> 32u);
        ++i;
    }

    return zero_killer();
}

mp::bignum& mp::bignum::operator*=(mp::bignum const& right) {
    if (!right) {
        *this = 0;
        return *this;
    }

    std::vector<uint32_t> res(value.size() + right.value.size());
    for (std::size_t i = 0; i < value.size(); ++i) {
        uint64_t rem = 0;
        for (std::size_t j = 0; j < right.value.size(); ++j) {
            uint64_t cur = static_cast<uint64_t>(value[i])
                           * static_cast<uint64_t>(right.value[j])
                           + res[i + j] + rem;
            res[i + j] = static_cast<uint32_t>(cur);
            rem = (cur >> 32u);
        }

        if (rem != 0) {
            res[i + right.value.size()] += static_cast<uint32_t>(rem);
        }
    }

    value = res;
    return zero_killer();
}

uint32_t mp::bignum::mod(uint32_t x) const {
    uint64_t multiply = static_cast<uint64_t>(1) << 32u;
    uint64_t cur_deg = multiply % x;
    uint64_t ans = value[0] % x;
    for (std::size_t i = 1; i < value.size(); i++) {
        uint64_t cur = (value[i] % x) * static_cast<uint32_t>(cur_deg) ;
        cur %= x;
        ans += static_cast<uint32_t>(cur);
        ans %= x;
        cur_deg *= multiply % x;
        cur_deg %= x;
    }
    return ans;
}

mp::bignum& mp::bignum::div(uint32_t x) {
    uint64_t rem = 0;
    std::vector<uint32_t> copy = value;
    for (std::size_t i = value.size(); i >= 1; --i) {
        uint64_t cur_value = value[i - 1];
        uint32_t cur = static_cast<uint32_t>(((cur_value + (rem << 32u)) / x));
        copy[i - 1] = cur;
        rem = (cur_value + (rem << 32u)) % x;
    }
    value = copy;
    return zero_killer();
}

mp::bignum &mp::bignum::zero_killer() {
    std::size_t right_size = value.size();
    while (right_size > 0 && value[right_size - 1] == 0) {
        --right_size;
    }
    value.resize(right_size);
    return *this;
}

inline mp::bignum operator+(mp::bignum const& a, mp::bignum const& b) {
    mp::bignum a_ = a;
    a_ += b;
    return a_;
}

inline mp::bignum operator*(mp::bignum const& a, mp::bignum const& b) {
    mp::bignum a_ = a;
    a_ *= b;
    return a_;
}


// mp::polynomial
mp::polynomial::polynomial(std::string s){
    uint32_t cur_coef = 0, cur_deg = 0;

    bool grad = false;
    for (std::size_t i = 0; i < s.size(); i++) {
        if (s[i] == '-' || s[i] == '+') {
            grad = false;
            if (this->coef.empty()) {
                this->coef.resize(cur_deg + 1, 0);
            }
            this->coef[cur_deg] = cur_coef;
            cur_coef = 0;
            cur_deg = 0;
        }

        if (s[i] - '0' >= 0 && s[i] - '0' <= 9) {
            if (grad) {
                cur_deg *= 10;
                cur_deg += s[i] - '0';
            } else {
                cur_coef *= 10;
                cur_coef += s[i] - '0';
            }
            if (i == s.size() - 1) {
                if (this->coef.empty()) {
                    this->coef.resize(cur_deg + 1, 0);
                }
                this->coef[cur_deg] = cur_coef;
            }
        }

        if (s[i] == '^') {
            grad = true;
        }
    }
}

uint32_t& mp::polynomial::at(uint32_t i) {
    if (i > this->coef.size() - 1) {
        this->coef.resize(i + 1);
    }
    return this->coef[i];
}

uint32_t mp::polynomial::at(uint32_t i) const {
    if (i > this->coef.size() - 1) {
        throw std::runtime_error("the index should be smaller than the size");
    }
    return this->coef[i];
}


int mp::polynomial::operator()(int x) const {
    int32_t cur = this->coef[this->coef.size() - 1];
    for (std::size_t i = this->coef.size() - 1; i >= 1; --i) {
        cur = this->coef[i - 1] + cur * x;
    }
    return cur;
}

double mp::polynomial::operator()(double x) const {
    double cur = this->coef[this->coef.size() - 1];
    for (std::size_t i = this->coef.size() - 1; i >= 1; --i) {
        cur = this->coef[i - 1] + cur * x;
    }
    return cur;
}

mp::bignum mp::polynomial::operator()(const mp::bignum& x) const {
    mp::bignum cur = this->coef[this->coef.size() - 1];
    for (std::size_t i = this->coef.size() - 1; i >= 1; --i) {
        cur = this->coef[i - 1] + cur * x;
    }
    return cur;
}
