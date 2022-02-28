#ifndef C___POLYNOMIAL_HPP
#define C___POLYNOMIAL_HPP

#include <vector>
#include <initializer_list>
#include <iostream>
#include <map>

class Polynomial {
public:
    explicit Polynomial(unsigned degree = 0);

    Polynomial(const std::initializer_list<int> &coefflist);

    explicit Polynomial(const std::vector<int> &coeffvector);

    ~Polynomial() = default;

    Polynomial(const Polynomial &other);

    Polynomial &operator=(const Polynomial &other);

    friend bool operator==(const Polynomial &p1, const Polynomial &p2);

    friend bool operator!=(const Polynomial &p1, const Polynomial &p2);

    friend Polynomial operator+(const Polynomial &p1, const Polynomial &p2);

    Polynomial &operator+=(const Polynomial &other);

    Polynomial operator-();

    friend Polynomial operator-(const Polynomial &p1, const Polynomial &p2);

    Polynomial &operator-=(const Polynomial &other);

    friend Polynomial operator*(const Polynomial &p, int n);

    Polynomial &operator*=(int n);

    friend Polynomial operator*(const Polynomial &p1, const Polynomial &p2);

    Polynomial &operator*=(const Polynomial &other);

    friend Polynomial operator/(const Polynomial &p, int n);

    Polynomial &operator/=(int n);

    int operator[](unsigned index);

    friend std::ostream &operator<<(std::ostream &stream, const Polynomial &other);

    friend std::istream &operator>>(std::istream &stream, Polynomial &other);

    unsigned get_degree() const;

    int get_y(int x) const;

    void setcoeff(unsigned x, int y);

protected:
    static int fastpow(int a, unsigned k);

private:
    std::map<unsigned, int> coeffs_;
};

#endif //C___POLYNOMIAL_HPP
