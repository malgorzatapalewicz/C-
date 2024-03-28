#pragma once
#include <fstream>

#include <SFML/Graphics.hpp>
#include <memory>

class Escape
{
    std::shared_ptr<sf::RenderWindow> window;
    float gravity, frame, space;
    int count, score, highscore, hscore;
    std::fstream zapis, odczyt;
    sf::Texture bg, flappy, column;
    std::shared_ptr<sf::Sprite> background, student, columnBottom, columnTop;
    std::vector<sf::Sprite> columns;
    bool gameover, add;

    sf::Font font;
    sf::Text txt_score, txt_gameover, txt_highscore, txt_hscore;

protected:
    void events();
    void draw();
    void game();
    void moveColumns();
    void moveStudent();
public:
    Escape();
    void run();

};