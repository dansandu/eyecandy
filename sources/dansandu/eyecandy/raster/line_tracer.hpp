#pragma once

#include "dansandu/eyecandy/common.hpp"

namespace dansandu::eyecandy::raster::line_tracer
{

class LineTracer
{
public:
    LineTracer(const Point2 a, const Point2 b);

    bool step();

    Point2 position() const;

private:
    int width_;
    int height_;
    int error_;
    int x_;
    int y_;
    int dx_;
    int dy_;
    int end_;
};

}
