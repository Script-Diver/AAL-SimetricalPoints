#include "Point2D.hpp"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <climits>

Point2D::Point2D() : x(0), y(0)
{ }

Point2D::Point2D(int coordinateX, int coordinateY) : x(coordinateX), y(coordinateY)
{ }

Point2D::~Point2D()
{ }

void Point2D::setRandomX(int minX, int maxX) {
    x = (int) rand() % (maxX - minX + 1) + minX;
}

void Point2D::setRandomY(int minY, int maxY) {
    y = (int) rand() % (maxY - minY + 1) + minY;
}

void Point2D::setRandomXY(int minX, int maxX, int minY, int maxY) {
    setRandomX(minX, maxX);
    setRandomY(minY, maxY);
}

void Point2D::setRandomXY() {
    setRandomXY(0, INT_MAX, 0, INT_MAX);
}

int Point2D::getX() {
    return x;
}

int Point2D::getY() {
    return y;
}