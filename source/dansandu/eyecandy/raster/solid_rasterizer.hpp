#pragma once

#include "dansandu/eyecandy/math/interpolation.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/raster/line_tracer.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

template<typename Precision, typename Shader>
class SolidRasterizer {
    using size_type = dansandu::eyecandy::math::size_type;
    using Point2 = dansandu::eyecandy::math::RowVector2<size_type>;

public:
    using Vertex = dansandu::eyecandy::math::RowVector3<Precision>;

    explicit SolidRasterizer(Shader shader) : shader_{std::move(shader)} {}

    auto operator()(Vertex a, Vertex b, Vertex c) {
        using dansandu::eyecandy::math::interpolate;
        using dansandu::eyecandy::math::matrix_cast;
        using slicer = dansandu::eyecandy::math::slicer<0, 1, 0, 2>;

        auto pa = matrix_cast<size_type>(slicer::slice(a));
        auto pb = matrix_cast<size_type>(slicer::slice(b));
        auto pc = matrix_cast<size_type>(slicer::slice(c));
        sortPointsByYxAndMirror(pa, pb, pc, a, b, c);

        if (pa.y() != pb.y() && pb.y() != pc.y()) {
            auto x = (b.y() - a.y()) * (c.x() - a.x()) / (c.y() - a.y()) + a.x();
            auto d = interpolate(a, c, x, b.y(), epsilon_);
            auto pd = matrix_cast<size_type>(slicer::slice(d));
            if (d.x() < b.x()) {
                d.swap(b);
                pd.swap(pb);
            }
            drawFlatTriangle(pa, pb, pd, a, b, d);
            drawFlatTriangle(pc, pb, pd, c, b, d);
        } else if (pa.y() == pb.y())
            drawFlatTriangle(pc, pa, pb, c, a, b);
        else
            drawFlatTriangle(pa, pb, pc, a, b, c);
    }

private:
    static auto smallerThanYxOrder(const Point2& pa, const Point2& pb) {
        return pa.y() < pb.y() || (pa.y() == pb.y() && pa.x() < pb.x());
    }

    static auto sortPointsByYxAndMirror(Point2& pa, Point2& pb, Point2& pc, Vertex& a, Vertex& b, Vertex& c) {
        using std::swap;
        if (smallerThanYxOrder(pa, pb)) {
            if (smallerThanYxOrder(pc, pa)) {
                swap(pa, pc);
                swap(a, c);
            }
        } else {
            if (smallerThanYxOrder(pb, pc)) {
                swap(pa, pb);
                swap(a, b);
            } else {
                swap(pa, pc);
                swap(a, c);
            }
        }
        if (smallerThanYxOrder(pc, pb)) {
            swap(pb, pc);
            swap(b, c);
        }
    }

    auto drawFlatTriangle(const Point2& pTip, const Point2& pFlatLeft, const Point2& pFlatRight, const Vertex& tip,
                          const Vertex& flatLeft, const Vertex& flatRight) {
        using dansandu::eyecandy::math::interpolate;

        auto dy = pTip.y() < pFlatLeft.y() ? 1 : -1;
        LineTracer leftTracer{pTip, pFlatLeft}, rightTracer{pTip, pFlatRight};
        for (auto py = pTip.y(); py != pFlatLeft.y() + dy; py += dy) {
            auto leftXbegin = leftTracer.position().x(), leftXend = leftXbegin;
            auto rightXbegin = rightTracer.position().x(), rightXend = rightXbegin;

            while (leftTracer.step() && py == leftTracer.position().y())
                leftXend = leftTracer.position().x();

            while (rightTracer.step() && py == rightTracer.position().y())
                rightXend = rightTracer.position().x();

            for (auto px = std::min(leftXbegin, leftXend); px <= std::max(rightXbegin, rightXend); ++px) {
                auto x = static_cast<Precision>(px);
                auto y = static_cast<Precision>(py);
                shader_(Point2{{px, py}}, interpolate(tip, flatLeft, flatRight, x, y, epsilon_));
            }
        }
    }

    static constexpr auto epsilon_ = 10e-5;

    Shader shader_;
};

template<typename Precision, typename Shader>
auto makeSolidRasterizer(Shader&& shader) {
    return SolidRasterizer<Precision, Shader>{std::forward<Shader>(shader)};
}
}
}
}
