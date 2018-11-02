#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <stdlib.h>

class Point2D
{
    int x, y;
    public:
        Point2D();
        Point2D(int x, int y);
        ~Point2D();
        int getX();
        int getY();
        void setRandomXY(int minX, int maxX, int minY, int maxY);
        void setRandomXY();
        void setRandomX(int minX, int maxX);
        void setRandomY(int minY, int maxY);
};

#endif // POINT2D_HPP