#include "dansandu/eyecandy/transform.hpp"
#include "dansandu/math/common.hpp"

using dansandu::math::common::close;
using dansandu::math::matrix::crossProduct;
using dansandu::math::matrix::dotProduct;
using dansandu::math::matrix::normalized;

namespace dansandu::eyecandy::transform
{

Vector3 interpolate(const ConstantVector3View a, const ConstantVector3View b, const float x, const float y,
                    const float epsilon)
{
    auto t = a.z() < b.z() ? 0.0f : 1.0f;
    if (!close(a.x(), b.x(), epsilon))
    {
        t = (x - a.x()) / (b.x() - a.x());
    }
    else if (!close(a.y(), b.y(), epsilon))
    {
        t = (y - a.y()) / (b.y() - a.y());
    }
    return a + t * (b - a);
}

Vector3 interpolate(const ConstantVector3View a, const ConstantVector3View b, const ConstantVector3View c,
                    const float x, const float y, const float epsilon)
{
    const auto n = crossProduct(a - c, b - c);
    const auto z = close(n.z(), 0.0f, epsilon) ? std::min(a.z(), std::min(b.z(), c.z()))
                                               : (n.x() * x + n.y() * y - dotProduct(c, n)) / -n.z();
    return Vector3{{x, y, z}};
}

Matrix4 scale(const float x, const float y, const float z)
{
    // clang-format off
    return Matrix4{{{   x, 0.0f, 0.0f, 0.0f},
                    {0.0f,    y, 0.0f, 0.0f},
                    {0.0f, 0.0f,    z, 0.0f},
                    {0.0f, 0.0f, 0.0f, 1.0f}}};
    // clang-format on
}

Matrix4 translate(const float x, const float y, const float z)
{
    // clang-format off
    return Matrix4{{{1.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 1.0f, 0.0f},
                    {   x,    y,    z, 1.0f}}};
    // clang-format on
}

Matrix4 translate(const ConstantVector3View vector)
{
    return translate(vector.x(), vector.y(), vector.z());
}

Matrix4 rotateByX(const float radians)
{
    const auto cos = std::cos(radians);
    const auto sin = std::sin(radians);
    // clang-format off
    return Matrix4{{{1.0f,   0.0f,  0.0f, 0.0f},
                    {0.0f,    cos,   sin, 0.0f},
                    {0.0f,   -sin,   cos, 0.0f},
                    {0.0f,   0.0f,  0.0f, 1.0f}}};
    // clang-format on
}

Matrix4 rotateByY(const float radians)
{
    const auto cos = std::cos(radians);
    const auto sin = std::sin(radians);
    // clang-format off
    return Matrix4{{{ cos, 0.0f, -sin, 0.0f},
                    {0.0f, 1.0f, 0.0f, 0.0f},
                    { sin, 0.0f,  cos, 0.0f},
                    {0.0f, 0.0f, 0.0f, 1.0f}}};
    // clang-format on
}

Matrix4 rotateByZ(const float radians)
{
    const auto cos = std::cos(radians);
    const auto sin = std::sin(radians);
    // clang-format off
    return Matrix4{{{ cos,   sin, 0.0f, 0.0f},
                    {-sin,   cos, 0.0f, 0.0f},
                    {0.0f,  0.0f, 1.0f, 0.0f},
                    {0.0f,  0.0f, 0.0f, 1.0f}}};
    // clang-format on
}

Matrix4 lookAt(const ConstantVector3View eye, const ConstantVector3View target, const ConstantVector3View up)
{
    const auto w = normalized(eye - target);
    const auto u = normalized(crossProduct(up, w));
    const auto v = crossProduct(w, u);
    // clang-format off
    return translate(-1.0f * eye) * Matrix4{{{u.x(), v.x(), w.x(), 0.0f},
                                             {u.y(), v.y(), w.y(), 0.0f},
                                             {u.z(), v.z(), w.z(), 0.0f},
                                             { 0.0f,  0.0f,  0.0f, 1.0f}}};
    // clang-format on
}

Matrix4 perspective(const float near, const float far, const float fieldOfViewRadians, const float aspect)
{
    const auto ctg = 1.0f / std::tan(0.5f * fieldOfViewRadians);
    const auto p33 = (far + near) / (near - far);
    const auto p43 = 2.0f * far * near / (near - far);
    // clang-format off
    return Matrix4{{{ ctg,         0.0f, 0.0f,  0.0f}, 
                    {0.0f, aspect * ctg, 0.0f,  0.0f},
                    {0.0f,         0.0f,  p33, -1.0f},
                    {0.0f,         0.0f,  p43,  0.0f}}};
    // clang-format on
}

Matrix4 viewport(const float width, const float height)
{
    return scale(width / 2.0f, height / 2.0f, 1.0f) * translate(width / 2.0f, height / 2.0f, -1.0f);
}

}
