#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <random>
#include "rubikcube.hpp"

#define CUBESIZE 70.0f
#define DELAY_KEY_TIME 0.15f
#define DELAY_VIS_TIME DELAY_KEY_TIME/10

class RubikCubeVisualizer {
public:
    RubikCubeVisualizer(RubikCube &cube) :
            sides(cube.getsides().size(),
                  std::vector(3, std::vector(3, sf::RectangleShape(sf::Vector2f(CUBESIZE, CUBESIZE))))) {
        update_colors(cube);
    }

    RubikCubeVisualizer(RubikCube &cube, sf::RenderWindow const &window) : RubikCubeVisualizer(cube) {
        set_positions(window);
    }

    void update_colors(RubikCube &cube) {
        for (size_t i = 0; i < sides.size(); ++i)
            for (size_t j = 0; j < sides[i].size(); ++j)
                for (size_t k = 0; k < sides[i][j].size(); ++k) {
                    enum KColor particle_color = cube.getsides()[i].get_color(j, k);
                    sides[i][j][k].setFillColor((particle_color == red) ? sf::Color::Red :
                                                (particle_color == blue) ? sf::Color::Blue :
                                                (particle_color == white) ? sf::Color::White :
                                                (particle_color == orange) ? sf::Color(255, 165, 0) :
                                                (particle_color == green) ? sf::Color::Green :
                                                (particle_color == yellow) ? sf::Color::Yellow :
                                                sf::Color::Black);
                }
    }

    void set_positions(sf::RenderWindow const &window) {
        sf::Vector2f center((float) window.getSize().x / 2 - CUBESIZE, (float) window.getSize().y / 2 - CUBESIZE);

        sf::Vector2f side_centers[6];

        for (enum RubikCube::sidenames cur_side: {RubikCube::sidenames::L,
                                                  RubikCube::sidenames::F,
                                                  RubikCube::sidenames::R,
                                                  RubikCube::sidenames::B}) {
            side_centers[cur_side] = sf::Vector2f(
                    center.x + ((cur_side == RubikCube::sidenames::L || cur_side == RubikCube::sidenames::F) ? -1 : 1) *
                               ((cur_side == RubikCube::sidenames::F || cur_side == RubikCube::sidenames::R) ? 1.5
                                                                                                             : 4.5) *
                               CUBESIZE, center.y
            );
        }

        side_centers[RubikCube::sidenames::U] = side_centers[RubikCube::sidenames::F] - sf::Vector2f(0, 3 * CUBESIZE);
        side_centers[RubikCube::sidenames::D] = side_centers[RubikCube::sidenames::F] + sf::Vector2f(0, 3 * CUBESIZE);


        for (size_t i = 0; i < sides.size(); ++i)
            for (size_t j = 0; j < sides[i].size(); ++j)
                for (size_t k = 0; k < sides[i][j].size(); ++k) {
                    size_t curpos = j * 3 + k;
                    sf::Vector2f center_cur = side_centers[i];
                    switch (curpos) {
                        case 0:
                            sides[i][j][k].setPosition(center_cur.x - CUBESIZE, center_cur.y - CUBESIZE);
                            break;
                        case 1:
                            sides[i][j][k].setPosition(center_cur.x, center_cur.y - CUBESIZE);
                            break;
                        case 2:
                            sides[i][j][k].setPosition(center_cur.x + CUBESIZE, center_cur.y - CUBESIZE);
                            break;
                        case 3:
                            sides[i][j][k].setPosition(center_cur.x - CUBESIZE, center_cur.y);
                            break;
                        case 4:
                            sides[i][j][k].setPosition(center_cur.x, center_cur.y);
                            break;
                        case 5:
                            sides[i][j][k].setPosition(center_cur.x + CUBESIZE, center_cur.y);
                            break;
                        case 6:
                            sides[i][j][k].setPosition(center_cur.x - CUBESIZE, center_cur.y + CUBESIZE);
                            break;
                        case 7:
                            sides[i][j][k].setPosition(center_cur.x, center_cur.y + CUBESIZE);
                            break;
                        case 8:
                            sides[i][j][k].setPosition(center_cur.x + CUBESIZE, center_cur.y + CUBESIZE);
                            break;
                    }
                }

    }

    void draw_all(sf::RenderWindow &window) const {
        for (auto const &sidename: sides)
            for (auto const &line: sidename)
                for (auto const &obj: line)
                    window.draw(obj);
    }

private:
    std::vector<std::vector<std::vector<sf::RectangleShape>>> sides;
};

int main(int argc, char *argv[]) {
    sf::RenderWindow window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Kubik",
            (argc == 1 && std::string(argv[0]) == std::string("-f")) ? sf::Style::Fullscreen : sf::Style::Close |
                                                                                               sf::Style::Titlebar);

    RubikCube kubik;
    kubik.print();
    RubikCubeVisualizer drawer(kubik, window);


    while (window.isOpen()) {
        sf::Event evnt;
        bool changed = true;

        while (window.pollEvent(evnt)) {
            switch (evnt.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    std::cout << evnt.size.width << ' ' << evnt.size.height << std::endl;
                    break;

            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            window.close();
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
            reorient(kubik, KOrientTypes::x);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
            reorient(kubik, KOrientTypes::y);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
            reorient(kubik, KOrientTypes::z);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            rotate(kubik, KRotationTypes::S);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M))
            rotate(kubik, KRotationTypes::M);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            rotate(kubik, KRotationTypes::E);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
            rotate(kubik, KRotationTypes::F);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            rotate(kubik, KRotationTypes::R);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
            rotate(kubik, KRotationTypes::L);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U))
            rotate(kubik, KRotationTypes::U);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            rotate(kubik, KRotationTypes::D);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B))
            rotate(kubik, KRotationTypes::B);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            std::random_device random_cube_generator;
            std::uniform_int_distribution<int> distribution(1, 157);
            KRotationTypes modes[9] = {KRotationTypes::F, KRotationTypes::R, KRotationTypes::L,
                                       KRotationTypes::U, KRotationTypes::D, KRotationTypes::B,
                                       KRotationTypes::S, KRotationTypes::M, KRotationTypes::E};
            KRotationDirections directions[2] = {clockwise, counterclockwise};
            for (size_t i = 0; i < 20; i++) {
                rotate(kubik, modes[distribution(random_cube_generator) % 9],
                       directions[distribution(random_cube_generator) % 2]);
                drawer.update_colors(kubik);
                sf::sleep(sf::seconds(DELAY_VIS_TIME/2));
                window.clear();
                drawer.draw_all(window);
                window.display();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
            CubeSolver solver(kubik);
            solver.solve();
            std::string filename = std::string("solve_") + std::to_string(time(0)) + std::string(".txt");

            std::ofstream temp(filename);
            temp << solver.solve_path << "." << std::endl;
            temp.close();

            std::ifstream strm(filename);

            while (!strm.eof()) {
                std::string command;
                strm >> command;
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
                        continue;
                }

                if (command.size() > 1)
                    switch (command[1]) {
                        case '\'':
                            rotate(kubik, mode, counterclockwise);
                            break;
                        case '2':
                            rotate(kubik, mode);
                            rotate(kubik, mode);
                            break;
                        default:
                            continue;
                    }
                else
                    rotate(kubik, mode);

                drawer.update_colors(kubik);
                sf::sleep(sf::seconds(DELAY_VIS_TIME));
                window.clear();
                drawer.draw_all(window);
                window.display();
            }
        } else
            changed = false;

        if (changed) {
            drawer.update_colors(kubik);
            sf::sleep(sf::seconds(DELAY_KEY_TIME));
        }

        window.clear();
        drawer.draw_all(window);
        window.display();
    }
}