#pragma once
#include <iosfwd>
#include <sstream>
#include <numeric>
#include <string>

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
class Q {
public:
    Q(): m{0}, n{1} {}
    Q(int32_t m, size_t n = 1): m{m}, n{n} {}
    Q(const std::string& str) { 
        std::stringstream ss(str);
        char skip;
        if (is_number(str)) {
            ss >> m;
            n = 1;
        }
        else {
            ss >> m >> skip >> n;
            normalize(m, n);
        }
    }
    Q(const Q& q): m{q.m}, n{q.n} {}
    Q(Q&& q): m{std::move(q.m)}, n{std::move(q.n)} {}
    Q& inverse() {
        auto buf = this->n;
        bool is_negative = this->m < 0;
        if (is_negative) this->n = std::abs(this->m);
        else this->n = this->m;
        this->m = is_negative ? -buf : buf;

        return *this;
    }
    Q& operator=(const Q& other) {
        this->n = other.n;
        this->m = other.m;
        return *this;
    }
    Q& operator=(Q&& other) {
        this->n = std::move(other.n);
        this->m = std::move(other.m);
        return *this;
    }
    int32_t get_m() const {
        return m;
    }
    size_t get_n() const {
        return n;
    }
    std::string to_string() const {
        if (m != n) {
            auto str_m = std::to_string(m);
            if (m == 0) return "0";
            if (n != 1)
                return str_m + "/" + std::to_string(n);
            else
                return str_m;
        }
        return "1";
    }
    friend inline bool operator==(const Q& lhs, const std::string& rhs) {
        return lhs.to_string() == rhs;
    }
    friend inline bool operator==(const std::string& lhs, const Q& rhs) {
        return rhs.to_string() == lhs;
    }
    friend inline bool operator==(const Q& lhs, const Q& rhs) {
        return lhs.n == rhs.n and rhs.m == lhs.m;
    }
    friend std::ostream& operator<<(std::ostream& os, const Q& q) {
        os << q.to_string();
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Q& q) {
        std::string tmp;
        is >> tmp;
        q = Q(tmp);
        return is;
    }
    friend Q operator-(Q lhs, Q rhs) {
        int32_t new_m = lhs.m*rhs.n - rhs.m*lhs.n;
        size_t new_n = lhs.n*rhs.n;
        normalize(new_m, new_n);
        return Q{new_m, new_n};
    }
    friend Q operator+(Q lhs, Q rhs) {
        int32_t new_m = lhs.m*rhs.n + rhs.m*lhs.n;
        size_t new_n = lhs.n*rhs.n;
        normalize(new_m, new_n);
        return Q{new_m, new_n};
    }
    friend Q operator+(Q lhs, int rhs) {
        int32_t new_m = lhs.m + rhs*lhs.n;
        size_t new_n = lhs.n;
        normalize(new_m, new_n);
        return Q{new_m, new_n};
    }
    friend Q operator+(int lhs, Q rhs) {
        int32_t new_m = rhs.m + lhs*rhs.n;
        size_t new_n = rhs.n;
        normalize(new_m, new_n);
        return Q{new_m, new_n};
    }
    Q& operator+=(const Q& rhs) {
        this->m = this->m*rhs.n + rhs.m*this->n;
        this->n = this->n*rhs.n;
        normalize(this->m, this->n);
        return *this;
    }
    Q& operator+=(int32_t rhs) {
        this->m = this->m + rhs*this->n;
        normalize(this->m, this->n);
        return *this;
    }
    friend Q operator*(Q lhs, Q rhs) {
        int32_t new_m = lhs.m*rhs.m;
        size_t new_n = lhs.n*rhs.n;
        normalize(new_m, new_n);
        return {new_m, new_n};
    }
    friend Q operator*(Q lhs, int rhs) {
        int32_t new_m = rhs*lhs.m;
        size_t new_n = lhs.n;
        normalize(new_m, new_n);
        return {new_m, new_n};
    }
    friend Q operator*(int lhs, Q rhs) {
        int32_t new_m = lhs*rhs.m;
        size_t new_n = rhs.n;
        normalize(new_m, new_n);
        return {new_m, new_n};
    }
    Q& operator*=(const Q& rhs) {
        this->m = this->m*rhs.m;
        this->n = this->n*rhs.n;
        normalize(this->m, this->n);
        return *this;
    }
    Q& operator*=(int32_t rhs) {
        this->m = rhs*this->m;
        normalize(this->m, this->n);
        return *this;
    }

private:
    int32_t m;
    size_t n;
    static void normalize(int32_t& m, size_t& n) {
        bool is_neg = m < 0;
        auto gcd = std::gcd(std::abs(m),n);
        if (is_neg) {
            m = std::abs(m);
        }
        for(;;) {
            m /= gcd;
            n /= gcd;
            gcd = std::gcd(m,n);
            if (gcd == 1) break;
            if (n == 1) break;
        } 
        if (is_neg) {
            m *= -1;
        }
    }
};
