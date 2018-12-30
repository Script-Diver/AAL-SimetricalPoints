#include <iostream>
#include "MyAlgorithm.hpp"

MyAlgorithm::MyAlgorithm() {}
MyAlgorithm::~MyAlgorithm(){}

void MyAlgorithm::checkCases(std::vector<Point2D> &result, std::vector<Point2D> &finalPoints, int cost1, int cost2, int cost3, int cost4) {
    int combination[] = {0, 3, 12, 15, 48, 51, 60, 63, 192, 195, 204, 207, 240, 243, 252};
    int reflectedPointsNum = -1;
    int minPerimeret = -1;
    int combinationLength = sizeof(combination) / sizeof(combination[1]);
    for (int i = 0 ; i < combinationLength ; i++) {
        int cost = countCosts(combination[i], cost1, cost2, cost3, cost4);
        updateMinRectangle(result, minPerimeret, reflectedPointsNum, combination[i], finalPoints, cost);
    }
    std::cout << "minPerimeret = " << minPerimeret << std::endl;
    std::cout << "reflectedPointsNum = " << reflectedPointsNum << std::endl << std::endl;
}

int MyAlgorithm::countCosts(int combination, int cost1, int cost2, int cost3, int cost4) {
    int cost = 0;
    //std::cout << combination << " -> ";
    if((combination & 3) != 0) { cost += cost1;}// std::cout << "+" << cost1; }
    if((combination & 12) != 0) { cost += cost2;}// std::cout << "+" << cost2; }
    if((combination & 48) != 0) { cost += cost3;}// std::cout << "+" << cost3; }
    if((combination & 192) != 0) { cost += cost4;}// std::cout << "+" << cost4; }
   // std::cout << std::endl;
    return cost;
}

void MyAlgorithm::dividePointsIntoUpperAndBottomGroup(std::vector<Point2D> points, std::vector<Point2D> &upG, std::vector<Point2D> &botG) {
    for(unsigned i = 0 ; i < points.size() ; i++){
        if(points[i].getX() < points[i].getY()) upG.push_back(points[i]);
        else botG.push_back(points[i]);
    }
}

std::vector<Point2D> MyAlgorithm::findMinRectangle(std::vector<Point2D> &rectanglesSurroundingConvexHulls, int cost1, int cost2, int cost3, int cost4) {
    std::vector<Point2D> result;
    checkCases(result, rectanglesSurroundingConvexHulls, cost1, cost2, cost3, cost4);
    return result;
}

std::vector<Point2D> MyAlgorithm::getMinRectangle(std::vector<Point2D> points) {
    if (points.size() == 0 ) return std::vector<Point2D>();
    std::vector<Point2D> upperG, bottomG, upperRec, bottomRec, result;

    dividePointsIntoUpperAndBottomGroup(points, upperG, bottomG);       //1

    upperRec = findExtremePointsOfRectangle(upperG);                    //2
    bottomRec = findExtremePointsOfRectangle(bottomG);
/*
    std::cout << "upperG:\n";
    showVector(upperG);
    std::cout << "bottomG:\n";
    showVector(bottomG);
*/
    reflectRectangle(upperRec, std::vector<Point2D>());                 //3

    std::vector<Point2D> intersectionArea, intersUpprG, intersBottG;    //4

    intersectionArea = getIntersectionArea(upperRec, bottomRec);        //5
    int intersectionAreaNumberOfPoints = intersectionArea.size();
    intersBottG = separateIntersectionPointsFromRectangles(bottomG, intersectionArea);
    for (int i = 0 ; i < intersectionAreaNumberOfPoints ; i++) intersBottG.pop_back();

    intersectionArea = getIntersectionArea(upperRec, bottomRec);
    reflectRectangle(upperRec, std::vector<Point2D>());
    reflectRectangle(intersectionArea, std::vector<Point2D>());
    intersUpprG = separateIntersectionPointsFromRectangles(upperG, intersectionArea);
    for (int i = 0 ; i < intersectionAreaNumberOfPoints ; i++) intersUpprG.pop_back();

    std::vector<Point2D> intersUpperRec, intersBottomRec;
    upperRec = findExtremePointsOfRectangle(upperG);                    //6
    bottomRec = findExtremePointsOfRectangle(bottomG);
    intersUpperRec = findExtremePointsOfRectangle(intersUpprG);
    intersBottomRec = findExtremePointsOfRectangle(intersBottG);        //7

    if(upperRec.size() == 0) { upperRec.push_back(points[0]); upperRec.push_back(points[0]); }
    if(bottomRec.size() == 0) { bottomRec.push_back(points[0]); bottomRec.push_back(points[0]); }
    if(intersUpperRec.size() == 0) { intersUpperRec.push_back(points[0]); intersUpperRec.push_back(points[0]); }
    if(intersBottomRec.size() == 0) { intersBottomRec.push_back(points[0]); intersBottomRec.push_back(points[0]); }
/*
    std::cout << "upperRec:\n";
    showVector(upperRec);
    std::cout << "upperG.size() = " << upperG.size() << std::endl << std::endl;

    std::cout << "bottomRec:\n";
    showVector(bottomRec);
    std::cout << "bottomG.size() = " << bottomG.size() << std::endl << std::endl;

    std::cout << "intersUpperRec:\n";
    showVector(intersUpperRec);
    std::cout << "intersUpprG.size() = " << intersUpprG.size() << std::endl << std::endl;

    std::cout << "intersBottomRec:\n";
    showVector(intersBottomRec);
    std::cout << "intersBottG.size() = " << intersBottG.size() << std::endl << std::endl;
*/
    std::vector<Point2D> rectanglesSurroundingConvexHulls = mergeVectors(upperRec, bottomRec, intersUpperRec, intersBottomRec);     //8
    /*
    std::cout << "rectanglesSurroundingConvexHulls:\n";
    showVector(rectanglesSurroundingConvexHulls);
    std::cout << "rectanglesSurroundingConvexHulls.size() = " << rectanglesSurroundingConvexHulls.size() << std::endl << std::endl;
*/
    result = findMinRectangle(rectanglesSurroundingConvexHulls, upperG.size(), bottomG.size(), intersUpprG.size(), intersBottG.size());
    result = findExtremePointsOfRectangle(result);

    return result;
}

bool MyAlgorithm::isInArea(Point2D point, std::vector<Point2D> area) {
    std::vector<Point2D> vec;
    vec.push_back(point);
    return getIntersectionArea(vec, area).size() > 0;
}

std::vector<Point2D> MyAlgorithm::mergeVectors(std::vector<Point2D> &r1, std::vector<Point2D> &r2, std::vector<Point2D> &r3, std::vector<Point2D> &r4) {
    std::vector<Point2D> result = sumVectors(r1, r2);
    result = sumVectors(result, r3);
    result = sumVectors(result, r4);
    return result;
}

std::vector<Point2D> MyAlgorithm::separateIntersectionPointsFromRectangles(std::vector<Point2D> &rec1, std::vector<Point2D> &rec2){
    std::vector<Point2D> intersectionArea, rec1Tmp, rec2Tmp, inP;
    intersectionArea = getIntersectionArea(rec1, rec2);
    if(intersectionArea.size() > 0) {
        for(unsigned i = 0 ; i < rec1.size() ; i++) {
            if(isInArea(rec1[i], intersectionArea)) inP.push_back(rec1[i]);
            else rec1Tmp.push_back(rec1[i]);
        }
        for(unsigned i = 0 ; i < rec2.size() ; i++) {
            if(isInArea(rec2[i], intersectionArea)) inP.push_back(rec2[i]);
            else rec2Tmp.push_back(rec2[i]);
        }
        rec1.clear();
        rec2.clear();
        rec1 = rec1Tmp;
        rec2 = rec2Tmp;
    }
    return inP;
}

void MyAlgorithm::showVector(std::vector<Point2D> vec) {
    for(unsigned i = 0 ; i < vec.size() ; i++) {
        std::cout << "(" << vec[i].getX() << " , " << vec[i].getY() << ")" << std::endl;
    }
}

std::vector<Point2D> MyAlgorithm::sumVectors (std::vector<Point2D> &vec1, std::vector<Point2D> &vec2) {
    std::vector<Point2D> result = vec1;
    for(unsigned i = 0 ; i < vec2.size() ; i++) result.push_back(vec2[i]);
    return result;
}

void MyAlgorithm::reflectRectangle(std::vector<Point2D> &rectangle, std::vector<Point2D> excludedArea) {
    if(rectangle.size() <= 0) return;
    excludedArea = findExtremePointsOfRectangle(excludedArea);
    for(unsigned i = 0 ; i < rectangle.size() ; i++) {
        if(excludedArea.size() < 1 || !isInArea(rectangle[i], excludedArea))
            rectangle[i].reflectOverYEqualsX();
    }
}