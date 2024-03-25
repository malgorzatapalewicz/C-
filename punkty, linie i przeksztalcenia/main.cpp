#include <iostream>
#include <algorithm> //do min i max
#include <SFML/Graphics.hpp>
using namespace std;

class Point {
private:
    double x,y;

public:
    Point(double x,double y) : x(x), y(y){} //konstruktor
    double getX() const {return x;}
    double getY() const {return y;}
    void setX(double x) {this->x = x;}
    void setY(double y) {this->y = y;}
    void displayPoint(){
        cout << "(" << getX() << ", " << getY() << ")";
    }
};

sf::Vector2f pointToVector(const Point &point) {
    return sf::Vector2f(static_cast<float>(point.getX()), static_cast<float>(point.getY()));
}

class Line {
    Point startPoint; // (punkt poczatkowy)
    Point endPoint; // (punkt koncowy)

public:
    Line(Point start, Point end) : startPoint(start), endPoint(end) {}
    Point getStartPoint() const { return startPoint; }
    Point getEndPoint() const { return endPoint; }

    void displayLine(){
        cout << " poczatek: (" << startPoint.getX() << ", " << startPoint.getY() << ") koniec: (" << endPoint.getX() << ", " << endPoint.getY() << ")" << endl;
    }

};

struct LineEquation { //rownanie liniowe kierunkowe
    double a,b;
    LineEquation(double a, double b) : a(a), b(b) {}
    void displayLineEquation(){
        cout << "y = " << a << "x + " << b << endl;
    }
};

struct LineGenEquation { // rownianie liniowe ogolne - linear general equation
    double A,B,C;
    LineGenEquation(double a, double b, double c) : A(a), B(b), C(c) {}
    void displayLineGenEquation(){
        cout << A <<"x + " << B << "y + " << C << " = 0" << endl;
    }
};

LineGenEquation convertToLineGenEquation(LineEquation dirExuation){
    LineGenEquation equation(-dirExuation.a,1,-dirExuation.b);
    return equation;
}

LineEquation convertToLineDirEquation(LineGenEquation genEquation){
    LineEquation equation(-genEquation.A / genEquation.B, -genEquation.C / genEquation.B);
    return equation;
}

LineGenEquation findLineEquation (Line line){ //wyznaczenie rownania prostej, do ktorej nalezy dana linia
    Point start = line.getStartPoint();
    Point end = line.getEndPoint();

    double x1 = start.getX();
    double y1 = start.getY();
    double x2 = end.getX();
    double y2 = end.getY();

    LineGenEquation equation{0,0,0};
    equation.A = y2 - y1;
    equation.B = x1 - x2;
    equation.C = x2 * y1 - x1 * y2;

    equation.displayLineGenEquation();
    return equation;
}

LineEquation changeToLineDirEquation(Line line){
        double a = (line.getEndPoint().getY() - line.getStartPoint().getY()) / (line.getEndPoint().getX() - line.getStartPoint().getX());
        double b = line.getStartPoint().getY() - a * line.getStartPoint().getX();
        return LineEquation{a, b};
}

LineGenEquation changeToLineGenEquation(Line line){
    Point start = line.getStartPoint();
    Point end = line.getEndPoint();

    double x1 = start.getX();
    double y1 = start.getY();
    double x2 = end.getX();
    double y2 = end.getY();

    double A = y2 - y1;
    double B = x1 - x2;
    double C = x2 * y1 - x1 * y2;

    return LineGenEquation{A,B,C};
}

bool ifPointOnLine (Point point, LineEquation equation){ //sprawdzenie przynaleznosci punktu do prostej
    double expectedY = equation.a * point.getX() + equation.b;
    double tolerance = 1e-9; // tolerancja dla bledow zmiennoprzecinkowych

        if(abs(point.getY() - expectedY) < tolerance){
            cout << "Punkt (" << point.getX() << ", " << point.getY() << ") znajduje się na prostej ";
            equation.displayLineEquation();
            return true;
        }
    cout << "Punkt (" << point.getX() << ", " << point.getY() << ") nie znajduje się na prostej ";
    equation.displayLineEquation();
    return false;
}

bool ifPointOnSegment (Point point, Line line){ //sprawdzenie przynaleznosci punktu do linii (odcinka)
    LineEquation equation = changeToLineDirEquation(line);
    double expectedY = equation.a * point.getX() + equation.b;
    double tolerance = 1e-9; // tolerancja dla bledow zmiennoprzecinkowych

    if(abs(point.getY() - expectedY) < tolerance){ //jest na prostej
        if(point.getX() >= min(line.getStartPoint().getX(),line.getEndPoint().getX()) &&
           point.getX() <= max(line.getStartPoint().getX(),line.getEndPoint().getX()) &&
           point.getY() >= min(line.getStartPoint().getY(),line.getEndPoint().getY()) &&
           point.getY() <= max(line.getStartPoint().getY(),line.getEndPoint().getY())){
                cout << "Punkt (" << point.getX()<< ", " << point.getY() << ") znajduje się na odcinku, ktory ma";
                line.displayLine();
                cout << " i znajduje sie na prostej: ";
                equation.displayLineEquation();
                cout << endl;
                return true;
        }

        cout << "Punkt (" << point.getX()<< ", " << point.getY() << ") nie znajduje się na odcinku, ktory ma ";
        line.displayLine();
        cout << " ale znajduje sie na prostej: ";
        equation.displayLineEquation();
        cout << endl;
        return false;
    } else {
        cout << "Punkt (" << point.getX() << ", " << point.getY() << ") nie znajduje się na odcinku, ktory przechodzi przez";
        line.displayLine();
        return false;
    }
}

void ifLeftOrRight(Point point, LineEquation equation){ //okreslenie polozenia punktu wzgledem prostej (prawo/lewo)
    LineGenEquation genEquation = convertToLineGenEquation(equation); //postac ogolna
    double result = genEquation.A * point.getX() + genEquation.B * point.getY() + genEquation.C;
    cout << "Punkt ";
    point.displayPoint();

    if(result < 0){
        cout << " jest po lewej stronie prostej: ";
    } else if (result > 0){
        cout << " jest po prawej stronie prostej: ";
    } else {
        cout << " lezy na prostej: ";
    }
    genEquation.displayLineGenEquation();
}

Point translation(Point point, Point vector){ //translacja punktu
    double x = point.getX() + vector.getX();
    double y = point.getY() + vector.getY();
    return Point{x,y};
}

Line lineTranslation(Line line, Point vector){ //dokonanie translacji linii o podany wektor
    Point start = translation(line.getStartPoint(),vector);
    Point end = translation(line.getEndPoint(),vector);

    cout << "Punkty poczatkowe: (" << line.getStartPoint().getX() << ", " << line.getStartPoint().getY() << "), ";
    cout << "(" << line.getEndPoint().getX() << ", " << line.getEndPoint().getY() << ")";
    cout << " po dokonaniu translacji: (" << start.getX() << ", " << start.getY() << "), (" << end.getX() << ", " << end.getY() << ")" << endl;

    return Line{start,end};
}

Point reflection(Point point, Line line){ //dokonanie odbicia danego punktu wzgledem linii

    LineEquation mainLine = changeToLineDirEquation(line);
    if(ifPointOnLine(point, mainLine)){
        cout << "Punkt (" << point.getX() << ", " << point.getY() << ") lezy na prostej" << endl;
        return point;
    }

    LineEquation orthogonal(0,0); // prosta prostopadla
    Point cutPoint(0,0);
    if(mainLine.a != 0 ){
        orthogonal.a = -1 / mainLine.a;
        orthogonal.b = point.getY() - orthogonal.a * point.getX(); //b = y - ax
        cutPoint.setX((mainLine.b - orthogonal.b) / (orthogonal.a - mainLine.a)); // a1x +b1 = a2x+b2 -> x = (b2-b1)/(a1-b2)
        cutPoint.setY(mainLine.a * cutPoint.getX() + mainLine.b); // y=a1x + b1
    } else {
        orthogonal.a = 0;
        orthogonal.b = point.getX();
        cutPoint.setX(mainLine.b);
        cutPoint.setY(point.getY());
    }

    double distanceX = cutPoint.getX() - point.getX();
    double distanceY = cutPoint.getY() - point.getY();

    double x = cutPoint.getX() + distanceX;
    double y = cutPoint.getY() + distanceY;

    return {x,y};
}

void drawPoint(sf::RenderWindow& window, Point pointPosition) {
    float pointRadius = 5; //promien
    sf::CircleShape point(pointRadius);
    point.setFillColor(sf::Color::Magenta);
    point.setPosition(pointToVector(pointPosition));
    point.setOrigin(pointRadius, pointRadius);
    window.draw(point);
}

void drawSegment(sf::RenderWindow& window, Point startPoint, Point endPoint){
    sf::Color color = sf::Color::Black;
    sf::Vertex line[] = {
            sf::Vertex(pointToVector(startPoint), color),
            sf::Vertex(pointToVector(endPoint), color)
    };
    window.draw(line, 2, sf::Lines);
}

void drawLine(sf::RenderWindow& window, double a, double b){
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = sf::Vector2f(0, a * 0 + b); // poczatek ukladu wspolrzednych
    line[1].position = sf::Vector2f(800, a * 800 + b); // 800 - tyle ma szerokosc okna
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;

    window.draw(line);
}

int main() {
    //podpunkt1
    Point point1(100, 200);
    Point point2(200, 300);
    Line line1(point1, point2);
    findLineEquation(line1);

    //podpunkt2
    Point point3(200, 250);
    LineEquation equation(5, 1);
    ifPointOnLine(point3,equation);

    //podpunkt3
    ifPointOnSegment(point3,line1);

    //podpunkt4
    ifLeftOrRight(point2, equation);

    //podpunkt5
    Line translation = lineTranslation(line1,point1);
    Point valueX = translation.getStartPoint();
    Point valueY = translation.getEndPoint();

    //podpunkt6
    Point point4 = reflection(point3,line1);
    point4.displayPoint();

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    while (window.isOpen()) {
        sf::Event event; //obsluga zdarzen
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);

        //wyznaczeine rownania prostej do ktorej nalezy dana linia (odcinek) y=x+100 p1(100,200), p2(200,300)
        drawSegment(window, point1, point2);
        drawLine(window,1,100);

       //sprawdzenie przynaleznosci punktu do prostej p3(200,250) y=5x+1
        drawPoint(window,point3);
        drawLine(window,5,1);

        //sprawdzenie przynaleznosci punktu do linii (odcinka) p3(200,250) odcinka (100,200),(200,300)
        drawPoint(window,point3);
        drawSegment(window, point1,point2);

       //okreslenie polozenia punktuwzgledem prostej (prawo/lewo) y=5x+1, p2(200,300)
        drawLine(window,5,1);
        drawPoint(window,point2);

        //dokonanie translacji linii o podany wektor p1(100,200) , odcinek z p1 i p2
        drawSegment(window,point1,point2);
        drawSegment(window, valueX, valueY);

        //dokonanie odbicia danego punktu wzgledem linii
        drawSegment(window, point1, point2);
        drawPoint(window, point3);
        drawPoint(window, point4);

        window.display(); //wysiweltenie zawartosci okna
    }
    return 0;
}
