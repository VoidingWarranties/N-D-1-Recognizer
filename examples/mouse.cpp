#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "../src/Point.h"
#include "../src/Path.h"
#include "../src/Recognizer.h"

#include <Carbon/Carbon.h>

using namespace NDRecognizer;

std::atomic_bool flag(true);

void waitForInput() {
  getchar();
  flag = false;
}

Path<2> GetPath() {
  Path<2> path;
  while (flag) {
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    Point<2> p;
    p[0] = cursor.x;
    p[1] = cursor.y;
    path.addPoint(p);
    CFRelease(event);
    // Sleep so that we don't add a bajillion points to the path.
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  flag = true;
  return path;
}

enum class Shape {
  Circle,
  Square,
  Triangle,
  X
};

int main() {
  Point<2> origin;
  origin[0] = origin[1] = 0;

  std::cout << "Draw a circle" << std::endl;
  std::thread waitForCircle(waitForInput);
  Path<2> circle_template = GetPath();
  waitForCircle.join();

  std::cout << "Draw a square" << std::endl;
  std::thread waitForSquare(waitForInput);
  Path<2> square_template = GetPath();
  waitForSquare.join();

  std::cout << "Draw a triangle" << std::endl;
  std::thread waitForTriangle(waitForInput);
  Path<2> triangle_template = GetPath();
  waitForTriangle.join();

  std::cout << "Draw an X" << std::endl;
  std::thread waitForX(waitForInput);
  Path<2> x_template = GetPath();
  waitForX.join();

  Recognizer<2, Shape> r(1000, 100);
  r.addTemplate(Shape::Circle, circle_template);
  r.addTemplate(Shape::Square, square_template);
  r.addTemplate(Shape::Triangle, triangle_template);
  r.addTemplate(Shape::X, x_template);

  while (true) {
    std::cout << "Draw something" << std::endl;
    std::thread waitForSomething(waitForInput);
    Path<2> something = GetPath();
    waitForSomething.join();

    float score;
    Shape recognized_template = r.recognizeMinDistance(something, score);
    switch (recognized_template) {
      case Shape::Circle:
        std::cout << "Circle" << std::endl;
        break;
      case Shape::Square:
        std::cout << "Square" << std::endl;
        break;
      case Shape::Triangle:
        std::cout << "Triangle" << std::endl;
        break;
      case Shape::X:
        std::cout << "X" << std::endl;
        break;
      default:
        std::cout << "Unknown shape" << std::endl;
        break;
    }
    std::cout << score << std::endl;
  }

  return 0;
}
