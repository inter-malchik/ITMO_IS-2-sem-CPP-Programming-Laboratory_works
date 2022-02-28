#include <iostream>
#include <initializer_list>
#include <vector>
#include <cmath>
#include <exception>

const float PI = 3.14159265f;

class Dot {
public:
    explicit Dot(const int x = 0, const int y = 0) : x_(x), y_(y) {}

    Dot(const Dot &other) : x_(other.x_), y_(other.y_) {}

    Dot &operator=(const Dot &other) {
        if (&other == this)
            return *this;
        x_ = other.x_;
        y_ = other.y_;
        return *this;
    }

    int getx() const {
        return x_;
    }

    int gety() const {
        return y_;
    }

    float length_to(const Dot &other) const {
        return sqrt(pow(x_ - other.x_, 2) + pow(y_ - other.y_, 2));
    }

private:
    int x_;
    int y_;
};

class Bline {
public:
    explicit Bline(unsigned length = 0) : amount_(length) { dots_.resize(length); }

    explicit Bline(const std::vector<Dot> &dots) : dots_(dots), amount_(dots.size()) {}

    Bline(const std::initializer_list<Dot> &dotslist) : amount_(dotslist.size()) {
        for (const Dot &dot: dotslist)
            dots_.push_back(dot);
    }

    Bline(const Bline &other) : amount_(other.amount_), dots_(other.dots_) {}

    Bline &operator=(const Bline &other) {
        if (&other == this)
            return *this;
        amount_ = other.amount_;
        dots_ = other.dots_;
        return *this;
    }


protected:
    unsigned amount_;
    std::vector<Dot> dots_;
};

class BlineClosed : public Bline {
public:
    explicit BlineClosed(unsigned length = 0) : Bline(length) {}

    explicit BlineClosed(const std::vector<Dot> &dots) : Bline(dots) {
        calibrate();
    }

    BlineClosed(const std::initializer_list<Dot> &dotslist) : Bline(dotslist) {
        calibrate();
    }

    BlineClosed(const BlineClosed &other) : Bline(other) {}

    BlineClosed &operator=(const BlineClosed &other) {
        if (&other == this)
            return *this;
        amount_ = other.amount_;
        dots_ = other.dots_;
        return *this;
    }

    float perimeter() const {
        float answer = 0;
        for (unsigned i = 1; i < dots_.size(); i++) {
            answer += dots_[i].length_to(dots_[i - 1]);
        }
        return answer;
    }

    float area() const {
        int presummar = 0;
        for (unsigned i = 0; i < amount_ - 1; i++)
            presummar += (dots_[i].getx() + dots_[i + 1].getx()) * (dots_[i].gety() - dots_[i + 1].gety());
        presummar = abs(presummar);
        return ((float) presummar) * 0.5f;
    }

private:
    void calibrate() {
        if (!dots_.empty())
            if (dots_.front().getx() != dots_.back().getx() || dots_.front().gety() != dots_.back().gety()) {
                amount_ += 1;
                dots_.push_back(dots_.front());
            }
    }
};

class Polygon : public BlineClosed {
public:
    explicit Polygon(unsigned amount = 0) : BlineClosed(amount) {}

    explicit Polygon(const std::vector<Dot> &dots) : BlineClosed(dots) {}

    Polygon(const std::initializer_list<Dot> &dotlist) : BlineClosed(dotlist) {}

    Polygon(const Polygon &other) : BlineClosed(other) {}

    Polygon &operator=(const Polygon &other) {
        if (&other == this)
            return *this;
        dots_ = other.dots_;
        amount_ = other.amount_;
        return *this;
    }

private:
    unsigned amount_of_angles() {
        return amount_ - 1;
    }
};

class Triangle {
public:
    Triangle() : edges_(3, 0) {}

    Triangle(const Dot &Point1, const Dot &Point2, const Dot &Point3) :
            edges_{Point1.length_to(Point2), Point2.length_to(Point3), Point3.length_to(Point1)} {
        if (!check_correctess())
            throw std::invalid_argument("invalid triangle");
    }

    Triangle(const Triangle &other) : edges_(other.edges_) {}

    Triangle &operator=(const Triangle &other) {
        if (&other == this)
            return *this;
        edges_ = other.edges_;
        return *this;
    }

    float perimeter() const { return edges_[0] + edges_[1] + edges_[2]; }

    float area() const {
        float pp = perimeter() / 2;
        return sqrt(pp * (pp - edges_[0]) * (pp - edges_[1]) * (pp - edges_[2]));
    }

private:
    std::vector<float> edges_;

    bool check_correctess() {
        if (edges_[0] + edges_[1] > edges_[2])
            if (edges_[1] + edges_[2] > edges_[0])
                if (edges_[2] + edges_[0] > edges_[1])
                    return true;
        return false;
    }
};


class Trapeze {
public:
    Trapeze() : accs_(4, Accretion(Dot(), Dot())) {}

    Trapeze(const Dot &Point1, const Dot &Point2, const Dot &Point3, const Dot &Point4) :
            accs_{Accretion(Point1, Point2), Accretion(Point2, Point3), Accretion(Point3, Point4),
                  Accretion(Point4, Point1)} {
        if (!check_correctness())
            throw std::invalid_argument("invalid trapeze");
    }

    Trapeze(const Trapeze &other) : accs_(other.accs_) {}

    Trapeze &operator=(const Trapeze &other) {
        if (&other == this)
            return *this;
        accs_ = other.accs_;;
        return *this;
    }

    float perimeter() const {
        float answer{0};
        for (const AccretionVector &av: accs_)
            answer += av.length();
        return answer;
    }

    float area() const {
        float a{0}, b{0}, c{0}, d{0};
        for (const AccretionVector &av: accs_)
            if (av.base)
                if (!a)
                    a = av.length();
                else
                    b = av.length();
            else if (!c)
                c = av.length();
            else
                d = av.length();

        return ((a + b) / 2) * sqrt(c * c - pow((pow(b - a, 2) + c * c - d * d) / (2 * (b - a)), 2));
    }

protected:
    typedef struct Accretion {
        int dx;
        int dy;
        bool base = false;

        Accretion(const Dot &d1, const Dot &d2) : dx(d2.getx() - d1.getx()), dy(d2.gety() - d1.gety()) {}

        float length() const {
            return sqrt(pow(dx, 2) + pow(dy, 2));
        };

        bool null_vector() const {
            return (!dx && !dy);
        }

        bool collinear(const struct Accretion &other) {
            if (null_vector() or other.null_vector() or (!dx and !other.dx) or (!dy and !other.dy))
                return true;
            else
                return ((float) dx / (float) other.dx) == ((float) dy / (float) other.dy);

        }
    } AccretionVector;

    static bool is_base(const AccretionVector &av) {
        return av.base;
    }

private:
    std::vector<AccretionVector> accs_;

    bool check_correctness() {
        for (const AccretionVector &av: accs_)
            if (av.null_vector())
                return false;

        for (unsigned i = 0; i < accs_.size(); i++)
            for (unsigned j = i + 1; j < accs_.size(); j++)
                if (accs_[i].collinear(accs_[j]))
                    accs_[i].base = accs_[j].base = true;

        if (std::count_if(accs_.begin(), accs_.end(), is_base) != 2)
            return false;
        return true;
    }
};

class RegularPolygon {
public:
    RegularPolygon(int amount, float a = 1.0f) : amount_(amount), a_(a)
    {}

    RegularPolygon(const RegularPolygon &other) : amount_(other.amount_), a_(other.a_)
    {}

    RegularPolygon &operator=(const RegularPolygon &other) {
        if (&other == this)
            return *this;
        amount_ = other.amount_;
        a_ = other.a_;
        return *this;
    }

    float perimeter() const {
        return a_ * amount_;
    }

    float area() const {
        float perimeter = this->perimeter();
        float apopheme = a_ / (2 * tan(PI / amount_));
        return (perimeter * apopheme) / 2;
    }

private:
    int amount_;
    float a_;
};

/*
int main() {
    BlineClosed square = {Dot(0, 0), Dot(3, 0), Dot(3, 2), Dot(0, 2)};
    std::cout << square.perimeter() << std::endl;
    std::cout << square.area() << std::endl;
    std::cout << std::endl;

    RegularPolygon hexagon(6);
    std::cout << hexagon.perimeter() << std::endl;
    std::cout << hexagon.area() << std::endl;
    std::cout << std::endl;

    Triangle ph = {Dot(2, 1), Dot(5, 2), Dot(4, 8)};
    std::cout << ph.area() << std::endl;
    std::cout << std::endl;

    Polygon ph2 = {Dot(2, 1), Dot(5, 2), Dot(4, 8)};
    std::cout << ph2.area() << std::endl;
    std::cout << std::endl;

    Trapeze trap(Dot(0, 0), Dot(8, 0), Dot(6, 5), Dot(2, 5));
    std::cout << trap.perimeter() << ' ' << trap.area() << std::endl;

    Triangle tr(Dot(0, 0), Dot(8, 0), Dot(4, 5));
    std::cout << tr.perimeter() << ' ' << tr.area() << std::endl;

    Triangle tr(Dot(0, 0), Dot(0, 0), Dot(4, 5));
    Trapeze trap(Dot(0, 0), Dot(0, 0), Dot(0, 0), Dot(0, 0));
}
*/