#include <iostream>
#include <fstream>
using namespace std;

struct Point {
    double x, y;
};

double lagrangeInterpolation(Point* points, int n, double x) {
    double L = 0.0;

    for (int i = 0; i < n; ++i) { //przechodzimy przez wszystkie punkty
        double y = points[i].y;
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                y *= (x - points[j].x) / (points[i].x - points[j].x); //y1 razy l1
            }
        }
        L += y;
    }
    return L;
}

int main() {

    ifstream file("/Users/gosiapalewicz/Documents/MetodyNumeryczne/file.txt");
    if (!file.is_open()) {
        cout << "blad otwierania pliku!" << endl;
        return 1;
    }

    int n = 0; //liczik linijek
    Point tmp;
    while (file >> tmp.x >> tmp.y) {
        n++;
    }
    file.close();

    Point* points = new Point[n];

    file.open("/Users/gosiapalewicz/Documents/MetodyNumeryczne/file.txt"); //wczytywannie danych
    for (int i = 0; i < n; ++i) {
        file >> points[i].x >> points[i].y;
    }
    file.close();

    double x;
    cout << "podaj x dla ktorego chcesz obliczyc interpolacje: ";
    cin >> x;

    double interpolatedValue = lagrangeInterpolation(points, n, x);
    cout << "wartosc dla x = " << x << " wynosi: " << interpolatedValue << endl;

    delete[] points;

    return 0;
}