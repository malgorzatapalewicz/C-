#pragma once

#include <SFML/Graphics.hpp>
#include <memory>


class Escape
{
    std::shared_ptr<sf::RenderWindow> window;
    float gravity, frame, space;
    int count, score;
    sf::Texture bg, flappy, column;
    std::shared_ptr<sf::Sprite> background, student, columnBottom, columnTop;
    std::vector<sf::Sprite> columns;
    bool gameover, add;

    sf::Font font;
    sf::Text txt_score, txt_gameover, txt_highscore;

protected:
    void events();
    void draw();
    void game();
    void moveColumns();
    //void setAnimeBird();
    void moveStudent();
public:
    Escape();
    void run();

};