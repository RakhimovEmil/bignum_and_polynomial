#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "bignum.hpp"

namespace mp {
    class polynomial {
    private:
        std::vector<uint32_t> coef;

    public:
        inline explicit polynomial(std::string);

        inline uint32_t &at(uint32_t);

        inline uint32_t at(uint32_t) const;

        inline int operator()(int) const;

        inline uint32_t operator()(uint32_t) const;

        inline double operator()(double) const;

        inline bignum operator()(bignum) const;
    };
}

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

        if ( s[i] - '0' >= 0 && s[i] - '0' <= 9) {
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

uint32_t& mp::polynomial::at(uint32_t a) {
    if (a > this->coef.size() - 1) {
        this->coef.resize(a + 1);
    }
    return this->coef[a];
}

uint32_t mp::polynomial::at(uint32_t a) const {
    return this->coef[a];
}


int mp::polynomial::operator()(int x) const {
    int64_t b_1, b_2;
    b_1 = this->coef[this->coef.size() - 1];
    for (std::size_t i = this->coef.size() - 2; i >= 0; --i) {
        b_2 = this->coef[i] + b_1 * x;
        b_1 = b_2;

        if (i == 0) {
            break;
        }
    }
    int ans = static_cast<int>(b_1);
    return ans;
}

uint32_t mp::polynomial::operator()(uint32_t x) const {
    int64_t b_1, b_2;
    b_1 = this->coef[this->coef.size() - 1];
    for (std::size_t i = this->coef.size() - 2; i >= 0; --i) {
        b_2 = this->coef[i] + b_1 * x;
        b_1 = b_2;

        if (i == 0) {
            break;
        }
    }
    uint32_t ans = static_cast<uint32_t>(b_1);
    return ans;
}

double mp::polynomial::operator()(double x) const {
    double b_1, b_2;
    b_1 = this->coef[this->coef.size() - 1];
    for (std::size_t i = this->coef.size() - 2; i >= 0; --i) {
        b_2 = this->coef[i] + b_1 * x;
        b_1 = b_2;

        if (i == 0) {
            break;
        }
    }
    return b_1;
}


mp::bignum mp::polynomial::operator()(mp::bignum x) const {
    mp::bignum b_1, b_2;
    b_1 = this->coef[this->coef.size() - 1];
    for (std::size_t i = this->coef.size() - 2; i >= 0; --i) {
        b_2 = this->coef[i] + b_1 * x;
        b_1 = b_2;

        if (i == 0) {
            break;
        }
    }
    return b_1;
}
