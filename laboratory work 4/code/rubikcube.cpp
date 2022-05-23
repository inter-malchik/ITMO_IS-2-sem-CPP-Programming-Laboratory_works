#include "rubikcube.hpp"

std::vector<std::reference_wrapper<enum KColor>> KubikGran::get_row(size_t index) {
    if (index >= 3)
        throw std::out_of_range("the cube's side size is 3x3");
    return {std::ref(data[index][0]), std::ref(data[index][1]), std::ref(data[index][2])};
}

std::vector<std::reference_wrapper<enum KColor>> KubikGran::get_column(size_t index) {
    if (index >= 3)
        throw std::out_of_range("the cube's side size is 3x3");
    return {std::ref(data[0][index]), std::ref(data[1][index]), std::ref(data[2][index])};
}

enum KColor KubikGran::get_center() const {
    return data[data.size() / 2][data[data.size() / 2].size() / 2];
}

size_t KubikGran::count(KColor rcolor) const {
    size_t answer = 0;
    for (size_t i = 0; i < data.size(); i++)
        for (size_t j = 0; j < data[i].size(); j++)
            if (data[i][j] == rcolor)
                answer++;
    return answer;
}

void KubikGran::transpose(enum KRotationDirections dir) {
    std::vector<std::vector<enum KColor>> rotated(data.size(), std::vector(data[0].size(), uncolored));
    for (int i = 0; i < rotated.size(); i++)
        for (int j = 0; j < rotated[i].size(); j++)
            if (dir == clockwise) // поворот по часовой стрелке ↻
                rotated[j][i] = data[rotated.size() - i - 1][j];
            else
                rotated[j][i] = data[i][rotated[0].size() - j - 1];
    data = rotated;
}

bool KubikGran::crest() const {
    return (data[1][1] == data[0][1]) &&
           (data[1][1] == data[1][0]) &&
           (data[1][1] == data[1][2]) &&
           (data[1][1] == data[2][1]);
}

bool KubikGran::colored() const {
    std::vector<size_t> color_counts(KColor::yellow
    +1, 0);
    for (const auto &side: data)
        for (KColor color: side)
            color_counts[color]++;
    return (!color_counts[0] &&
            std::any_of(color_counts.begin() + 1, color_counts.end(), [&](int i) { return i == 9; }));
}

std::ostream &operator<<(std::ostream &stream, const KubikGran &gran) {
    for (size_t i = 0; i < gran.data.size(); i++) {
        for (size_t j = 0; j < gran.data[0].size(); j++) {
            switch (gran.data[i][j]) {
                case uncolored:
                    stream << '_';
                    break;
                case red:
                    stream << 'R';
                    break;
                case blue:
                    stream << 'B';
                    break;
                case white:
                    stream << 'W';
                    break;
                case orange:
                    stream << 'O';
                    break;
                case green:
                    stream << 'G';
                    break;
                case yellow:
                    stream << 'Y';
                    break;
            }
            stream << ' ';
        }
        stream << std::endl;
    }

    return stream;
}

std::istream &operator>>(std::istream &stream, KubikGran &gran) {
    for (size_t i = 0; i < gran.data.size(); i++) {
        for (size_t j = 0; j < gran.data[0].size(); j++) {
            KColor current_color;
            char bufcol;
            stream >> bufcol;
            switch (bufcol) {
                case 'W':
                case 'w':
                    current_color = white;
                    break;
                case 'R':
                case 'r':
                    current_color = red;
                    break;
                case 'B':
                case 'b':
                    current_color = blue;
                    break;
                case 'O':
                case 'o':
                    current_color = orange;
                    break;
                case 'G':
                case 'g':
                    current_color = green;
                    break;
                case 'Y':
                case 'y':
                    current_color = yellow;
                    break;
                default:
                    current_color = uncolored;
                    break;
            }
            gran.data[i][j] = current_color;
        }

    }
    return stream;
}


std::ostream &operator<<(std::ostream &stream, const RubikCube &cube) {
    stream << '<' << cube.name << '>' << std::endl;
    for (size_t i = 0; i < cube.sides.size(); i++) {
        char sidedecl = (i == RubikCube::F) ? 'F' :
                        (i == RubikCube::R) ? 'R' :
                        (i == RubikCube::B) ? 'B' :
                        (i == RubikCube::L) ? 'L' :
                        (i == RubikCube::U) ? 'U' : 'D';
        stream << '<' << sidedecl << '>' << std::endl;
        stream << cube.sides[i];
        stream << '<' << '/' << sidedecl << '>' << std::endl;
    }


    stream << '<' << '/' << cube.name << '>' << std::endl;
    return stream;
}

std::istream &operator>>(std::istream &stream, RubikCube &cube) {
    std::string root_tag;
    stream >> root_tag;
    clear_tag_opening(root_tag);
    cube.name = root_tag;

    for (size_t i = 0; i < cube.sides.size(); i++) {
        std::string place_tag;
        stream >> place_tag;
        clear_tag_opening(place_tag);

        RubikCube::sidenames current_side;
        switch (place_tag[0]) {
            case 'U':
            case 'u':
                current_side = RubikCube::U;
                break;
            case 'D':
            case 'd':
                current_side = RubikCube::D;
                break;
            case 'F':
            case 'f':
                current_side = RubikCube::F;
                break;
            case 'L':
            case 'l':
                current_side = RubikCube::L;
                break;
            case 'B':
            case 'b':
                current_side = RubikCube::B;
                break;
            case 'R':
            case 'r':
                current_side = RubikCube::R;
                break;
            default:
                throw std::logic_error("Unsuppoted sidename declaration");
        }
        stream >> cube.sides[current_side];

        std::string place_end;
        stream >> place_end;
        clear_tag_closing(place_end);

        if (place_tag != place_end)
            throw std::logic_error("Unsuppoted sidename declaration");
    }
    std::string root_end;
    stream >> root_end;
    clear_tag_closing(root_end);
    if (root_tag != root_end)
        throw std::logic_error("Unsuppoted cube declaration");
    return stream;
}

bool RubikCube::check_correctness() const {
    std::vector<size_t> color_counts(sides.size() + 1, 0);
    for (const KubikGran &side: sides)
        for (const std::vector<KColor> &row: side.data)
            for (KColor cur_color: row)
                color_counts[cur_color]++;
    return (!color_counts[0] &&
            std::all_of(color_counts.begin() + 1, color_counts.end(), [&](int i) { return i == 9; }));
}

void RubikCube::print(std::ostream &stream) {
    stream << "UP FACE" << std::endl << sides[U] << std::endl;

    stream << std::setw(9) << "FRONT FACE" << ' ' << std::setw(11) << "RIGHT FACE" << ' ' << std::setw(12)
           << "BACK FACE" << ' ' << std::setw(13) << "LEFT FACE" << std::endl;

    for (size_t i = 0; i < sides[0].data.size(); i++) {
        stream << std::setw(0);
        auto row_f = sides[F].get_row(i),
                row_r = sides[R].get_row(i),
                row_b = sides[B].get_row(i),
                row_l = sides[L].get_row(i);
        for (auto &row: {row_f, row_r, row_b, row_l}) {
            for (auto element: row) {
                switch (element.get()) {
                    case uncolored:
                        stream << '_';
                        break;
                    case red:
                        stream << 'R';
                        break;
                    case blue:
                        stream << 'B';
                        break;
                    case white:
                        stream << 'W';
                        break;
                    case orange:
                        stream << 'O';
                        break;
                    case green:
                        stream << 'G';
                        break;
                    case yellow:
                        stream << 'Y';
                        break;
                }
                stream << ' ';
            }
            stream << std::setw(9);
        }
        stream << std::endl;
    }

    stream << std::endl;

    stream << "DOWN FACE" << std::endl << sides[D] << std::endl;
}

std::vector<KubikGran>& RubikCube::getsides() {
    return sides;
}

void rotate(RubikCube &cube, KRotationTypes MD, KRotationDirections DR, KRotationAmounts AMT) {
    using KSide = std::vector<std::reference_wrapper<enum KColor>>;
    std::vector<KSide> adj_sides(4);
    switch (MD) {
        case KRotationTypes::F:
            cube.sides[RubikCube::sidenames::F].transpose(DR);
            adj_sides = {cube.sides[RubikCube::sidenames::L].get_column(2),
                         cube.sides[RubikCube::sidenames::U].get_row(2),
                         cube.sides[RubikCube::sidenames::R].get_column(0),
                         cube.sides[RubikCube::sidenames::D].get_row(0)};
            std::reverse(adj_sides[0].begin(), adj_sides[0].end());
            std::reverse(adj_sides[3].begin(), adj_sides[3].end());
            break;
        case KRotationTypes::R:
            cube.sides[RubikCube::sidenames::R].transpose(DR);
            adj_sides = {cube.sides[RubikCube::sidenames::F].get_column(2),
                         cube.sides[RubikCube::sidenames::U].get_column(2),
                         cube.sides[RubikCube::sidenames::B].get_column(0),
                         cube.sides[RubikCube::sidenames::D].get_column(2)};
            std::reverse(adj_sides[2].begin(), adj_sides[2].end());
            break;
        case KRotationTypes::L:
            cube.sides[RubikCube::sidenames::L].transpose(DR);
            adj_sides = {cube.sides[RubikCube::sidenames::B].get_column(2),
                         cube.sides[RubikCube::sidenames::U].get_column(0),
                         cube.sides[RubikCube::sidenames::F].get_column(0),
                         cube.sides[RubikCube::sidenames::D].get_column(0)};
            std::reverse(adj_sides[0].begin(), adj_sides[0].end());
            break;
        case KRotationTypes::B:
            cube.sides[RubikCube::sidenames::B].transpose(DR);
            adj_sides = {cube.sides[RubikCube::sidenames::R].get_column(2),
                         cube.sides[RubikCube::sidenames::U].get_row(0),
                         cube.sides[RubikCube::sidenames::L].get_column(0),
                         cube.sides[RubikCube::sidenames::D].get_row(2)};
            std::reverse(adj_sides[2].begin(), adj_sides[2].end());
            std::reverse(adj_sides[3].begin(), adj_sides[3].end());
            break;
        case KRotationTypes::U:
            cube.sides[RubikCube::sidenames::U].transpose(DR);
            adj_sides = {cube.sides[RubikCube::sidenames::F].get_row(0),
                         cube.sides[RubikCube::sidenames::L].get_row(0),
                         cube.sides[RubikCube::sidenames::B].get_row(0),
                         cube.sides[RubikCube::sidenames::R].get_row(0)};
            break;
        case KRotationTypes::D:
            cube.sides[RubikCube::sidenames::D].transpose(DR);
            adj_sides = {cube.sides[RubikCube::sidenames::F].get_row(2),
                         cube.sides[RubikCube::sidenames::R].get_row(2),
                         cube.sides[RubikCube::sidenames::B].get_row(2),
                         cube.sides[RubikCube::sidenames::L].get_row(2)};
            break;
        case KRotationTypes::S:
            adj_sides = {cube.sides[RubikCube::sidenames::U].get_row(1),
                         cube.sides[RubikCube::sidenames::R].get_column(1),
                         cube.sides[RubikCube::sidenames::D].get_row(1),
                         cube.sides[RubikCube::sidenames::L].get_column(1)};
            std::reverse(adj_sides[2].begin(), adj_sides[2].end());
            std::reverse(adj_sides[3].begin(), adj_sides[3].end());
            break;
        case KRotationTypes::M:
            adj_sides = {cube.sides[RubikCube::sidenames::F].get_column(1),
                         cube.sides[RubikCube::sidenames::D].get_column(1),
                         cube.sides[RubikCube::sidenames::B].get_column(1),
                         cube.sides[RubikCube::sidenames::U].get_column(1)};
            std::reverse(adj_sides[2].begin(), adj_sides[2].end());
            break;
        case KRotationTypes::E:
            adj_sides = {cube.sides[RubikCube::sidenames::F].get_row(1),
                         cube.sides[RubikCube::sidenames::R].get_row(1),
                         cube.sides[RubikCube::sidenames::B].get_row(1),
                         cube.sides[RubikCube::sidenames::L].get_row(1)};
            break;
    }
    std::vector<KColor> buffer(adj_sides[0].size());
    size_t ind = 0;
    for (std::reference_wrapper<enum KColor> val: (DR == counterclockwise) ?
                                                  adj_sides[0] : adj_sides[adj_sides.size() - 1])
        buffer[ind++] = val.get();

    if (DR == counterclockwise) {

        for (size_t i = 0; i < adj_sides.size() - 1; i++)
            for (size_t j = 0; j < adj_sides[i].size(); j++)
                adj_sides[i][j].get() = adj_sides[i + 1][j].get();

        for (size_t i = 0; i < buffer.size(); i++) {
            adj_sides[adj_sides.size() - 1][i].get() = buffer[i];
        }
    } else {
        for (size_t i = adj_sides.size() - 1; i != 0; i--)
            for (size_t j = 0; j < adj_sides[i].size(); j++)
                adj_sides[i][j].get() = adj_sides[i - 1][j].get();

        for (size_t i = 0; i < buffer.size(); i++) {
            adj_sides[0][i].get() = buffer[i];
        }
    }

    if (AMT == twice) {
        rotate(cube, MD, DR, once);
    }
}

enum KColor KubikGran::get_color(size_t x, size_t y) const {
    return data[x][y];
}

void reorient(RubikCube &cube, KOrientTypes OR, KRotationDirections DR) {
    switch (OR) {
        case KOrientTypes::x:
            switch (DR) {
                case clockwise:
                    rotate(cube, KRotationTypes::M, counterclockwise);
                    rotate(cube, KRotationTypes::R, clockwise);
                    rotate(cube, KRotationTypes::L, counterclockwise);
                    break;
                case counterclockwise:
                    rotate(cube, KRotationTypes::M, clockwise);
                    rotate(cube, KRotationTypes::R, counterclockwise);
                    rotate(cube, KRotationTypes::L, clockwise);
                    break;
            }
            break;
        case KOrientTypes::y:
            switch (DR) {
                case clockwise:
                    rotate(cube, KRotationTypes::E, counterclockwise);
                    rotate(cube, KRotationTypes::D, counterclockwise);
                    rotate(cube, KRotationTypes::U, clockwise);
                    break;
                case counterclockwise:
                    rotate(cube, KRotationTypes::E, clockwise);
                    rotate(cube, KRotationTypes::D, clockwise);
                    rotate(cube, KRotationTypes::U, counterclockwise);
                    break;
            }

            break;
        case KOrientTypes::z:
            switch (DR) {
                case clockwise:
                    rotate(cube, KRotationTypes::F, clockwise);
                    rotate(cube, KRotationTypes::S, clockwise);
                    rotate(cube, KRotationTypes::B, counterclockwise);
                    break;
                case counterclockwise:
                    rotate(cube, KRotationTypes::F, counterclockwise);
                    rotate(cube, KRotationTypes::S, counterclockwise);
                    rotate(cube, KRotationTypes::B, clockwise);
                    break;
            }
    }
}

void scan_rotations(RubikCube &cube, std::istream &stream) {
    while (true) {
        std::string command;
        stream >> command;
        if (command.empty())
            return;
        KRotationTypes mode;
        switch (command[0]) {
            case 'F':
                mode = KRotationTypes::F;
                break;
            case 'R':
                mode = KRotationTypes::R;
                break;
            case 'L':
                mode = KRotationTypes::L;
                break;
            case 'B':
                mode = KRotationTypes::B;
                break;
            case 'U':
                mode = KRotationTypes::U;
                break;
            case 'D':
                mode = KRotationTypes::D;
                break;
            case 'S':
                mode = KRotationTypes::S;
                break;
            case 'M':
                mode = KRotationTypes::M;
                break;
            case 'E':
                mode = KRotationTypes::E;
                break;
            default:
                return;
        }

        if (command.size() > 1)
            switch (command[1]) {
                case '\'':
                    rotate(cube, mode, counterclockwise);
                    break;
                case '2':
                    rotate(cube, mode);
                    rotate(cube, mode);
                    break;
                default:
                    return;
            }
        else
            rotate(cube, mode);
    }
}

void mix(RubikCube &cube, size_t precision) {
    std::random_device random_cube_generator;
    std::uniform_int_distribution<int> distribution(1, 157);
    KRotationTypes modes[9] = {KRotationTypes::F, KRotationTypes::R, KRotationTypes::L,
                               KRotationTypes::U, KRotationTypes::D, KRotationTypes::B,
                               KRotationTypes::S, KRotationTypes::M, KRotationTypes::E};
    KRotationDirections directions[2] = {clockwise, counterclockwise};
    for (size_t i = 0; i < precision; i++)
        rotate(cube, modes[distribution(random_cube_generator) % 9], directions[distribution(random_cube_generator) % 2]);
}

bool opposite_color(KColor one, KColor two) {
    switch (one) {
        case white:
            return two == yellow;
        case blue:
            return two == green;
        case orange:
            return two == red;
        case green:
            return two == blue;
        case red:
            return two == orange;
        case yellow:
            return two == white;
        default:
            throw std::logic_error("uncolored");
    }
}

RubikCube &CubeSolver::solve() {
    axes_read();
    blind_edges();
    blind_angles();
    return cube;
}

void CubeSolver::blind_edges() {
    size_t inversions = 0;
    std::vector<std::vector<bool>> made_rebra(cube.sides.size() + 1,
                                              std::vector<bool>(cube.sides.size() + 1, false));
    for (const KColor &f_c: {white, orange, yellow, blue, red, green})
        for (const KColor &s_c: {white, orange, yellow, blue, red, green})
            if (f_c == s_c || opposite_color(f_c, s_c) || (f_c == red and s_c == white) ||
                (f_c == white and s_c == red))
                made_rebra[f_c][s_c] = true;
    bool resending = false;
    std::pair<KColor, KColor> base = get_baze_edge();
    while (!all_crests()) {
        if (inversions > 40)
            throw std::logic_error("error when doing crests");

        if ((base.first == red && base.second == white) ||
            (base.first == white && base.second == red)) {
            // ЗАТРУДНИТЕЛЬНАЯ СИТУАЦИЯ
            if (get_zapad() != std::pair(white, orange))
                zapad();
            else if (get_yug() != std::pair(white, green))
                yug();
            else
                for (const KColor &f_c: {white, orange, yellow, blue, red, green}) {
                    if (resending) break;
                    for (const KColor &s_c: {white, orange, yellow, blue, red, green})
                        if (f_c == s_c)
                            continue;
                        else if (!made_rebra[f_c][s_c]) {
                            base = {f_c, s_c};
                            resending = true;
                            break;
                        }
                }
        } else if (base.first == orange && base.second == white) {
            rotate_to_solve("L2", KRotationTypes::L, counterclockwise, twice);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("M'", KRotationTypes::M, counterclockwise);
            yug();
            rotate_to_solve("M", KRotationTypes::M, clockwise);
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
            rotate_to_solve("L2", KRotationTypes::L, counterclockwise, twice);
        } else if (base.first == orange && base.second == yellow) {
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("M'", KRotationTypes::M, counterclockwise);
            yug();
            rotate_to_solve("M", KRotationTypes::M, clockwise);
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
        } else if (base.first == orange && base.second == blue) {
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("E", KRotationTypes::E, clockwise);
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
            zapad();
            rotate_to_solve("L", KRotationTypes::L, clockwise);
            rotate_to_solve("E'", KRotationTypes::E, counterclockwise);
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
        } else if (base.first == white && base.second == orange) {
            zapad();
        } else if (base.first == white && base.second == blue) {
            rotate_to_solve("M2", KRotationTypes::M, clockwise, twice);
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            zapad();
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("M2", KRotationTypes::M, clockwise, twice);
        } else if (base.first == white && base.second == green) {
            yug();
        } else if (base.first == red && base.second == green) {
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
            rotate_to_solve("E'", KRotationTypes::E, counterclockwise);
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
            zapad();
            rotate_to_solve("L", KRotationTypes::L, clockwise);
            rotate_to_solve("E", KRotationTypes::E, clockwise);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
        } else if (base.first == red && base.second == blue) {
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("E", KRotationTypes::E, clockwise);
            rotate_to_solve("L", KRotationTypes::L, clockwise);
            zapad();
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
            rotate_to_solve("E'", KRotationTypes::E, counterclockwise);
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
        } else if (base.first == red && base.second == yellow) {
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
            rotate_to_solve("M'", KRotationTypes::M, counterclockwise);
            yug();
            rotate_to_solve("M", KRotationTypes::M, clockwise);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
        } else if (base.first == orange && base.second == green) {
            rotate_to_solve("E'", KRotationTypes::E, counterclockwise);
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
            rotate_to_solve("L", KRotationTypes::L, clockwise);
            zapad();
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("E", KRotationTypes::E, clockwise);
        } else if (base.first == green && base.second == orange) {
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
            zapad();
            rotate_to_solve("L", KRotationTypes::L, clockwise);
        } else if (base.first == blue && base.second == orange) {
            rotate_to_solve("L", KRotationTypes::L, clockwise);
            zapad();
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
        } else if (base.first == blue && base.second == yellow) {
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
            rotate_to_solve("M'", KRotationTypes::M, counterclockwise);
            yug();
            rotate_to_solve("M", KRotationTypes::M, clockwise);
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
        } else if (base.first == yellow && base.second == green) {
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            zapad();
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
        } else if (base.first == green && base.second == red) {
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
            rotate_to_solve("E2", KRotationTypes::E, counterclockwise, twice);
            rotate_to_solve("L", KRotationTypes::L, clockwise);
            zapad();
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
            rotate_to_solve("E2", KRotationTypes::E, counterclockwise, twice);
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
        } else if (base.first == green && base.second == white) {
            rotate_to_solve("M", KRotationTypes::M, clockwise);
            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            zapad();
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("M'", KRotationTypes::M, counterclockwise);
        } else if (base.first == green && base.second == yellow) {
            rotate_to_solve("M'", KRotationTypes::M, counterclockwise);
            yug();
            rotate_to_solve("M", KRotationTypes::M, clockwise);
        } else if (base.first == blue && base.second == white) {
            rotate_to_solve("M", KRotationTypes::M, clockwise);
            yug();
            rotate_to_solve("M'", KRotationTypes::M, counterclockwise);
        } else if (base.first == yellow && base.second == orange) {
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            zapad();
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
        } else if (base.first == blue && base.second == red) {
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
            rotate_to_solve("E2", KRotationTypes::E, clockwise, twice);
            rotate_to_solve("L'", KRotationTypes::L, counterclockwise);
            zapad();
            rotate_to_solve("L", KRotationTypes::L, clockwise);
            rotate_to_solve("E2", KRotationTypes::E, clockwise, twice);
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
        } else if (base.first == yellow && base.second == red) {
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            zapad();
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
        } else if (base.first == yellow && base.second == blue) {
            rotate_to_solve("D", KRotationTypes::D, clockwise);
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            zapad();
            rotate_to_solve("L2", KRotationTypes::L, clockwise, twice);
            rotate_to_solve("D", KRotationTypes::D, clockwise);
        }
        else
            std::cout << "unworked" << std::endl;
        std::cout << solve_path << std::endl;
        cube.print();
        std::cout << inversions << std::endl;
        if (resending) {
            resending = false;
            continue;
        }
        inversions++;
        made_rebra[base.first][base.second] = true;
        base = get_baze_edge();

    }
    /*
    if (inversions % 2 == 1) {
        std::cout << "экватор:" << std::endl;
        equator();
        cube.print();
    }*/
}

void CubeSolver::blind_angles() {
    //УГЛЫ
    size_t inversions = 0;
    std::vector<std::vector<std::vector<bool>>> made_angles(cube.sides.size() + 1,
                                                            std::vector<std::vector<bool>>(
                                                                    cube.sides.size() + 1,
                                                                    std::vector<bool>(
                                                                            cube.sides.size() +
                                                                            1, false)));
    for (const KColor &f_c: {white, orange, yellow, blue, red, green})
        for (const KColor &s_c: {white, orange, yellow, blue, red, green})
            for (const KColor &t_c: {white, orange, yellow, blue, red, green})
                if (f_c == s_c || s_c == t_c || f_c == t_c ||
                    opposite_color(f_c, s_c) || opposite_color(s_c, t_c) || opposite_color(f_c, t_c) ||
                    (f_c == white and s_c == orange and t_c == blue) ||
                    (s_c == white and t_c == orange and f_c == blue) ||
                    (t_c == white and f_c == orange and s_c == blue) ||
                    (f_c == white and s_c == blue and t_c == orange))
                    made_angles[f_c][s_c][t_c] = true;
    bool resending = false;
    std::tuple<KColor, KColor, KColor> base_angle = get_base_angle();
    while (!finished()) {
        if (base_angle == std::tuple<KColor, KColor, KColor>({white, orange, blue}) ||
            base_angle == std::tuple<KColor, KColor, KColor>({blue, white, orange}) ||
            base_angle == std::tuple<KColor, KColor, KColor>({orange, blue, white})) {
            if (get_aus() != std::tuple<KColor, KColor, KColor>(white, red, green))
                australia();
            else {
                for (const KColor &f_c: {white, orange, yellow, blue, red, green}) {
                    if (resending) break;
                    for (const KColor &s_c: {white, orange, yellow, blue, red, green}) {
                        if (resending) break;
                        if (f_c == s_c) continue;
                        for (const KColor &t_c: {white, orange, yellow, blue, red, green})
                            if (s_c == t_c || t_c == f_c)
                                continue;
                            else if (!made_angles[f_c][s_c][t_c]) {
                                base_angle = {f_c, s_c, t_c};
                                resending = true;
                                break;
                            }
                    }
                }
            }
        } else {
            switch (std::get<0>(base_angle)) {
                case white:
                    switch (std::get<1>(base_angle)) {
                        case red:
                            if (std::get<2>(base_angle) != green)
                                break;
                            australia();
                            break;
                        case green:
                            if (std::get<2>(base_angle) != orange)
                                break;
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            rotate_to_solve("D", KRotationTypes::D, clockwise);
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            australia();
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            break;
                        case blue:
                            if (std::get<2>(base_angle) != red)
                                break;
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            australia();
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            rotate_to_solve("D", KRotationTypes::D, clockwise);
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            break;
                    }
                    break;
                case green:
                    switch (std::get<1>(base_angle)) {
                        case red:
                            if (std::get<2>(base_angle) != yellow)
                                break;
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            australia();
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            break;
                        case orange:
                            if (std::get<2>(base_angle) != white)
                                break;
                            rotate_to_solve("F2", KRotationTypes::F, clockwise, twice);
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            australia();
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            rotate_to_solve("F2", KRotationTypes::F, clockwise, twice);
                            break;
                        case white:
                            if (std::get<2>(base_angle) != red)
                                break;
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            australia();
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            break;
                        case yellow:
                            if (std::get<2>(base_angle) != orange)
                                break;
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            australia();
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            break;
                    }
                    break;
                case orange:
                    switch (std::get<1>(base_angle)) {
                        case white:
                            if (std::get<2>(base_angle) != green)
                                break;
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            australia();
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            break;
                        case green:
                            if (std::get<2>(base_angle) != yellow)
                                break;
                            rotate_to_solve("D", KRotationTypes::D, clockwise);
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            australia();
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
                            break;
                        case yellow:
                            switch (std::get<2>(base_angle)) {
                                case blue:
                                    //оранжево желто синяя - D2 F' + австралия + F D2
                                    rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
                                    rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                                    australia();
                                    rotate_to_solve("F", KRotationTypes::F, clockwise);
                                    rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
                                    break;
                            }
                            break;
                    }
                    break;
                case red:
                    switch (std::get<1>(base_angle)) {
                        case white:
                            if (std::get<2>(base_angle) != blue)
                                break;
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            australia();
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            break;
                        case green:
                            if (std::get<2>(base_angle) != white)
                                break;
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            australia();
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            break;
                        case blue:
                            if (std::get<2>(base_angle) != yellow)
                                break;
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            australia();
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            break;

                            break;
                        case yellow:
                            if (std::get<2>(base_angle) != green)
                                break;
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            australia();
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            break;
                    }
                    break;
                case blue:
                    switch (std::get<1>(base_angle)) {
                        case orange:
                            if (std::get<2>(base_angle) != yellow)
                                break;
                            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            australia();
                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            rotate_to_solve("D2", KRotationTypes::D, clockwise, twice);
                            break;
                        case red:
                            if (std::get<2>(base_angle) != white)
                                break;

                            rotate_to_solve("R'", KRotationTypes::R, counterclockwise);
                            australia();
                            rotate_to_solve("R", KRotationTypes::R, clockwise);
                            break;


                        case yellow:
                            if (std::get<2>(base_angle) != red)
                                break;
                            //сине красно желтая - D' F' + австралия + F D
                            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
                            rotate_to_solve("F'", KRotationTypes::F, counterclockwise);
                            australia();
                            rotate_to_solve("F", KRotationTypes::F, clockwise);
                            rotate_to_solve("D", KRotationTypes::D, clockwise);
                            break;

                    }
                    break;
                case yellow:
                    switch (std::get<1>(base_angle)) {
                        case blue:
                            if (std::get<2>(base_angle) != orange)
                                break;
                            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            australia();
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            rotate_to_solve("D", KRotationTypes::D, clockwise);
                            break;
                        case green:
                            if (std::get<2>(base_angle) != red)
                                break;
                            rotate_to_solve("D", KRotationTypes::D, clockwise);
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            australia();
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            rotate_to_solve("D'", KRotationTypes::D, counterclockwise);
                            break;

                        case orange:
                            if (std::get<2>(base_angle) != green)
                                break;
                            rotate_to_solve("F2", KRotationTypes::F, clockwise, twice);
                            australia();
                            rotate_to_solve("F2", KRotationTypes::F, clockwise, twice);
                            break;

                        case red:
                            if (std::get<2>(base_angle) != blue)
                                break;
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            australia();
                            rotate_to_solve("R2", KRotationTypes::R, clockwise, twice);
                            break;
                    }
                    break;
            }
        }
        cube.print();
        made_angles[std::get<0>(base_angle)]
        [std::get<1>(base_angle)][std::get<2>(base_angle)] = true;
        if (resending) {
            resending = false;
            continue;
        }
        base_angle = get_base_angle();
        inversions++;
    }
}

void CubeSolver::add_file_to_solve(std::istream &stream) {
    std::string solving;
    stream >> solving;
    while (!solving.empty() and solving != ".") {
        solve_path += solving + " ";
        stream >> solving;
    }
}

void CubeSolver::rotate_to_solve(const std::string& part, KRotationTypes MD,
                                 KRotationDirections DR, KRotationAmounts AMT) {
    rotate(cube, MD, DR, AMT);
    solve_path += part + " ";
}

std::pair<KColor, KColor> CubeSolver::get_baze_edge() const {
    return {cube.sides[RubikCube::U].data[1][2],
            cube.sides[RubikCube::R].data[0][1]};
}

std::tuple<KColor, KColor, KColor> CubeSolver::get_base_angle() const {
    return {cube.sides[RubikCube::U].data[0][0],
            cube.sides[RubikCube::L].data[0][0],
            cube.sides[RubikCube::B].data[0][2]};
}

std::tuple<KColor, KColor, KColor> CubeSolver::get_aus() const {
    return {cube.sides[RubikCube::U].data[2][2],
            cube.sides[RubikCube::R].data[0][0],
            cube.sides[RubikCube::F].data[0][2]};
}

std::pair<KColor, KColor> CubeSolver::get_zapad() const {
    return {cube.sides[RubikCube::U].data[1][0],
            cube.sides[RubikCube::L].data[0][1]};
}

std::pair<KColor, KColor> CubeSolver::get_yug() const {
    return {cube.sides[RubikCube::U].data[2][1],
            cube.sides[RubikCube::F].data[0][1]};
}


bool CubeSolver::all_crests() const {
    for (auto const &gran: cube.sides)
        if (!gran.crest())
            return false;
    return true;
}

bool CubeSolver::finished() const {
    for (auto const &gran: cube.sides)
        if (!gran.colored())
            return false;
    return true;
}

void CubeSolver::zapad() {
    std::ifstream temp("special_moves/zapad.txt");
    if(!temp)
        throw std::logic_error("no file with special move. expected at special_moves/zapad.txt\ntry to run through terminal");
    scan_rotations(cube, temp);
    temp.seekg(0);
    add_file_to_solve(temp);
    temp.close();
}

void CubeSolver::yug() {
    std::ifstream temp("special_moves/yug.txt");
    if(!temp)
        throw std::logic_error("no file with special move. expected at special_moves/yug.txt\ntry to run through terminal");
    scan_rotations(cube, temp);
    temp.seekg(0);
    add_file_to_solve(temp);
    temp.close();
}

void CubeSolver::equator() {
    std::ifstream temp("special_moves/equator.txt");
    if(!temp)
        throw std::logic_error("no file with special move. expected at special_moves/equator.txt\ntry to run through terminal");
    scan_rotations(cube, temp);
    temp.seekg(0);
    add_file_to_solve(temp);
    temp.close();
}

void CubeSolver::australia() {
    std::ifstream temp("special_moves/australia.txt");
    if(!temp)
        throw std::logic_error("no file with special move. expected at special_moves/australie.txt\ntry to run through terminal");
    scan_rotations(cube, temp);
    temp.seekg(0);
    add_file_to_solve(temp);
    temp.close();
}

void CubeSolver::axes_read() {
    //add white to the upper
    if (cube.sides[RubikCube::sidenames::U].get_center() != white) {
        for (size_t i = 0; i < cube.sides.size(); i++)
            if (i == RubikCube::sidenames::U)
                continue;
            else if (cube.sides[i].get_center() == white)
                switch (i) {
                    case RubikCube::sidenames::F:
                        rotate(cube, KRotationTypes::M, counterclockwise);
                        solve_path += "M' ";
                        break;
                    case RubikCube::sidenames::R:
                        rotate(cube, KRotationTypes::S, counterclockwise);
                        solve_path += "S' ";
                        break;
                    case RubikCube::sidenames::B:
                        rotate(cube, KRotationTypes::M, clockwise);
                        solve_path += "M ";
                        break;
                    case RubikCube::sidenames::L:
                        rotate(cube, KRotationTypes::S, clockwise);
                        solve_path += "S ";
                        break;
                    case RubikCube::sidenames::D:
                        rotate(cube, KRotationTypes::S, clockwise, twice);
                        solve_path += "S2 ";
                        break;
                }
        if (cube.sides[RubikCube::D].get_center() != yellow)
            throw std::logic_error("incorrect invariant");
    }

    //add green! to the front
    if (cube.sides[RubikCube::sidenames::F].get_center() != green) {
        for (size_t i = 0; i < cube.sides.size(); i++)
            if (i == RubikCube::sidenames::U || i == RubikCube::sidenames::D
                || i == RubikCube::sidenames::F)
                continue;
            else if (cube.sides[i].get_center() == green) {
                switch (i) {
                    case RubikCube::sidenames::L:
                        rotate(cube, KRotationTypes::E, clockwise);
                        solve_path += "E ";
                        break;
                    case RubikCube::sidenames::R:
                        rotate(cube, KRotationTypes::E, counterclockwise);
                        solve_path += "E' ";
                        break;
                    case RubikCube::sidenames::B:
                        rotate(cube, KRotationTypes::E, clockwise, twice);
                        solve_path += "E2 ";
                        break;
                }
                break;
            }
        if (cube.sides[RubikCube::B].get_center() != blue)
            throw std::logic_error("incorrect invariant");
    }
    if (cube.sides[RubikCube::R].get_center() != red && cube.sides[RubikCube::L].get_center() != orange)
        throw std::logic_error("incorrect invariant");
}