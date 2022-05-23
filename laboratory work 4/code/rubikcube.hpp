#ifndef KUB_RUBIKCUBE_HPP
#define KUB_RUBIKCUBE_HPP

#include <iostream>
#include <fstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <tuple>
#include <exception>
#include <functional>
#include <algorithm>
#include <random>

#define clear_tag_opening(s) do {(s).erase((s).size()-1); (s).erase(0,1);} while(0)
#define clear_tag_closing(s) do {(s).erase((s).size()-1); (s).erase(0,2);} while(0)

enum KColor {
    uncolored = 0,
    red,
    blue,
    white,
    orange,
    green,
    yellow
};

enum class KRotationTypes {
    F, R, L, U, D, B, S, M, E
};

enum class KOrientTypes {
    x, y, z
};

enum KRotationDirections {
    clockwise = 0, counterclockwise = 1
};

enum KRotationAmounts {
    once = 1, twice = 2
};

class RubikCube;

class CubeSolver;

class KubikGran {
public:
    explicit KubikGran(enum KColor chvet = uncolored) : data(3, std::vector(3, chvet)) {};

    std::vector<std::reference_wrapper<enum KColor>> get_row(size_t index);

    std::vector<std::reference_wrapper<enum KColor>> get_column(size_t index);

    friend std::ostream &operator<<(std::ostream &stream, const KubikGran &gran);

    friend std::istream &operator>>(std::istream &stream, KubikGran &gran);

    enum KColor get_center() const;

    enum KColor get_color(size_t x, size_t y) const;

    size_t count(KColor rcolor) const;

private:
    void transpose(enum KRotationDirections dir = clockwise);

    bool crest() const;

    bool colored() const;

    friend RubikCube;
    friend CubeSolver;

    friend void rotate(RubikCube &cube, KRotationTypes MD, KRotationDirections DR, KRotationAmounts AMT);

    std::vector<std::vector<enum KColor>> data;
};


class RubikCube {
public:
    RubikCube() :
            sides({KubikGran(red), KubikGran(blue), KubikGran(orange),
                   KubikGran(green), KubikGran(white), KubikGran(yellow)}) {};

    RubikCube(const RubikCube &other) :
            sides(other.sides), name(other.name) {};

    friend std::ostream &operator<<(std::ostream &stream, const RubikCube &cube);

    friend std::istream &operator>>(std::istream &stream, RubikCube &cube);

    bool check_correctness() const;

    void print(std::ostream &stream = std::cout);

    std::vector<KubikGran>& getsides();

    friend void rotate(RubikCube &cube, KRotationTypes MD, KRotationDirections DR, KRotationAmounts AMT);

    enum sidenames {
        F = 0, R = 1, B = 2, L = 3, U = 4, D = 5
    };

private:
    friend KubikGran;
    friend CubeSolver;
    std::vector<KubikGran> sides;
    std::string name = "C";
};

void rotate(RubikCube &cube, KRotationTypes MD, KRotationDirections DR = clockwise, KRotationAmounts AMT = once);

void reorient(RubikCube &cube, KOrientTypes OR, KRotationDirections DR = clockwise);

void scan_rotations(RubikCube &cube, std::istream &stream = std::cin);

void mix(RubikCube &cube, size_t precision = 20);

bool opposite_color(KColor one, KColor two);

class CubeSolver {
public:
    CubeSolver(RubikCube &rubik_cube) : cube(rubik_cube) {}

    CubeSolver(const CubeSolver &) = delete;

    void operator=(const CubeSolver &) = delete;

    RubikCube &solve();

    std::string solve_path;
private:
    RubikCube cube;

    void blind_edges();

    void blind_angles();

    void add_file_to_solve(std::istream &stream);

    void rotate_to_solve(const std::string& part, KRotationTypes MD,
                         KRotationDirections DR = clockwise, KRotationAmounts AMT = once);

    std::pair<KColor, KColor> get_baze_edge() const;

    std::tuple<KColor, KColor, KColor> get_base_angle() const;

    std::tuple<KColor, KColor, KColor> get_aus() const;

    std::pair<KColor, KColor> get_zapad() const;

    std::pair<KColor, KColor> get_yug() const;


    bool all_crests() const;

    bool finished() const;

    void zapad();

    void yug();

    void equator();

    void australia();

    void axes_read();
};


#endif //KUB_RUBIKCUBE_HPP
