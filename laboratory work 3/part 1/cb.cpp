#include <iostream>
#include <exception>
#include <initializer_list>
#include <algorithm>
//#include <iterator>

namespace utl {
    template<typename T>
    class CBiterator;

    template<typename T>
    class CircularBuffer {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

        explicit CircularBuffer(size_type size = 0) : size_(size), data_(new T[size_]), head(-1), tail(-1) {}

        CircularBuffer(size_type size, const T &value) : CircularBuffer(size) {
            fill(value);
        }

        CircularBuffer(const std::initializer_list<T> init_list) : CircularBuffer(init_list.size()) {
            for (auto elem = init_list.begin(); elem != init_list.end(); ++elem) {
                push_back(*elem);
            }
        }

        ~CircularBuffer() {
            delete[] data_;
        }

        CircularBuffer(const CircularBuffer &other) : size_(other.size_), data_(new T[other.size_]), head(other.head),
                                                      tail(other.tail) {

            std::memcpy(data_, other.data_, sizeof(T) * size_);
        }

        CircularBuffer &operator=(const CircularBuffer &other) {
            if (&other == this)
                return *this;

            delete[] data_;
            data_ = new T[other.size_];
            size_ = other.size_;
            head = other.head;
            tail = other.tail;
            std::memcpy(data_, other.data_, sizeof(T) * size_);

            return *this;
        }

        bool empty() const {
            return head == -1;
        }

        bool full() const {
            return (head + 1) % size_ == tail;
        }

        size_t size() const {
            return size_;
        }

        void push_back(const T &obj) {
            if (empty()) {
                tail++;
            } else if (full()) {
                throw std::logic_error("buffer is full");
            }
            head = (++head) % size_;
            data_[head] = obj;
        }

        void push_front(const T &obj) {
            if (empty()) {
                head = tail = size_ - 1;
                data_[tail] = obj;
            } else if (full()) {
                throw std::logic_error("buffer is full");
            }
            tail = (tail == 0) ? size_ - 1 : tail - 1;
            data_[tail] = obj;
        }

        reference take_back() {
            if (empty())
                throw std::logic_error("buffer is empty");
            T &answer = data_[head];
            if (head == tail)
                head = tail = -1;
            else
                head = (head == 0) ? size_ - 1 : head - 1;
            return answer;
        }

        reference take_front() {
            if (empty())
                throw std::logic_error("buffer is empty");
            T &answer = data_[tail];
            if (head == tail)
                head = tail = -1;
            else
                tail = (++tail) % size_;
            return answer;
        }

        reference back() const {
            if (empty())
                throw std::logic_error("buffer is empty");
            return data_[head];
        }

        reference front() const {

            if (empty())
                throw std::logic_error("buffer is empty");

            return data_[tail];
        }

        reference operator[](unsigned int index) {
            if (index >= size_)
                throw std::out_of_range("index exceeds possible range");
            return data_[(tail + index) % size_];
        }

        void resize(size_t new_size) {
            T *newdata = new T[new_size];
            memcpy(newdata, data_, ((new_size > size_) ? size_ : new_size) * sizeof(T));

            size_ = new_size;
            delete[] data_;
            data_ = newdata;
        }

        void fill(const T &obj) {
            for (size_t i = 0; i < size_; i++) {
                data_[i] = obj;
            }
            tail = 0;
            head = size_ - 1;
        }

        void clear() {
            head = tail = -1;
            size_ = 0;
            delete[] data_;
            data_ = nullptr;
        }

        friend class CBiterator<T>;

        using iterator = CBiterator<T>;
        using const_iterator = CBiterator<const T>;

        iterator begin() {
            return iterator(this, tail);
        }

        iterator end() {
            return iterator(this, tail + size_);
        }

        const_iterator cbegin() {
            return const_iterator(this, tail);
        }

        const_iterator cend() {
            return const_iterator(this, tail + size_);
        }

        friend std::ostream &operator<<(std::ostream &stream, CircularBuffer &value) {
            for (auto i = value.begin(); i != value.end(); ++i)
                stream << *i << ' ';
            stream << std::endl;
            return stream;
        }

    private:
        size_t size_;
        int head, tail;
        T *data_;
    };

    template<typename T>
    class CBiterator {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::random_access_iterator_tag;

        CBiterator(CircularBuffer<T> *buff, size_t ind) : buffer_(buff), index_(ind) {}

        CBiterator(CBiterator const &other) : CBiterator(other.buffer_, other.index_) {}

        reference operator*() const { return buffer_->data_[index_ % buffer_->size_]; }

        reference operator[](size_type index) const { return *(*this + index); }

        pointer operator->() { return *buffer_->data_[index_ % buffer_->size_]; }

        CBiterator &operator=(CBiterator const &other) = default;

        CBiterator &operator++() {
            index_++;
            return *this;
        }

        CBiterator &operator--() {
            index_--;
            return *this;
        }

        CBiterator operator++(int) {
            CBiterator tmp = *this;
            ++(*this);
            return tmp;
        }

        CBiterator operator--(int) {
            CBiterator tmp = *this;
            --(*this);
            return tmp;
        }

        friend bool operator==(const CBiterator &a, const CBiterator &b) { return a.index_ == b.index_; }

        friend bool operator!=(const CBiterator &a, const CBiterator &b) { return a.index_ != b.index_; }

        friend bool operator>(const CBiterator &a, const CBiterator &b) { return a.index_ > b.index_; }

        friend bool operator<(const CBiterator &a, const CBiterator &b) { return a.index_ < b.index_; }

        friend bool operator>=(const CBiterator &a, const CBiterator &b) { return a.index_ >= b.index_; }

        friend bool operator<=(const CBiterator &a, const CBiterator &b) { return a.index_ <= b.index_; }

        friend CBiterator operator+(const CBiterator &a, const CBiterator &b) {
            return CBiterator(a.buffer_, mini(a.index_ + b.index_, a.buffer_->end().index_));
        }

        friend CBiterator operator+(const CBiterator &a, int b) {
            return CBiterator(a.buffer_, std::min(a.index_ + b, a.buffer_->end().index_));
        }

        friend CBiterator operator+(int a, const CBiterator &b) {
            return b + a;
        }

        friend difference_type operator-(const CBiterator &a, const CBiterator &b) {
            return a.index_ - b.index_;
        }

        friend CBiterator operator-(const CBiterator &a, int b) {
            return CBiterator(a.buffer_, std::max(a.index_ - b, a.buffer_->begin().index_));
        }

        friend CBiterator operator-(int a, const CBiterator &b) {
            return b - a;
        }

        CBiterator operator+=(const CBiterator &other) {
            index_ = std::min(index_ + other.b, buffer_->end().index_);
            return *this;
        }

        CBiterator operator+=(int other) {
            index_ = std::min(index_ + other, buffer_->end().index_);
            return *this;
        }

        CBiterator operator-=(const CBiterator &other) {
            index_ = std::max(index_ - other.b, buffer_->begin().index_);
            return *this;
        }

        CBiterator operator-=(int other) {
            index_ = std::max(index_ - other, buffer_->begin().index_);
            return *this;
        }

    private:
        size_t index_;
        CircularBuffer<T> *buffer_;
    };
}
/*
int main() {
    utl::CircularBuffer<unsigned> temp = {2, 2, 6, 2, 1, 0, 3, 6, 7, 2, 9};
    utl::CircularBuffer<unsigned>::iterator ass = temp.begin();
    for (auto const &i: temp)
        std::cout << i << ' ';
    std::cout << std::endl;
    std::sort(temp.begin(), temp.end(), std::less<>());
    for (auto const &i: temp)
        std::cout << i << ' ';
}
*/

