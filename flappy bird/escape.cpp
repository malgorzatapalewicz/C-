#include "escape.h"
#include "time.h"
#include "windows.h"
#include "iostream"
#include "fstream"

Escape::Escape()
{
    window = std::make_shared<sf::RenderWindow>
            (sf::VideoMode(1000, 600),
             "Flappy Bird",
             sf::Style::Titlebar | sf::Style::Close);

    window->setPosition(sf::Vector2i(0, 0));
    window->setFramerateLimit(60);

    gravity = frame = { 0.f };
    space = { 220.f };
    count = { 0 };

    bg.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\background.png");  // plik tla
    flappy.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\character.png"); //plik postaci (student)
    column.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\column.png"); //plik przeszkod (kolumn)

    background = std::make_shared<sf::Sprite>();
    student = std::make_shared<sf::Sprite>();
    columnBottom = std::make_shared<sf::Sprite>();
    columnTop = std::make_shared<sf::Sprite>();


    background->setTexture(bg); //przypisanie plikow do rzeczy
    student->setTexture(flappy);
    columnBottom->setTexture(column);
    columnTop->setTexture(column);

    student->setPosition
            (500.f - flappy.getSize().x / 2.f, 300.f - flappy.getSize().y / 2.f); //pozycja startowa flappiego?

    student->setScale(1.f, 1.f); //wielkosc postaci
    student->setTextureRect(sf::IntRect(0, 0, 65, 60));

    columnBottom->setScale(1.5f, 1.5f);
    columnTop->setScale(1.5f, -1.5f); //wielkosc kolumn na gorze i dole
    odczyt.open("highscore.txt", std::ios::in);
    odczyt >> hscore;
    gameover = add = { true };
    score = { 0 };
    highscore = { hscore };
    odczyt.close();
    font.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\font.ttf");  // sciezka do pliku z czcionka

    txt_gameover.setFont(font);
    txt_gameover.setString("Press space to play"); // co wyswietla sie po przegraniu
    txt_gameover.setPosition(200, 300); //pozycja napisu w oknie
    txt_gameover.setCharacterSize(50); //rozmiar liter
    txt_gameover.setOutlineThickness(3); //rozmiar obrysu liter

    txt_highscore.setFont(font);
    txt_highscore.setString("Current highscore: "); // co wyswietla sie po przegraniu
    txt_highscore.setPosition(320, 375); //pozycja napisu w oknie
    txt_highscore.setCharacterSize(25); //rozmiar liter
    txt_highscore.setOutlineThickness(1); //rozmiar obrysu liter

    txt_score.setFont(font);
    txt_score.setString(std::to_string(score));
    txt_score.setPosition(10.f, 10.f);
    txt_score.setCharacterSize(50);
    txt_score.setOutlineThickness(3);

    txt_hscore.setFont(font);
    txt_hscore.setString(std::to_string(hscore));
    txt_hscore.setPosition(600, 375);
    txt_hscore.setCharacterSize(25);
    txt_hscore.setOutlineThickness(1);
}


void Escape::events()
{
    auto e = std::make_shared<sf::Event>();
    while (window->pollEvent( *e ))
    {
        if (e->type == sf::Event::Closed)
        {
            window->close();
        }
    }
    if (score > hscore)
    {
        zapis.open("highscore.txt", std::ios::out);
        zapis << score;
        zapis.close();
        hscore = score;
    }

    if (gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //sterowanieeeeeeeeeeeeee
    {
        score = 0;
        txt_score.setString(std::to_string(score));
        columns.clear(); //restartowanie gry
        student->setPosition
                (500.f - flappy.getSize().x / 2.f, 300.f - flappy.getSize().y / 2.f); //powrot do pozycji startowej

        gameover = false;
    }

}

void Escape::draw()
{

    window->clear(sf::Color::Black); //kolor pustego t�a
    window->draw(*background);

    for (auto& p : columns)
    {
        window->draw(p);
    }

    window->draw(*student);

    if (gameover)
    {
        window->draw(txt_gameover);
        window->draw(txt_highscore);
        window->draw(txt_hscore);
    }

    window->draw(txt_score);

    window->display();

}

void Escape::run()
{

    while (window->isOpen())
    {
        events();
        game();
        draw();

        ++count;
        if (count == 300)
        {
            count = 0;
        }
    }

}

void Escape::moveColumns() //przesuwa kolumny
{

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        gravity = -8.f;
        student->setRotation(-frame - 10.f);
    }
    else
    {
        student->setRotation(frame - 10.f);
    }

    if (count % 150 == 0)
    {
        int pos = std::rand() % 275 + 175;

        columnBottom->setPosition(1000, pos + space);
        columnTop->setPosition(1000, pos);

        columns.push_back(*columnBottom);
        columns.push_back(*columnTop);
    }

    for (std::size_t i{}; i < columns.size(); ++i)
    {
        if (columns[i].getGlobalBounds().intersects(student->getGlobalBounds())) // granice rur, ktorych nie mozna dotknac
        {
            student->move(15.f, 0); //skala bledu? ruch ptaka nachodzacego na rury?

            if (columns[i].getScale().y < 0)  //tego wgl nie rozumiem mozna usunac, nie wplywa to na gre
            {
                student->move(0, -15.f);
            }
            else
            {
                student->move(0, 15.f);
            }

            gameover = true;
        }



        if (columns[i].getPosition().x < -100)
        {
            columns.erase(columns.begin() + i); //znikanie i pojawianie sie kolejnych kolumn
        }

        columns[i].move(-2.f, 0);

        if (columns[i].getPosition().x == 448 && !add)
        {
            txt_score.setString(std::to_string(++score));
            add = true;
        }
        else
        {
            add = false;
        }

        if(score>4) // przyspieszanie kolumn
        {
            columns[i].move(-3.f, 0);
        }

    }
}
void Escape::game()
{
    if (!gameover) //implementacja jesli gra trwa
    {
        moveStudent();
        moveColumns();
    }
}


void Escape::moveStudent()
{
    student->move(0, gravity);  //bez ingerencji gracza na postac dziala tylko grawitacja
    gravity += 0.5f;
}
