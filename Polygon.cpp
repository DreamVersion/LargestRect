//
// Created by qiguanjie on 2017/4/22.
//

#include <cstdlib>
#include <math.h>
#include "Polygon.h"
#include "util.h"

bool Polygon::findLargestRetangle(Point &center, double &resAngle, vector<Point> &maxRect, double &resWidth,
                                  double &resHeight)
{
    //https://d3plus.org/blog/behind-the-scenes/2014/07/08/largest-rect/
    if (polygon.size() < 3)
    {
        return false;
    }
    double src_minx = 0.0, src_miny = 0.0, src_maxx = 0.0, src_maxy = 0.0;
    util::getPolyBounds(polygon, src_minx, src_miny, src_maxx, src_maxy);

    for (int i = 0; i < polygon.size(); i++)
    {
        polygon[i].x -= src_minx;
        polygon[i].y -= src_miny;
    }
    double minx = 0.0, miny = 0.0, maxx = 0.0, maxy = 0.0;
    util::getPolyBounds(polygon, minx, miny, maxx, maxy);
    double boxWidth = maxx - minx;
    double boxHeight = maxy - miny;

    //const double widthStep = ConvertCoord(1.0, m_nlevel);//a font size as step
    const double widthStep = 1.0;
    const int minFontSize = 5;//最小字号
//	double minWidth = minFontSize * widthStep * aspectRatio;
//	double minHeight = minFontSize * widthStep;
    const double minWidth = minFontSize * widthStep;
    const double minHeight = minFontSize * widthStep;
    const double maxAspectRatio = 15;
    const double aspectRatioStep = 0.5;
    const int tryTimes = 20;
    const int angleStep = 5;
    if (boxWidth < minWidth || boxHeight < minHeight)
    {
        return false;
    }
    vector<Point> origins;
    Point centroid = util::centroid(polygon);
    if (util::pointInPoly(centroid, polygon))
    {
        origins.push_back(centroid);
    }

    srand(time(NULL));
    while (origins.size() < tryTimes)
    {
        Point randOrigin;
        randOrigin.x = ((double) rand() / RAND_MAX) * boxWidth + minx;
        randOrigin.y = ((double) rand() / RAND_MAX) * boxHeight + miny;
        if (util::pointInPoly(randOrigin, polygon))
        {
            origins.push_back(randOrigin);
        }
    }

    double maxArea = 0.0;

    for (int angle = -90; angle < 90 + angleStep; angle += angleStep)
    {
        double angleRad = -angle * M_PI / 180;
        for (int i = 0; i < origins.size(); i++)
        {
            vector<Point> modifOrigins;
            Point &origOrigin = origins[i];
            Point *newP1W = NULL, *p2W = NULL;
            util::intersectPoints(polygon, origOrigin, angleRad, newP1W, p2W);
            if (newP1W != NULL && p2W != NULL)
            {
                modifOrigins.push_back(Point((newP1W->x + p2W->x) / 2.0, (newP1W->y + p2W->y) / 2.0));
            }
            SAFE_DELETE(newP1W);
            SAFE_DELETE(p2W);

            Point *p1H = NULL, *p2H = NULL;
            util::intersectPoints(polygon, origOrigin, angleRad + M_PI / 2, p1H, p2H);
            if (p1H != NULL && p2H != NULL)
            {
                modifOrigins.push_back(Point((p1H->x + p2H->x) / 2.0, (p1H->y + p2H->y) / 2.0));
            }
            SAFE_DELETE(p1H);
            SAFE_DELETE(p2H);

            for (int j = 0; j < modifOrigins.size(); j++)
            {
                Point &origin = modifOrigins[j];
                Point *newP1W = NULL, *newP2W = NULL;
                util::intersectPoints(polygon, origin, angleRad, newP1W, newP2W);
                if (newP1W == NULL || newP2W == NULL)
                {
                    SAFE_DELETE(newP1W);
                    SAFE_DELETE(newP2W);
                    continue;
                }

                Point *newP1H = NULL, *newP2H = NULL;
                util::intersectPoints(polygon, origin, angleRad + M_PI / 2, newP1H, newP2H);
                if (newP1H == NULL || newP2H == NULL)
                {
                    SAFE_DELETE(newP1H);
                    SAFE_DELETE(newP2H);
                    SAFE_DELETE(newP1W);
                    SAFE_DELETE(newP2W);
                    continue;
                }

                double minx1 = util::squaredDist(origin, *newP1W);
                double minx2 = util::squaredDist(origin, *newP2W);
                double minSqDistW = minx1 < minx2 ? minx1 : minx2;
                double maxWidth = 2 * sqrt(minSqDistW);

                double miny1 = util::squaredDist(origin, *newP1H);
                double miny2 = util::squaredDist(origin, *newP2H);
                double minSqDistH = miny1 < miny2 ? miny1 : miny2;
                double maxHeight = 2 * sqrt(minSqDistH);

                SAFE_DELETE(newP1H);
                SAFE_DELETE(newP2H);
                SAFE_DELETE(newP1W);
                SAFE_DELETE(newP2W);

                if (maxWidth * maxHeight <= maxArea)
                {
                    continue;
                }
                double minAspectRatio = 1 > minWidth / maxHeight ? 1 : minWidth / maxHeight;
                minAspectRatio = minAspectRatio > maxArea / (maxHeight * maxHeight) ? minAspectRatio : maxArea /
                                                                                                       (maxHeight *
                                                                                                        maxHeight);
                double maxAspectRatio1 = maxAspectRatio < maxWidth / minHeight ? maxAspectRatio : maxWidth / minHeight;
                maxAspectRatio1 =
                        maxAspectRatio1 < (maxWidth * maxWidth) / maxArea ? maxAspectRatio1 : (maxWidth * maxWidth) /
                                                                                              maxArea;
                for (double ratio = minAspectRatio; ratio < maxAspectRatio1 + aspectRatioStep; ratio += aspectRatioStep)
                {
                    double left = minWidth > sqrt(maxArea * ratio) ? minWidth : sqrt(maxArea * ratio);
                    double right = maxWidth < maxHeight * ratio ? maxWidth : maxHeight * ratio;
                    if (right * maxHeight < maxArea)
                        continue;

                    while ((right - left) >= widthStep)
                    {
                        double width = (left + right) / 2.0;
                        double height = width / ratio;
                        //wsl::coor::Point pt0 = origin;
                        vector<Point> rectPoly;
                        rectPoly.push_back(Point(origin.x - width / 2, origin.y - height / 2.0));
                        rectPoly.push_back(Point(origin.x + width / 2, origin.y - height / 2.0));
                        rectPoly.push_back(Point(origin.x + width / 2, origin.y + height / 2.0));
                        rectPoly.push_back(Point(origin.x - width / 2, origin.y + height / 2.0));
                        vector<Point> rotateRectPoly = util::rotatePoly(rectPoly, origin, angleRad);
                        if (util::polyInsidePoly(rotateRectPoly, polygon))
                        {
                            maxArea = width * height;
                            center.x = origin.x + src_minx;
                            center.y = origin.y + src_miny;
                            resAngle = angle;
                            maxRect = rotateRectPoly;
                            for (int rect_index = 0; rect_index < maxRect.size(); rect_index++)
                            {
                                maxRect[rect_index].x += src_minx;
                                maxRect[rect_index].y += src_miny;
                            }
                            resWidth = width;
                            resHeight = height;
                            left = width;
                        }
                        else
                        {
                            right = width;
                        }
                    }//endwhile
                }//endfor ratio
            }//endfor modifOrigins
        }//endfor origin point
    }//endfor angle

    if (maxRect.size() > 0)
        return true;
    else
        return false;
}