//
// Created by qiguanjie on 2017/4/22.
//

#ifndef POLYGON_UTIL_H
#define POLYGON_UTIL_H

#include "Polygon.h"

#define FLOAT_JUDGE_DIFFERENCE 0.000001
#define SAFE_DELETE(p) if(p){delete p;p=NULL;}

class util
{
public:

    static Point centroid(const vector<Point> &polygon);

    static void getPolyBounds(const vector<Point> &poly, double &minx, double &miny, double &maxx,
                              double &maxy);

    static Point rotatePoint(const Point &pt, const Point &center, const double angle);

    static vector<Point> rotatePoly(vector<Point> &rectPoly, const Point &origin, double angle);

    static bool rayIntersectsSegment(const Point &p, const Point &p1, const Point &p2);

    static bool pointInPoly(const Point &pt, const vector<Point> &poly);

    static bool segmentsIntersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

    static bool pointInSegmentBox(const Point &p, const Point &p1, const Point &q1);

    static bool pointInSegment(const Point &p, const Point &p1, const Point &q1);

    static Point *lineIntersection(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

    static bool polyInsidePoly(const vector<Point> &polyA, const vector<Point> &polyB);

    static void intersectPoints(const vector<Point> &poly, const Point &origin, double alpha, Point *&closestPointLeft,
                                Point *&closestPointRight);

    static double squaredDist(const Point &a, const Point &b);

};


#endif //POLYGON_UTIL_H
