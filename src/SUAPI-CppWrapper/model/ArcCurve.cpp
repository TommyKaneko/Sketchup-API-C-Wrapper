//
//  ArcCurve.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/ArcCurve.hpp"

#include <cassert>
#include <stdexcept>

#include <SketchUpAPI/model/curve.h>

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUArcCurveRef ArcCurve::create_arc_curve(
    const Point3D& center, const Point3D& start_point,
    const Point3D& end_point, const Vector3D& normal, size_t num_edges) {
  SUArcCurveRef arc = SU_INVALID;
  SUPoint3D c = center;
  SUPoint3D s = start_point;
  SUPoint3D e = end_point;
  SUVector3D n = normal;
  SUResult res = SUArcCurveCreate(&arc, &c, &s, &e, &n, num_edges);
  assert(res == SU_ERROR_NONE); _unused(res);
  return arc;
}


SUArcCurveRef ArcCurve::copy_reference(const ArcCurve& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  // Create a new arc curve with the same geometry
  SUPoint3D c, s, e;
  SUVector3D n;
  SUResult res = SUArcCurveGetCenter(other.ref(), &c);
  assert(res == SU_ERROR_NONE);
  res = SUArcCurveGetStartPoint(other.ref(), &s);
  assert(res == SU_ERROR_NONE);
  res = SUArcCurveGetEndPoint(other.ref(), &e);
  assert(res == SU_ERROR_NONE);
  res = SUArcCurveGetNormal(other.ref(), &n);
  assert(res == SU_ERROR_NONE); _unused(res);
  return create_arc_curve(Point3D(c), Point3D(s), Point3D(e), Vector3D(n),
                          other.num_edges());
}


/******************************
** Constructors / Destructor **
*******************************/
ArcCurve::ArcCurve():
  Entity(SU_INVALID, true)
{}


ArcCurve::ArcCurve(const Point3D& center, const Point3D& start_point,
                    const Point3D& end_point, const Vector3D& normal,
                    size_t num_edges):
  Entity(SUArcCurveToEntity(
    create_arc_curve(center, start_point, end_point, normal, num_edges)), false)
{}


ArcCurve::ArcCurve(SUArcCurveRef arc_curve, bool attached):
  Entity(SUArcCurveToEntity(arc_curve), attached)
{}


ArcCurve::ArcCurve(const ArcCurve& other):
  Entity(other, SUArcCurveToEntity(copy_reference(other)))
{}


ArcCurve::~ArcCurve() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUArcCurveRef arc = this->ref();
    SUResult res = SUArcCurveRelease(&arc);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


ArcCurve& ArcCurve::operator=(const ArcCurve& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUArcCurveRef arc = this->ref();
    SUResult res = SUArcCurveRelease(&arc);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUArcCurveToEntity(copy_reference(other));
  Entity::operator=(other);
  return *this;
}


/*******************
** Public Methods **
********************/
SUArcCurveRef ArcCurve::ref() const {
  return SUArcCurveFromEntity(m_entity);
}


bool ArcCurve::operator!() const {
  return SUIsInvalid(m_entity);
}


ArcCurve ArcCurve::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::copy(): ArcCurve is null");
  }
  ArcCurve new_arc(this->center(), this->start_point(), this->end_point(),
                   this->normal(), this->num_edges());
  new_arc.copy_attributes_from(*this);
  return new_arc;
}


size_t ArcCurve::num_edges() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::num_edges(): ArcCurve is null");
  }
  SUCurveRef curve = SUArcCurveToCurve(this->ref());
  size_t count = 0;
  SUResult res = SUCurveGetNumEdges(curve, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


double ArcCurve::radius() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::radius(): ArcCurve is null");
  }
  double r;
  SUResult res = SUArcCurveGetRadius(this->ref(), &r);
  assert(res == SU_ERROR_NONE); _unused(res);
  return r;
}


Point3D ArcCurve::start_point() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::start_point(): ArcCurve is null");
  }
  SUPoint3D point;
  SUResult res = SUArcCurveGetStartPoint(this->ref(), &point);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(point);
}


Point3D ArcCurve::end_point() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::end_point(): ArcCurve is null");
  }
  SUPoint3D point;
  SUResult res = SUArcCurveGetEndPoint(this->ref(), &point);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(point);
}


Vector3D ArcCurve::x_axis() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::x_axis(): ArcCurve is null");
  }
  SUVector3D axis;
  SUResult res = SUArcCurveGetXAxis(this->ref(), &axis);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(axis);
}


Vector3D ArcCurve::y_axis() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::y_axis(): ArcCurve is null");
  }
  SUVector3D axis;
  SUResult res = SUArcCurveGetYAxis(this->ref(), &axis);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(axis);
}


Point3D ArcCurve::center() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::center(): ArcCurve is null");
  }
  SUPoint3D point;
  SUResult res = SUArcCurveGetCenter(this->ref(), &point);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(point);
}


Vector3D ArcCurve::normal() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::normal(): ArcCurve is null");
  }
  SUVector3D norm;
  SUResult res = SUArcCurveGetNormal(this->ref(), &norm);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(norm);
}


double ArcCurve::start_angle() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::start_angle(): ArcCurve is null");
  }
  double angle;
  SUResult res = SUArcCurveGetStartAngle(this->ref(), &angle);
  assert(res == SU_ERROR_NONE); _unused(res);
  return angle;
}


double ArcCurve::end_angle() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::end_angle(): ArcCurve is null");
  }
  double angle;
  SUResult res = SUArcCurveGetEndAngle(this->ref(), &angle);
  assert(res == SU_ERROR_NONE); _unused(res);
  return angle;
}


bool ArcCurve::is_full_circle() const {
  if (!(*this)) {
    throw std::logic_error("CW::ArcCurve::is_full_circle(): ArcCurve is null");
  }
  bool is_full;
  SUResult res = SUArcCurveGetIsFullCircle(this->ref(), &is_full);
  assert(res == SU_ERROR_NONE); _unused(res);
  return is_full;
}


} /* namespace CW */
