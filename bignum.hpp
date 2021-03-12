#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

namespace mp {
    class bignum {
    private:

        // div, mod для to_string
        inline uint32_t mod(uint32_t x) const {
            uint64_t multiply = static_cast<uint64_t>(1) << 32u;
            uint64_t cur_deg = multiply % x;
            uint64_t ans = this->value[0] % x;
            for (std::size_t i = 1; i < this->value.size(); i++) {
                uint64_t cur = (this->value[i] % x) * static_cast<uint32_t>(cur_deg) ;
                cur %= x;
                ans += static_cast<uint32_t>(cur) ;
                ans %= x;
                cur_deg *= multiply % x;
                cur_deg %= x;
            }
            return ans;
        }

        inline mp::bignum& div(uint32_t x) {
            uint64_t rem = 0;
            std::vector<uint32_t> copy = value;
            for (std::size_t i = this->value.size() - 1; i >= 0; i--) {
                uint64_t cur_value = this->value[i];
                uint32_t cur = static_cast<uint32_t>(((cur_value + (rem << 32u)) / x));
                copy[i] = cur;
                rem = (cur_value + (rem << 32u)) % x;
                if (i == 0) {
                    break;
                }
            }
            value = copy;
            size_t nw_size = this->value.size();
            while (nw_size > 0 && this->value[nw_size - 1] == 0) {
                nw_size--;
            }
            this->value.resize(nw_size);
            return *this;
        }

        //запрет оператора -
        bignum operator- (bignum);
        bignum operator- (int32_t);
    public:
        std::vector<uint32_t> value;
        //конструкторы
        bignum(); // конструктор по умолчанию
        explicit bignum(uint32_t); // конструктор от целого числа
        explicit bignum(const std::string&); //конструктор от строки

        //копирование
        bignum(const bignum &copy);
        bignum& operator= (const bignum& copy);
        bignum& operator= (const uint32_t& copy);

        //преобразование к целому числу
        explicit operator uint32_t() const;

        //использование в условных выражениях
        explicit operator bool() const;

        //получение строкового представления
        std::string to_string() const;

        //операции +, *, +=, *= bignum
        bignum& operator+= (bignum const& right);
        bignum& operator*= (bignum const& right);

        //операции +, * на число
        bignum& operator+=(uint32_t);
        bignum& operator*=(uint32_t);
    };

}

// **** ввод и вывод ****
inline std::istringstream& operator>>(std::istringstream &in, mp::bignum& smth) {
    std::string s;
    in >> s;
    mp::bignum copy(s);
    smth = copy;
    return in;
}

inline std::ostringstream& operator<<(std::ostringstream &out, mp::bignum const& smth) {
    out << smth.to_string();
    return out;
}

// **** вспомогательные методы ***
inline mp::bignum& mp::bignum::operator+=(uint32_t x) {
    uint64_t rem = x;
    for (std::size_t i = 0; i < this->value.size(); i++) {
        uint64_t cur = this->value[i] + rem;
        this->value[i] = static_cast<uint32_t>(cur);
        rem = (cur >> 32u);
    }
    if (rem != 0) {
        this->value.push_back(rem);
    }
    return *this;
}

inline mp::bignum& mp::bignum::operator*=(uint32_t x) {
    uint64_t rem = 0;
    uint64_t copy_x = x;
    for (std::size_t i = 0; i < this->value.size(); i++) {
        uint64_t cur = static_cast<uint64_t>(this->value[i]) * copy_x + rem;
        this->value[i] = static_cast<uint32_t>(cur);
        rem = (cur >> 32u);
    }
    if (rem != 0) {
        this->value.push_back(rem);
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
// **** конструкторы ****
inline mp::bignum::bignum() {
    value.push_back(0);
}

inline mp::bignum::bignum(uint32_t x) {
    value.push_back(x);
}

inline mp::bignum::bignum(const std::string& s) {
    mp::bignum cur;
    for (std::size_t i = 0; i < s.size(); i++) {
        if (s[i] == '+') {
            continue;
        }
        *this = *this * static_cast<uint32_t>(10);
        cur = static_cast<int>(s[i] - '0');
        *this += cur;
    }
}

// **** копирование ****
inline mp::bignum::bignum(const bignum &copy) = default;

inline mp::bignum& mp::bignum::operator=(const bignum &copy) = default;

inline mp::bignum& mp::bignum::operator=(const uint32_t &copy) {
    this->value[0] = copy;
    return *this;
}

// **** преобразование к целому ****
inline mp::bignum::operator uint32_t() const {
    return value[0];
}


// **** получение строки ****
inline std::string mp::bignum::to_string() const{
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

// каст к bool
inline mp::bignum::operator bool() const {
    for (std::size_t i = 0; i < this->value.size(); ++i) {
        if (this->value[i] != 0) {
            return true;
        }
    }
    return false;
}

// +, *
inline mp::bignum &mp::bignum::operator+=(mp::bignum const& right){
    bignum right_ = right;
    int size = std::max(this->value.size(), right.value.size()) + 1;
    for (int i = 0; i < static_cast<int>(size - this->value.size()); i++) {
        this->value.push_back(0);
    }
    for (int i = 0; i < static_cast<int>(size - right.value.size()); i++) {
        right_.value.push_back(0);
    }
    uint64_t rem = 0;
    for (std::size_t i = 0; i < this->value.size(); i++) {
        uint64_t cur = static_cast<uint64_t>(this->value[i]) + right_.value[i] + rem;
        this->value[i] = static_cast<uint32_t>(cur);
        rem = (cur >> 32u);
    }

    if (rem != 0) {
        this->value.push_back(rem);
    }
    return *this;
}

inline mp::bignum& mp::bignum::operator*=(mp::bignum const& right) {
    std::vector<uint32_t> res(this->value.size() + right.value.size());
    for (std::size_t i = 0; i < this->value.size(); i++) {
        uint64_t rem = 0;
        for (std::size_t j = 0; j < right.value.size(); j++) {
            uint64_t cur = static_cast<uint64_t>(this->value[i])
                           * static_cast<uint64_t>(right.value[j])
                           + res[i + j] + rem;
            res[i + j] = static_cast<uint32_t>(cur);
            rem = (cur >> 32u);
        }
        if(rem != 0) {
            res[i + right.value.size()] += static_cast<uint32_t>(rem);
        }
    }
    this->value = res;
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
