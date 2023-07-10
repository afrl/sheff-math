#include "simple_math/LineGeometry.h"
#include "simple_math/Utility.h"

#include <assert.h>

using namespace simple_math;

int main()
{
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Test intersect_circle_line and intersect_circle_line_segment

    // Circle centered at (1, 1) with a radius of 0.5
    // Consider intersection with three segments:
    // o---------o segment #1 (no intersection)
    //    -----   o----o segment #2 (line is tangent but segment does not intersect)
    //   /  o--\----o segment #4 (line intersects twice but segment intersects once)
    //  |   x   |
    // o-\-----/--o segment #5 (line segment intersects twice)
    //    -----o----o segment #3 (tangent at the line segment endpoint)
    //

    const double radius = 0.5;
    const Point center  = { 1.0, 1.0, 0.0 };

    auto is_on_circle = [&](const Point& p) -> bool
    {
        return abs(distance(p, center) - radius) < 1e-12;
    };

    // Segment #1
    auto i = intersect_circle_line(radius, center, { 0.0, 1.75, 0.0 }, { 2.0, 1.75, 0.0 });
    assert(i.type == CircleIntersection::NO_INTERSECTION);
    i = intersect_circle_line_segment(radius, center, { 0.0, 1.75, 0.0 }, { 2.0, 1.75, 0.0 });
    assert(i.type == CircleIntersection::NO_INTERSECTION);

    // Segment #2
    i = intersect_circle_line(radius, center, { 1.75, 1.5, 0.0 }, { 2.0, 1.5, 0.0 });
    assert(i.type == CircleIntersection::TANGENT);
    assert(is_on_circle(i.intersect_1));
    i = intersect_circle_line_segment(radius, center, { 1.75, 1.5, 0.0 }, { 2.0, 1.5, 0.0 });
    assert(i.type == CircleIntersection::NO_INTERSECTION);

    // Segment #3
    i = intersect_circle_line(radius, center, { 1.0, 0.5, 0.0 }, { 2.0, 0.5, 0.0 });
    assert(i.type == CircleIntersection::TANGENT);
    assert(is_on_circle(i.intersect_1));
    i = intersect_circle_line_segment(radius, center, { 1.0, 0.5, 0.0 }, { 2.0, 0.5, 0.0 });
    assert(i.type == CircleIntersection::TANGENT);
    assert(is_on_circle(i.intersect_1));
    // Segment #4
    i = intersect_circle_line(radius, center, { 1.0, 1.25, 0.0 }, { 2.0, 1.25, 0.0 });
    assert(i.type == CircleIntersection::TWO_INTERSECTS);
    assert(is_on_circle(i.intersect_1));
    assert(is_on_circle(i.intersect_2));
    assert(i.intersect_1[0] < i.intersect_2[0]);
    i = intersect_circle_line_segment(radius, center, { 1.0, 1.25, 0.0 }, { 2.0, 1.25, 0.0 });
    assert(i.type == CircleIntersection::ONE_INTERSECT);
    assert(is_on_circle(i.intersect_1));

    // Segment #5
    i = intersect_circle_line(radius, center, { 0.0, 0.75, 0.0 }, { 2.0, 0.75, 0.0 });
    assert(i.type == CircleIntersection::TWO_INTERSECTS);
    assert(is_on_circle(i.intersect_1));
    assert(is_on_circle(i.intersect_2));
    assert(i.intersect_1[0] < i.intersect_2[0]);
    i = intersect_circle_line_segment(radius, center, { 0.0, 0.75, 0.0 }, { 2.0, 0.75, 0.0 });
    assert(i.type == CircleIntersection::TWO_INTERSECTS);
    assert(is_on_circle(i.intersect_1));
    assert(is_on_circle(i.intersect_2));
    assert(i.intersect_1[0] < i.intersect_2[0]);

    /////////////////////////////////////////////////////////////////////////////////////////////
    // Test is_point_in_polygon

    //    4 x Point #2 (on a polygon point)
    //     / \ x Point #1 (outside)
    //    /   \
    // 3 o     o 0
    //   |  x  x Point #3 (on a polygon line)
    //   |P #4 |
    // 2 o-----o 1

    std::vector<Point> polygon_points = { { 1, 1, 0 },
                                          { 1, 0, 0 },
                                          { 0, 0, 0 },
                                          { 0, 1, 0 },
                                          { 0.5, 1.5, 0 } };

    assert(!is_point_in_polygon({ 1, 1.25, 0 }, polygon_points));
    assert(is_point_in_polygon({ 0.5, 1.5, 0 }, polygon_points));
    assert(is_point_in_polygon({ 1, 0.75, 0 }, polygon_points));
    assert(is_point_in_polygon({ 0.5, 0.5, 0 }, polygon_points));
    

    return 0;
}