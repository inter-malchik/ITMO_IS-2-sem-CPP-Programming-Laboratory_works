#include "polynomial.hpp"

Polynomial::Polynomial(unsigned degree) { coeffs_[degree] = 0; }

Polynomial::Polynomial(const std::initializer_list<int> &coefflist) {
    unsigned temp_deg{0};
    for (const int &coeff: coefflist)
        coeffs_[temp_deg++] = coeff;
}

Polynomial::Polynomial(const std::vector<int> &coeffvector) {
    unsigned temp_deg{0};
    for (const int &coeff: coeffvector)
        coeffs_[temp_deg++] = coeff;
}

Polynomial::Polynomial(const Polynomial &other) : coeffs_(other.coeffs_) {}

Polynomial &Polynomial::operator=(const Polynomial &other) {
    if (&other == this)
        return *this;
    coeffs_ = other.coeffs_;
    return *this;
}

bool operator==(const Polynomial &p1, const Polynomial &p2) {
    auto it1 = p1.coeffs_.begin();
    auto it2 = p2.coeffs_.begin();

    while (it1 != p1.coeffs_.end() && it2 != p2.coeffs_.end()) {
        // std::cout << it1->first << ' ' << it1->second << "; " << it2->first << ' ' << it2->second << std::endl;
        if (it1->first == it2->first && it1->second == it2->second) {
            it1 = (it1 != p1.coeffs_.end()) ? ++it1 : it1;
            it2 = (it2 != p2.coeffs_.end()) ? ++it2 : it2;
        } else if (it1->first == it2->first && it1->second != it2->second)
            return false;
        else if (it1->second && it2->second)
            return false;
        else {
            it1 = (!it1->second) ? ++it1 : it1;
            it2 = (!it2->second) ? ++it2 : it2;
        }
    }
    return true;
}

bool operator!=(const Polynomial &p1, const Polynomial &p2) {
    return !(p1 == p2);
}

Polynomial operator+(const Polynomial &p1, const Polynomial &p2) {
    Polynomial result(p1);
    for (auto coeff: p2.coeffs_) {
        if (result.coeffs_.find(coeff.first) == result.coeffs_.end())
            result.coeffs_[coeff.first] = coeff.second;
        else
            result.coeffs_[coeff.first] += coeff.second;
    }
    return result;
}

Polynomial &Polynomial::operator+=(const Polynomial &other) {
    for (auto coeff: other.coeffs_) {
        if (coeffs_.find(coeff.first) == coeffs_.end())
            coeffs_[coeff.first] = coeff.second;
        else
            coeffs_[coeff.first] += coeff.second;
    }
    return *this;
}

Polynomial Polynomial::operator-() {
    for (auto coeff: this->coeffs_)
        coeff.second *= -1;
    return *this;
}

Polynomial operator-(const Polynomial &p1, const Polynomial &p2) {
    Polynomial result(p1);
    for (auto coeff: p2.coeffs_) {
        if (result.coeffs_.find(coeff.first) == result.coeffs_.end())
            result.coeffs_[coeff.first] = -coeff.second;
        else
            result.coeffs_[coeff.first] -= coeff.second;
    }
    return result;
}

Polynomial &Polynomial::operator-=(const Polynomial &other) {
    for (auto coeff: other.coeffs_) {
        if (coeffs_.find(coeff.first) == coeffs_.end())
            coeffs_[coeff.first] = -coeff.second;
        else
            coeffs_[coeff.first] -= coeff.second;
    }
    return *this;
}

Polynomial operator*(const Polynomial &p, int n) {
    Polynomial result(p);
    for (auto coeff: result.coeffs_)
        result.coeffs_[coeff.first] = coeff.second * n;
    return result;
}

Polynomial &Polynomial::operator*=(int n) {
    for (auto coeff: this->coeffs_)
        coeff.second *= n;
    return *this;
}

Polynomial operator*(const Polynomial &p1, const Polynomial &p2) {
    Polynomial result(0);
    for (auto icoeff: p1.coeffs_) {
        if (icoeff.second) continue;
        for (auto jcoeff: p2.coeffs_) {
            if (jcoeff.second) continue;
            result.coeffs_[icoeff.first + jcoeff.first] += icoeff.second * jcoeff.second;
        }
    }
    return result;
}

Polynomial &Polynomial::operator*=(const Polynomial &other) {
    *this = *this * other;
    return *this;
}

Polynomial operator/(const Polynomial &p, int n) {
    Polynomial result(p);
    for (auto coeff: result.coeffs_)
        result.coeffs_[coeff.first] = coeff.second / n;
    return result;
}

Polynomial &Polynomial::operator/=(int n) {
    for (auto coeff: this->coeffs_)
        coeff.second /= n;
    return *this;
}

int Polynomial::operator[](unsigned index) {
    if (coeffs_.find(index) != coeffs_.end())
        return coeffs_[index];
    else
        return 0;
}

unsigned Polynomial::get_degree() const {
    if (!coeffs_.empty())
        return std::prev(coeffs_.end())->first;
    else
        return 0;
}


int Polynomial::get_y(int x) const {
    unsigned k = 0;
    int nome = 1;
    int y = 0;
    for (auto coeff: this->coeffs_) {
        if (!coeff.second) continue;
        if (coeff.first > k) {
            nome *= fastpow(x, coeff.first - k);
            k = coeff.first;
        }
        y += coeff.second * nome;
    }
    return y;
}

void Polynomial::setcoeff(unsigned x, int y) {
    coeffs_[x] = y;
}

int Polynomial::fastpow(int a, unsigned k) {
    int b = 1;
    while (k > 0) {
        if (k % 2 == 0) {
            k /= 2;
            a *= a;
        } else {
            k -= 1;
            b *= a;
        }
    }
    return b;
}

std::ostream &operator<<(std::ostream &stream, const Polynomial &other) {

    stream << "P(" << other.get_degree() << ") ";
    for (auto coeff: other.coeffs_)
        if (coeff.second)
            stream << coeff.first << "X^" << coeff.second << ' ';
    stream << std::endl;
    return stream;
}

std::istream &operator>>(std::istream &stream, Polynomial &other) {
    unsigned deg = 0;
    stream >> deg;
    for (unsigned i = 0; i <= deg; i++)
        stream >> other.coeffs_[i];
    return stream;
}
