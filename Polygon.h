#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
using namespace std;

struct Point{
    double x;
    double y;
    Point(double _x,double _y):x(_x),y(_y){}
    Point():x(0.0),y(0.0){}
};

class Polygon
{
public:
    Polygon(const vector<Point> &_points):polygon(_points){}
    inline void addPoint(const Point &point){polygon.push_back(point);}
    bool findLargestRetangle(Point &center, double &resangle, vector<Point> &rect,double &resWidth,double &resHeight);
private:
    vector<Point> polygon;
};

#endif