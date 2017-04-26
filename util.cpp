//
// Created by qiguanjie on 2017/4/22.
//

#include <cmath>
#include "util.h"

Point util::centroid(const vector<Point> &polygon)
{
    double totalArea = 0.0;
    double totalX = 0.0;
    double totalY = 0.0;
    int i = 0;
    int pointCount = polygon.size();
    for (i = 0; i < pointCount; i++)
    {
        const Point &a = polygon[i];
        const Point &b = polygon[(i + 1) % pointCount];
        double area = 0.5 * (a.x * b.y - b.x * a.y);
        double x = (a.x + b.x) / 3.0;
        double y = (a.y + b.y) / 3.0;
        totalArea += area;
        totalX += area * x;
        totalY += area * y;
    }
    Point centroid(totalX / totalArea, totalY / totalArea);
    return centroid;
}

void util::getPolyBounds(const vector<Point> &poly, double &minx, double &miny, double &maxx,
                         double &maxy)
{
    for (size_t i = 0; i < poly.size(); i++)
    {
        if (i == 0)
        {
            minx = maxx = poly[i].x;
            miny = maxy = poly[i].y;
        }
        else
        {
            if (minx > poly[i].x)
            {
                minx = poly[i].x;
            }
            if (maxx < poly[i].x)
            {
                maxx = poly[i].x;
            }
            if (miny > poly[i].y)
            {
                miny = poly[i].y;
            }
            if (maxy < poly[i].y)
            {
                maxy = poly[i].y;
            }
        }
    }
}

Point util::rotatePoint(const Point &pt, const Point &center, const double angle)
{
    Point afterRotate;
    afterRotate.x = (pt.x - center.x) * cos(angle) - (pt.y - center.y) * sin(angle) + center.x;
    afterRotate.y = (pt.x - center.x) * sin(angle) + (pt.y - center.y) * cos(angle) + center.y;
    return afterRotate;
}

vector<Point> util::rotatePoly(vector<Point> &rectPoly, const Point &origin, double angle)
{
    vector<Point> result;
    for (int i = 0; i < rectPoly.size(); i++)
    {
        result.push_back(rotatePoint(rectPoly[i], origin, angle));
    }
    return result;
}

bool util::rayIntersectsSegment(const Point &p, const Point &p1, const Point &p2)
{
    Point a;
    Point b;
    Point pt(p.x, p.y);
    //double eps = 1e-9;
    if (p1.y < p2.y)
    {
        a = p1;
        b = p2;
    }
    else
    {
        a = p2;
        b = p1;
    }

    if (pt.y == b.y || pt.y == a.y)
    {
        pt.y += FLOAT_JUDGE_DIFFERENCE;
    }

    if (pt.y > b.y || pt.y < a.y || (pt.x > a.x && pt.x > b.x))
    {
        return false;
    }
    else if (pt.x < a.x && pt.x < b.x)
    {
        return true;
    }
    else
    {
        double mAB = (b.y - a.y) / (b.x - a.x);
        double mAP = (pt.y - a.y) / (pt.x - a.x);
        return mAP >= mAB;
    }
}

bool util::pointInPoly(const Point &pt, const vector<Point> &poly)
{
    /*int i = -1;
    int n = poly.size();
    Point b = poly[n - 1];
    int c = 0;
    while (++i < n)
    {
        Point a = b;
        b = poly[i];
        if (rayIntersectsSegment(pt, a, b))
        {
            c++;
        }
    }
    return c % 2 != 0;*/
    int i = 0, j = poly.size() - 1;
    bool oddNodes = false;

    for (i = 0; i < poly.size(); i++)
    {
        if ((poly[i].y < pt.y  && poly[j].y  >= pt.y ||
             poly[j].y < pt.y  && poly[i].y  >= pt.y)
            && (poly[i].x <= pt.x  || poly[j].x <= pt.x ))
        {
            if (poly[i].x + (pt.y - poly[i].y) / (poly[j].y - poly[i].y) * (poly[j].x - poly[i].x) <
                pt.x )
            {
                oddNodes = !oddNodes;
            }
        }
        j = i;
    }

    return oddNodes;
}

bool util::segmentsIntersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2)
{
    Point *p = lineIntersection(p1, q1, p2, q2);
    if (p == NULL)
        return false;
    bool ret = (pointInSegmentBox(*p, p1, q1) && pointInSegmentBox(*p, p2, q2));
    SAFE_DELETE(p);
    return ret;

}

bool util::pointInSegmentBox(const Point &p, const Point &p1, const Point &q1)
{
    // allow for some margins due to numerical errors
    //double eps = 1e-9;
    double minx = p1.x < q1.x ? p1.x : q1.x;
    double miny = p1.y < q1.y ? p1.y : q1.y;
    double maxx = p1.x > q1.x ? p1.x : q1.x;
    double maxy = p1.y > q1.y ? p1.y : q1.y;
    if (p.x < minx - FLOAT_JUDGE_DIFFERENCE || p.x > maxx + FLOAT_JUDGE_DIFFERENCE ||
        p.y < miny - FLOAT_JUDGE_DIFFERENCE || p.y > maxy + FLOAT_JUDGE_DIFFERENCE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool util::pointInSegment(const Point &p, const Point &p1, const Point &q1)
{
    double disTotal = sqrt(squaredDist(p1, q1));
    double dis1 = sqrt(squaredDist(p, p1));
    double dis2 = sqrt(squaredDist(p, q1));
    return pointInSegmentBox(p, q1, p1) && (disTotal - dis1 - dis2) < FLOAT_JUDGE_DIFFERENCE;
}

Point *util::lineIntersection(const Point &p1, const Point &q1, const Point &p2, const Point &q2)
{
    double x1 = p1.x, y1 = p1.y, x2 = q1.x, y2 = q1.y, x3 = p2.x, y3 = p2.y, x4 = q2.x, y4 = q2.y;

    if (fabs(x1 - x2) < FLOAT_JUDGE_DIFFERENCE && fabs(y3 - y4) < FLOAT_JUDGE_DIFFERENCE)
    {
        Point *pt = new Point(x1, y3);
        return pt;
    }
    if (fabs(y1 - y2) < FLOAT_JUDGE_DIFFERENCE && fabs(x3 - x4) < FLOAT_JUDGE_DIFFERENCE)
    {
        Point *pt = new Point(x3, y1);
        return pt;
    }
    double nominator = (y1 - y2) * (x3 - x4) - (x1 - x2) * (y3 - y4);
    if (fabs(nominator) < FLOAT_JUDGE_DIFFERENCE)
    {
        return NULL;
    }
    Point *pt = new Point();
    pt->x = ((x1 - x2) * (x3 * y4 - x4 * y3) - (x3 - x4) * (x1 * y2 - x2 * y1)) / nominator;
    pt->y = ((y1 - y2) * (x3 * y4 - x4 * y3) - (x1 * y2 - x2 * y1) * (y3 - y4)) / nominator;
    return pt;
}

bool util::polyInsidePoly(const vector<Point> &polyA, const vector<Point> &polyB)
{
    int iA = -1;
    int nA = polyA.size();
    int nB = polyB.size();
    Point bA = polyA[nA - 1];
    while (++iA < nA)
    {
        Point aA = bA;
        bA = polyA[iA];
        int iB = -1;
        Point bB = polyB[nB - 1];
        while (++iB < nB)
        {
            Point aB = bB;
            bB = polyB[iB];
            if (segmentsIntersect(aA, bA, aB, bB))
            {
                return false;
            }
        }
    }
    return pointInPoly(polyA[0], polyB);
}

void util::intersectPoints(const vector<Point> &poly, const Point &origin, double alpha, Point *&closestPointLeft,
                           Point *&closestPointRight)
{
    //double eps = 1e-9;
    Point modif;
    modif.x = origin.x + FLOAT_JUDGE_DIFFERENCE * cos(alpha);
    modif.y = origin.y + FLOAT_JUDGE_DIFFERENCE * sin(alpha);

    Point pt0(modif.x, modif.y);
    Point shiftedOrigin(pt0.x + cos(alpha), pt0.y + sin(alpha));

    int idx = 0;
    if (fabs(shiftedOrigin.x - pt0.x) < FLOAT_JUDGE_DIFFERENCE)
        idx = 1;

    int i = -1;
    int n = poly.size();
    Point b = poly[n - 1];
    double minSqDistLeft = INT_MAX;
    double minSqDistRight = INT_MAX;
    while (++i < n)
    {
        Point a = b;
        b = poly[i];
        Point *p = lineIntersection(modif, shiftedOrigin, a, b);
        if (p == NULL)
            continue;

        if (!pointInSegment(*p, a, b))
        {
            SAFE_DELETE(p);
            continue;
        }

        double sqDist = squaredDist(modif, *p);
        if (idx == 0)
        {
            if (p->x < modif.x)
            {
                if (sqDist < minSqDistLeft)
                {
                    minSqDistLeft = sqDist;
                    if (closestPointLeft == NULL)
                        closestPointLeft = new Point(p->x, p->y);
                    else
                    {
                        closestPointLeft->x = p->x;
                        closestPointLeft->y = p->y;
                    }
                }
            }
            else if (p->x > modif.x)
            {
                if (sqDist < minSqDistRight)
                {
                    minSqDistRight = sqDist;
                    if (closestPointRight == NULL)
                    {
                        closestPointRight = new Point(p->x, p->y);
                    }
                    else
                    {
                        closestPointRight->x = p->x;
                        closestPointRight->y = p->y;
                    }
                }
            }
        }
        else//idx==1
        {
            if (p->y < modif.y)
            {
                if (sqDist < minSqDistLeft)
                {
                    minSqDistLeft = sqDist;
                    if (closestPointLeft == NULL)
                        closestPointLeft = new Point(p->x, p->y);
                    else
                    {
                        closestPointLeft->x = p->x;
                        closestPointLeft->y = p->y;
                    }
                }
            }
            else if (p->y  > modif.y)
            {
                if (sqDist < minSqDistRight)
                {
                    minSqDistRight = sqDist;
                    if (closestPointRight == NULL)
                    {
                        closestPointRight = new Point(p->x, p->y);
                    }
                    else
                    {
                        closestPointRight->x = p->x;
                        closestPointRight->y = p->y;
                    }
                }
            }
        }//endif idx=1
        SAFE_DELETE(p);
    }//endwhile
}

double util::squaredDist(const Point &a, const Point &b)
{
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}