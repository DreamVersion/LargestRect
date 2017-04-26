#include <iostream>
#include "Polygon.h"
#include <limits>
typedef std::numeric_limits< double > dbl;

int main()
{
    cout.precision(dbl::max_digits10);
    //cout<<220947387.3846235*101643724.28589579<<endl;
    vector<Point> polygon;
    polygon.push_back(Point(387,724));
    polygon.push_back(Point(391,715));
    polygon.push_back(Point(309,672));
    polygon.push_back(Point(297,680));
    polygon.push_back(Point(327,723));
    polygon.push_back(Point(367,818));
    polygon.push_back(Point(408,848));
    polygon.push_back(Point(451,753));

    Polygon poly(polygon);
    Point center;
    double angle=0.0;
    vector<Point> rect;
    double width=0.0;
    double height=0.0;
    if(poly.findLargestRetangle(center,angle,rect,width,height))
    {
        cout<<"center: x:"<<fixed<<center.x<<" y:"<<fixed<<center.y<<endl;
        cout<<"angle: "<<fixed<<angle<<endl;
        cout<<"Rectangle:"<<endl;
        for(int i=0;i<rect.size();i++)
        {
            cout<<"\tx:"<<fixed<<rect[i].x<<"\ty:"<<fixed<<rect[i].y<<endl;
        }
        cout<<"width: "<<fixed<<width<<endl;
        cout<<"height: "<<fixed<<height<<endl;
    }
    else{
        cout<<"can't find largest retangle"<<endl;
    }
    return 0;
}