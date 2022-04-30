#ifndef USER_METAFUCTIONS_HPP
#define USER_METAFUCTIONS_HPP
namespace meta {

    template<int X, unsigned N, bool even = N%2>
    struct fastpow {
        enum {
            value = X * fastpow<X, N - 1, (N - 1) % 2>::value
        };
    };

    template<int X, unsigned N>
    struct fastpow<X, N, true> {
        enum {
            value = X * fastpow<X, N - 1>::value
        };
    };

    template<int X, unsigned N>
    struct fastpow<X, N, false> {
        enum {
            value = fastpow<X, N / 2>::value * fastpow<X, N / 2>::value
        };
    };

    template<int X>
    struct fastpow<X, 1, true> {
        enum {
            value = X
        };
    };

    template<int X>
    struct fastpow<X, 0, false> {
        enum {
            value = 1
        };
    };

    template<>
    struct fastpow<0, 0, false> {
        enum {
            value = 1
        };
    };


    template<unsigned deg, unsigned order>
    struct derivative_coefficient_counter {
        enum {
            value = deg * derivative_coefficient_counter<deg - 1, order - 1>::value
        };
    };

    template<unsigned order>
    struct derivative_coefficient_counter<0, order> {
        enum {
            value = 0
        };
    };

    template<unsigned deg>
    struct derivative_coefficient_counter<deg, 0> {
        enum {
            value = 1
        };
    };

    template<>
    struct derivative_coefficient_counter<0, 0> {
        enum {
            value = 1
        };
    };


    template<int X, unsigned deg, int first, int ... coeffs>
    struct polynomial_counter {
        enum {
            value = first * fastpow<X, deg, deg % 2>::value + polynomial_counter<X, deg + 1, coeffs...>::value
        };
    };

    template<int X, unsigned deg, int first>
    struct polynomial_counter<X, deg, first> {
        enum {
            value = first * fastpow<X, deg, deg % 2>::value
        };
    };

    template<int X, unsigned order, unsigned deg, int first, int ... coeffs>
    struct derivative_counter {
        enum {
            value =
            first * derivative_coefficient_counter<deg, order>::value *
            fastpow<X, deg - order, (deg - order) % 2>::value
            + derivative_counter<X, order, deg + 1, coeffs...>::value
        };
    };

    template<int X, unsigned order, unsigned deg, int first>
    struct derivative_counter<X, order, deg, first> {
        enum {
            value = first * derivative_coefficient_counter<deg, order>::value *
                    fastpow<X, deg - order, (deg - order) % 2>::value
        };
    };

    template<int first, int... coeffs>
    struct Polynomial {
        template<int X, unsigned deg = 0>
        struct get_y {
            enum {
                value = first * fastpow<X, deg, deg % 2>::value + polynomial_counter<X, deg + 1, coeffs...>::value,
            };
        };
        template<int X, unsigned order = 1, unsigned deg = 0>
        struct derivative {
            enum {
                value = first * derivative_coefficient_counter<deg, order>::value
                        + derivative_counter<X, order, deg + 1, coeffs...>::value
            };
        };
    };

    template<int first>
    struct Polynomial<first> {
        template<int X, unsigned deg = 0>
        struct get_y {
            enum {
                value = first * fastpow<X, deg>::value
            };
        };
        template<int X, unsigned order = 1, unsigned deg = 0>
        struct derivative {
            enum {
                value = 0
            };
        };
    };


    template<int first, int... coeffs>
    void poly_print_handler(std::ostream &stream, unsigned deg, const Polynomial<first, coeffs...> &p) {
        if (first < 0)
            stream << first;
        else
            stream << '+' << first;
        stream << "·X^" << deg;
    }

    template<int first, int second, int... coeffs>
    void poly_print_handler(std::ostream &stream, unsigned deg, const Polynomial<first, second, coeffs...> &p) {
        if (first < 0)
            stream << first;
        else
            stream << '+' << first;
        stream << "·X^" << deg;
        poly_print_handler(stream, deg + 1, Polynomial<second, coeffs...>());
    }

    template<int first, int... coeffs>
    std::ostream &operator<<(std::ostream &stream, const Polynomial<first, coeffs...> &p) {
        stream << "P(X) = " << first;
        return stream;
    }

    template<int first, int second, int... coeffs>
    std::ostream &operator<<(std::ostream &stream, const Polynomial<first, second, coeffs...> &p) {
        stream << "P(X) = " << first;
        poly_print_handler(stream, 1, Polynomial<second, coeffs...>());
        return stream;
    }
}
#endif //USER_METAFUCTIONS_HPP
