#include "flappy.h"

FlappyBird::FlappyBird()
{
    window = std::make_shared<sf::RenderWindow>
            (sf::VideoMode(1000, 600), //rozmiar okna?
             "Flappy Bird",
             sf::Style::Titlebar | sf::Style::Close);

    window->setPosition(sf::Vector2i(0, 0));
    window->setFramerateLimit(60); //ilosc klatek na sek?

    gravity = frame = { 0.f };
    space = { 220.f };
    count = { 0 };

    bg.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\background.png");  // plik t�a
    flappy.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\flappy.png"); //plik postaci
    pipe.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\pipe.png"); //plik przeszkod

    background = std::make_shared<sf::Sprite>();
    bird = std::make_shared<sf::Sprite>();
    pipeBottom = std::make_shared<sf::Sprite>();
    pipeTop = std::make_shared<sf::Sprite>();


    background->setTexture(bg); //przypisanie plikow do rzeczy
    bird->setTexture(flappy);
    pipeBottom->setTexture(pipe);
    pipeTop->setTexture(pipe);

    bird->setPosition
            (500.f - flappy.getSize().x / 2.f, 300.f - flappy.getSize().y / 2.f); //pozycja startowa flappiego?

    bird->setScale(1.f, 1.f); //wielkosc postaci
    bird->setTextureRect(sf::IntRect(0, 0, 65, 60));

    pipeBottom->setScale(1.5f, 1.5f);
    pipeTop->setScale(1.5f, -1.5f); //wielkosc rur na gorze i dole


    gameover = add = { true };
    score = { 0 };

    font.loadFromFile("C:\\Users\\gosia\\Desktop\\PROJEKTY 20222023\\Testowy\\flappybird.ttf");  // sciezka do pliku z czcionka

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
}


void FlappyBird::events()
{
    auto e = std::make_shared<sf::Event>();
    while (window->pollEvent( *e ))
    {
        if (e->type == sf::Event::Closed)
        {
            window->close();
        }
    }

    if (gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //sterowanieeeeeeeeeeeeee
    {
        score = 0;
        txt_score.setString(std::to_string(score));
        pipes.clear(); //restartowanie gry
        bird->setPosition
                (500.f - flappy.getSize().x / 2.f, 300.f - flappy.getSize().y / 2.f); //powrot do pozycji startowej

        gameover = false;
    }

}

void FlappyBird::draw()
{

    window->clear(sf::Color::Black); //kolor pustego t�a
    window->draw(*background);

    for (auto& p : pipes)
    {
        window->draw(p);
    }

    window->draw(*bird);

    if (gameover)
    {
        window->draw(txt_gameover);
        window->draw(txt_highscore);
    }

    window->draw(txt_score);

    window->display();

}

void FlappyBird::run()
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

void FlappyBird::moveColumns() //przesuwa rury
{

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))  //sterowanieeeeeeeeeeeeee
    {
        gravity = -8.f;
        bird->setRotation(-frame - 10.f);
    }
    else
    {
        bird->setRotation(frame - 10.f);
    }

    if (count % 150 == 0)                 // liczby do ogarniecia czemu taK?
    {
        int pos = std::rand() % 275 + 175;

        pipeBottom->setPosition(1000, pos + space);
        pipeTop->setPosition(1000, pos);

        pipes.push_back(*pipeBottom);
        pipes.push_back(*pipeTop);
    }

    for (std::size_t i{}; i < pipes.size(); ++i)
    {
        if (pipes[i].getGlobalBounds().intersects(bird->getGlobalBounds())) // granice rur, ktorych nie mozna dotknac
        {
            bird->move(15.f, 0); //skala bledu? ruch ptaka nachodzacego na rury?

            if (pipes[i].getScale().y < 0)  //tego wgl nie rozumiem mozna usunac, nie wplywa to na gre
            {
                bird->move(0, -15.f);
            }
            else
            {
                bird->move(0, 15.f);
            }

            gameover = true;
        }



        if (pipes[i].getPosition().x < -100)
        {
            pipes.erase(pipes.begin() + i); //znikanie i pojawianie sie kolejnych rur
        }

        pipes[i].move(-2.f, 0);
        /*if(score>0 && score<2)
        {
            pipes[i].move(-2.5, 0);
        }*/
        /*if(score>2 && score<4)
        {
            pipes[i].move(-3.5, 0);
        }*/
       /* if(score>3 && score<5)
        {
            pipes[i].move(-4.f, 0);
        }*/

        if (pipes[i].getPosition().x ==448 && !add) // czemu zaczynamy od 448?
        {
            //std::cout << pipes[i].getPosition().x << '\n';  czemuuuu?
            txt_score.setString(std::to_string(++score));
            add = true;
        }
        else
        {
            add = false;
        }

        if(score==1)
        {
            pipes[i].move(-3.f, 0);
        }
    }

}

void FlappyBird::game()
{
    if (!gameover) //implementacja jesli gra trwa
    {
        moveBird();
        movePipes();
    }
}


void FlappyBird::moveBird()
{
    bird->move(0, gravity);  //bez ingerencji gracza na postac dziala tylko grawitacja
    gravity += 0.5f;
}