#include <bits/stdc++.h>

using namespace std;

// wczytac dane
// sort po x
// dziel i zwyciezaj
// zabawa w podzbiorach
// koncowy merge
struct Point {
  int x;
  int y;

  Point(int given_x, int given_y) {
    x = given_x;
    y = given_y;
  }
};

vector<Point> X_points;
vector<Point> Y_points;

double min_perimeter = INT_MAX;
vector<Point> res(3);

struct Comparator {
  char order;
  Comparator(char indicator) { order = indicator; }

  bool operator()(const Point &p1, const Point &p2) {
    switch (order) {
    case 'x':
      if (p1.x != p2.x)
        return p1.x < p2.x;
      return p1.y < p2.y;

    case 'y': // just in case
      if (p1.y != p2.y)
        return p1.y < p2.y;
      return p1.x < p2.x;
    }
    return false;
  }
};

double distance(Point p1, Point p2) { // dystans
  return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// void main_logic(vector<Point> X_points, vector<Point> Y_points, int start,
// int end) { // more to do // potenjalnie do merge
void main_logic(int start, int end) { // more to do
  int s = end - start + 1;
  if (s < 6) {
    double sum;
    switch (s) {
    case 3:
      sum = distance(X_points[start], X_points[start + 1]) +
            distance(X_points[start + 1], X_points[end]) +
            distance(X_points[start], X_points[end]);
      if (sum < min_perimeter) {
        min_perimeter = sum;
        for (int i = 0; i < 3; i++) {
          res[i] = X_points[start + i];
        }
      }
      break;
    case 4:
    case 5:
      for (int i = start; i <= s; i++) {
        for (int j = i + 1; j <= s; j++) {
          for (int k = j + 1; k <= s; k++) {
            sum = distance(X_points[i], X_points[j]) +
                  distance(X_points[j], X_points[k]) +
                  distance(X_points[k], X_points[i]);
            if (sum < min_perimeter) {
              min_perimeter = sum;
              res[0] = X_points[i];
              res[1] = X_points[j];
              res[2] = X_points[k];
            }
          }
        }
      }
      break;
    }
    // vector<Point> tmp_Y; // potencjalnie do merge
    // for(int i = start; i <= end - start + 1; i++) {
    //     tmp_Y.push_back(X_points[i]);
    // }

    return;
  }
  // main_logic(X_points, Y_points, start, (start + end) / 2); // potencjalnie
  // do merge main_logic(X_points, Y_points, (start + end) / 2 + 1, end);
  main_logic(start, (start + end) / 2);
  main_logic((start + end) / 2 + 1, end);

  // vector<Point> Y_tmp(X_points.begin() + start, X_points.begin() + end + 1);
  // // elementy w naszym przedziale

  int Mid = (start + end) / 2;
  double Min = min_perimeter / 2;
  vector<Point> sub_X;

  for (int i = start; i <= end; i++) {
    if (abs(X_points[i].x - X_points[Mid].x) <
        Min) { // ewentualnie patrzec cały obowd nie pol
      sub_X.push_back(X_points[i]);
    }
  }

  sort(sub_X.begin(), sub_X.end(), Comparator('y')); // sortujemy po y
  int size = sub_X.size();
  double sum;

  for (int i = 0; i < size; i++) {

    for (int j = i + 1; j < size && abs(sub_X[i].y - sub_X[j].y) <= Min; j++) {

      for (int k = j + 1; k < size && abs(sub_X[i].y - sub_X[k].y) <= Min;
           k++) {

        sum = distance(X_points[i], X_points[j]) +
              distance(X_points[j], X_points[k]) +
              distance(X_points[k], X_points[i]);
        if (sum < min_perimeter) {
          min_perimeter = sum;
          res[0] = X_points[i];
          res[1] = X_points[j];
          res[2] = X_points[k];
        }
      }
    }
  }

  // tu musi wejsc merge
  // logic
}

int main() {
  // ios_base::sync_with_stdio(false);
  // cout.tie(0);
  // cin.tie(0);

  int n;
  cin >> n;
  // vector<Point> X_points; // go to posortowane po x

  int new_x;
  int new_y;
  for (int i = 0; i < n; i++) { // wczytujemy punkty ;>
    cin >> new_x >> new_y;
    Point Point(new_x, new_y);
    X_points.push_back(Point);
  }

  sort(X_points.begin(), X_points.end(), Comparator('x')); // sortujemy po x

  // double min_perimeter; // circut hehe
  // min_perimeter = distance(X_points[0], X_points[1]) + distance(X_points[1],
  // X_points[2]) + distance(X_points[2], X_points[0]); // jakaś wartość min
  // obwodu res[0] = X_points[0]; res[1] = X_points[1]; res[2] = X_points[2];
  //

  // vector<Point> Y_points(X_points);
  // sort(Y_points.begin(), Y_points.end(), Comparator('y')); // sortujemy po y

  for (int i = 0; i < 3; i++) {
    cout << res[i].x << " " << res[i].y << "\n";
  }
}