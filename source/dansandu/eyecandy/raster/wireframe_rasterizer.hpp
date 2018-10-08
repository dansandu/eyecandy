#pragma once

#include "dansandu/eyecandy/math/interpolation.hpp"
#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/raster/line_tracer.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

template<typename Precision, typename Shader>
class WireframeRasterizer {
    using Vertex = dansandu::eyecandy::math::RowVector3<Precision>;

public:
    explicit WireframeRasterizer(Shader shader) : shader_{std::move(shader)} {}

    auto operator()(const Vertex& a, const Vertex& b, const Vertex& c) {
        drawLine(a, b);
        drawLine(a, c);
        drawLine(b, c);
    }

private:
    auto drawLine(const Vertex& a, const Vertex& b) {
        using dansandu::eyecandy::math::interpolate;
        using dansandu::eyecandy::math::matrix_cast;
        using dansandu::eyecandy::math::size_type;
        using slicer = dansandu::eyecandy::math::slicer<0, 1, 0, 2>;

        auto lineTracer =
            LineTracer{matrix_cast<size_type>(slicer::slice(a)), matrix_cast<size_type>(slicer::slice(b))};
        do {
            auto point = lineTracer.position();
            auto x = static_cast<Precision>(point.x());
            auto y = static_cast<Precision>(point.y());
            shader_(point, interpolate(a, b, x, y, epsilon_));
        } while (lineTracer.step());
    }

    static constexpr auto epsilon_ = 10e-5;

    Shader shader_;
};

template<typename Precision, typename Shader>
auto makeWireframeRasterizer(Shader&& shader) {
    return WireframeRasterizer<Precision, Shader>{std::forward<Shader>(shader)};
}
}
}
}
