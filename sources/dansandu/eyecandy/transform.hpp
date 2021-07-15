#pragma once

#include "dansandu/eyecandy/common.hpp"

namespace dansandu::eyecandy::transform
{

Vector3 interpolate(const ConstantVector3View a, const ConstantVector3View b, const float x, const float y,
                    const float epsilon);

Vector3 interpolate(const ConstantVector3View a, const ConstantVector3View b, const ConstantVector3View c,
                    const float x, const float y, const float epsilon);

Matrix4 scale(const float x, const float y, const float z);

Matrix4 translate(const float x, const float y, const float z);

Matrix4 translate(const ConstantVector3View vector);

Matrix4 rotateByX(const float radians);

Matrix4 rotateByY(const float radians);

Matrix4 rotateByZ(const float radians);

Matrix4 lookAt(const ConstantVector3View eye, const ConstantVector3View target, const ConstantVector3View up);

Matrix4 perspective(const float near, const float far, const float fieldOfViewRadians, const float aspect);

Matrix4 viewport(const float width, const float height);

}
