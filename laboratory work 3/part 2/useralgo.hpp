#ifndef USERALGO_HPP
#define USERALGO_HPP

#include <functional>

namespace ualgo {
    /* возвращает true, если все элементы диапазона удовлетворяют
     * некоторому предикату. Иначе false */
    template<class InputIt, class UnaryPredicate>
    bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
        for (; first != last; ++first)
            if (!p(*first))
                return false;
        return true;
    }

    /* возвращает true, если хотя бы один из элементов диапазона
     * удовлетворяет некоторому предикату. Иначе false */
    template<class InputIt, class UnaryPredicate>
    bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
        for (; first != last; ++first)
            if (p(*first))
                return true;
        return false;
    }

    /* возвращает true, если все элементы диапазона не удовлетворяют
     * некоторому предикату. Иначе false */
    template<class InputIt, class UnaryPredicate>
    bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
        return !any_of(first, last, p);
    }

    /* возвращает true, если ровно один элемент диапазона удовлетворяет
     * некоторому предикату. Иначе false */
    template<class InputIt, class UnaryPredicate>
    bool one_of(InputIt first, InputIt last, UnaryPredicate p) {
        bool found = false;
        for (; first != last; ++first)
            if (p(*first)) {
                if (!found)
                    found = true;
                else
                    return false;
            }
        return true;
    }

    /* возвращает true, если все элементы диапазона находятся в
     * отсортированном порядке относительно некоторого критерия */
    template<class InputIt, class Comparator=std::less<>>
    bool is_sorted(InputIt first, InputIt last, Comparator p = Comparator()) {
        for (; first != last - 1; ++first)
            if (!p(*first, *(first + 1)))
                return false;
        return true;
    }

    /* возвращает true, если в диапазоне есть элемент, делящий
     * все элементы на удовлетворяющие и не удовлетворяющие
     * некоторому предикату. Иначе false. */
    template<class InputIt, class UnaryPredicate>
    bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p) {
        bool satisfy = true;
        for (; first != last; ++first)
            if (p(*first))
                if (!satisfy)
                    return false;
                else
                    continue;
            else
                satisfy = false;
        return true;
    }

    /* находит первый элемент, не равный заданному */
    template<class InputIt, class T>
    InputIt find_not(InputIt first, InputIt last, T obj) {
        for (; first != last; ++first)
            if (*first != obj)
                return first;
        return last;

    }

    /* находит первый элемент, равный заданному, с конца */
    template<class InputIt, class T>
    InputIt find_backward(InputIt first, InputIt last, T obj) {
        InputIt last_unfound = last;
        do {
            --last;
            if (*last == obj)
                return last;
        } while (last != first);
        return last_unfound;
    }

    /* возвращает true, если заданная последовательность является
     * палиндромом относительно некоторого условия. Иначе false. */
    template<class InputIt, class UnaryPredicate>
    bool is_palindrome(InputIt first, InputIt last, UnaryPredicate p) {
        --last;
        while (first != last && first + 1 != last) {
            if (p(*first) != p(*last))
                return false;
        }
        return true;
    }
}

#endif //USERALGO_HPP