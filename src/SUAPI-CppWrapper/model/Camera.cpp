//
//  Camera.cpp
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

#include "SUAPI-CppWrapper/model/Camera.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUCameraRef Camera::create_camera() {
  SUCameraRef camera = SU_INVALID;
  SUResult res = SUCameraCreate(&camera);
  assert(res == SU_ERROR_NONE); _unused(res);
  return camera;
}


SUCameraRef Camera::copy_reference(const Camera& other) {
  if (other.m_attached || SUIsInvalid(other.m_camera)) {
    return other.ref();
  }
  SUCameraRef new_camera = create_camera();
  // Copy orientation
  SUPoint3D position, target;
  SUVector3D up_vector;
  SUResult res = SUCameraGetOrientation(other.m_camera, &position, &target, &up_vector);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUCameraSetOrientation(new_camera, &position, &target, &up_vector);
  assert(res == SU_ERROR_NONE); _unused(res);
  // Copy perspective/ortho setting
  bool is_persp;
  res = SUCameraGetPerspective(other.m_camera, &is_persp);
  assert(res == SU_ERROR_NONE); _unused(res);
  res = SUCameraSetPerspective(new_camera, is_persp);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (is_persp) {
    double fov_val;
    res = SUCameraGetPerspectiveFrustumFOV(other.m_camera, &fov_val);
    if (res == SU_ERROR_NONE) {
      SUCameraSetPerspectiveFrustumFOV(new_camera, fov_val);
    }
  } else {
    double height;
    res = SUCameraGetOrthographicFrustumHeight(other.m_camera, &height);
    if (res == SU_ERROR_NONE) {
      SUCameraSetOrthographicFrustumHeight(new_camera, height);
    }
  }
  return new_camera;
}


/******************************
** Constructors / Destructor **
*******************************/
Camera::Camera():
  m_camera(SU_INVALID),
  m_attached(false)
{}


Camera::Camera(const Point3D& eye, const Point3D& target, const Vector3D& up):
  m_camera(create_camera()),
  m_attached(false)
{
  set_orientation(eye, target, up);
}


Camera::Camera(SUCameraRef camera, bool attached):
  m_camera(camera),
  m_attached(attached)
{}


Camera::Camera(const Camera& other):
  m_camera(copy_reference(other)),
  m_attached(other.m_attached)
{}


Camera::~Camera() {
  if (!m_attached && SUIsValid(m_camera)) {
    SUResult res = SUCameraRelease(&m_camera);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


/*******************
** Public Methods **
********************/
Camera& Camera::operator=(const Camera& other) {
  if (!m_attached && SUIsValid(m_camera)) {
    SUResult res = SUCameraRelease(&m_camera);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_camera = copy_reference(other);
  m_attached = other.m_attached;
  return (*this);
}


SUCameraRef Camera::ref() const {
  return m_camera;
}

Camera::operator SUCameraRef() const {
  return m_camera;
}

Camera::operator SUCameraRef*() {
  return &m_camera;
}

bool Camera::operator!() const {
  return SUIsInvalid(m_camera);
}


bool Camera::attached() const {
  return m_attached;
}


void Camera::attached(bool attach) {
  m_attached = attach;
}


void Camera::orientation(Point3D& position, Point3D& target, Vector3D& up_vector) const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::orientation(): Camera is null");
  }
  SUPoint3D pos, tgt;
  SUVector3D up;
  SUResult res = SUCameraGetOrientation(m_camera, &pos, &tgt, &up);
  assert(res == SU_ERROR_NONE); _unused(res);
  position = Point3D(pos);
  target = Point3D(tgt);
  up_vector = Vector3D(up);
}


void Camera::set_orientation(const Point3D& position, const Point3D& target, const Vector3D& up_vector) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::set_orientation(): Camera is null");
  }
  SUPoint3D pos = position;
  SUPoint3D tgt = target;
  SUVector3D up = up_vector;
  SUResult res = SUCameraSetOrientation(m_camera, &pos, &tgt, &up);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Point3D Camera::eye() const {
  Point3D pos, tgt;
  Vector3D up;
  orientation(pos, tgt, up);
  return pos;
}


Point3D Camera::target() const {
  Point3D pos, tgt;
  Vector3D up;
  orientation(pos, tgt, up);
  return tgt;
}


Vector3D Camera::up() const {
  Point3D pos, tgt;
  Vector3D up;
  orientation(pos, tgt, up);
  return up;
}


Vector3D Camera::direction() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::direction(): Camera is null");
  }
  SUVector3D dir;
  SUResult res = SUCameraGetDirection(m_camera, &dir);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Vector3D(dir);
}


Transformation Camera::view_transformation() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::view_transformation(): Camera is null");
  }
  SUTransformation transform;
  SUResult res = SUCameraGetViewTransformation(m_camera, &transform);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Transformation(transform);
}


void Camera::fov(double fov_degrees) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::fov(): Camera is null");
  }
  SUResult res = SUCameraSetPerspectiveFrustumFOV(m_camera, fov_degrees);
  assert(res == SU_ERROR_NONE); _unused(res);
}


double Camera::fov() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::fov(): Camera is null");
  }
  double fov_val;
  SUResult res = SUCameraGetPerspectiveFrustumFOV(m_camera, &fov_val);
  if (res == SU_ERROR_NO_DATA) {
    throw std::logic_error("CW::Camera::fov(): Camera is orthographic, no FOV available");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return fov_val;
}


void Camera::aspect_ratio(double ratio) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::aspect_ratio(): Camera is null");
  }
  SUResult res = SUCameraSetAspectRatio(m_camera, ratio);
  if (res != SU_ERROR_NONE) {
    throw std::invalid_argument("CW::Camera::aspect_ratio(): invalid aspect ratio");
  }
}


double Camera::aspect_ratio() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::aspect_ratio(): Camera is null");
  }
  double ratio;
  SUResult res = SUCameraGetAspectRatio(m_camera, &ratio);
  if (res == SU_ERROR_NO_DATA) {
    throw std::logic_error("CW::Camera::aspect_ratio(): Camera uses screen aspect ratio");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return ratio;
}


void Camera::orthographic_height(double height) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::orthographic_height(): Camera is null");
  }
  SUResult res = SUCameraSetOrthographicFrustumHeight(m_camera, height);
  assert(res == SU_ERROR_NONE); _unused(res);
}


double Camera::orthographic_height() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::orthographic_height(): Camera is null");
  }
  double height;
  SUResult res = SUCameraGetOrthographicFrustumHeight(m_camera, &height);
  if (res == SU_ERROR_NO_DATA) {
    throw std::logic_error("CW::Camera::orthographic_height(): Camera is perspective, no height available");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return height;
}


void Camera::perspective(bool is_perspective) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::perspective(): Camera is null");
  }
  SUResult res = SUCameraSetPerspective(m_camera, is_perspective);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Camera::perspective() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::perspective(): Camera is null");
  }
  bool is_persp;
  SUResult res = SUCameraGetPerspective(m_camera, &is_persp);
  assert(res == SU_ERROR_NONE); _unused(res);
  return is_persp;
}


std::pair<double, double> Camera::clipping_distances() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::clipping_distances(): Camera is null");
  }
  double znear, zfar;
  SUResult res = SUCameraGetClippingDistances(m_camera, &znear, &zfar);
  assert(res == SU_ERROR_NONE); _unused(res);
  return {znear, zfar};
}


void Camera::fov_is_height(bool is_fov_height) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::fov_is_height(): Camera is null");
  }
  SUResult res = SUCameraSetFOVIsHeight(m_camera, is_fov_height);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Camera::fov_is_height() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::fov_is_height(): Camera is null");
  }
  bool is_fov_h;
  SUResult res = SUCameraGetFOVIsHeight(m_camera, &is_fov_h);
  assert(res == SU_ERROR_NONE); _unused(res);
  return is_fov_h;
}


void Camera::image_width(double width) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::image_width(): Camera is null");
  }
  SUResult res = SUCameraSetImageWidth(m_camera, width);
  assert(res == SU_ERROR_NONE); _unused(res);
}


double Camera::image_width() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::image_width(): Camera is null");
  }
  double width;
  SUResult res = SUCameraGetImageWidth(m_camera, &width);
  assert(res == SU_ERROR_NONE); _unused(res);
  return width;
}


void Camera::description(const String& desc) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::description(): Camera is null");
  }
  SUResult res = SUCameraSetDescription(m_camera, desc.std_string().c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Camera::description(const std::string& desc) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::description(): Camera is null");
  }
  SUResult res = SUCameraSetDescription(m_camera, desc.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


String Camera::description() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::description(): Camera is null");
  }
  String string;
  SUResult res = SUCameraGetDescription(m_camera, string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


void Camera::is_2d(bool make_2d) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::is_2d(): Camera is null");
  }
  SUResult res = SUCameraSet2D(m_camera, make_2d);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Camera::is_2d() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::is_2d(): Camera is null");
  }
  bool val;
  SUResult res = SUCameraGet2D(m_camera, &val);
  assert(res == SU_ERROR_NONE); _unused(res);
  return val;
}


void Camera::scale_2d(double scale) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::scale_2d(): Camera is null");
  }
  SUResult res = SUCameraSetScale2D(m_camera, scale);
  assert(res == SU_ERROR_NONE); _unused(res);
}


double Camera::scale_2d() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::scale_2d(): Camera is null");
  }
  double scale;
  SUResult res = SUCameraGetScale2D(m_camera, &scale);
  assert(res == SU_ERROR_NONE); _unused(res);
  return scale;
}


void Camera::center_2d(const Point3D& center) {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::center_2d(): Camera is null");
  }
  SUPoint3D pt = center;
  SUResult res = SUCameraSetCenter2D(m_camera, &pt);
  assert(res == SU_ERROR_NONE); _unused(res);
}


Point3D Camera::center_2d() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::center_2d(): Camera is null");
  }
  SUPoint3D pt;
  SUResult res = SUCameraGetCenter2D(m_camera, &pt);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Point3D(pt);
}


Camera Camera::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::Camera::copy(): Camera is null");
  }
  Camera new_camera(create_camera(), false);
  new_camera.set_orientation(this->eye(), this->target(), this->up());
  new_camera.perspective(this->perspective());
  if (this->perspective()) {
    new_camera.fov(this->fov());
  } else {
    new_camera.orthographic_height(this->orthographic_height());
  }
  new_camera.fov_is_height(this->fov_is_height());
  new_camera.image_width(this->image_width());
  new_camera.description(this->description());
  new_camera.is_2d(this->is_2d());
  new_camera.scale_2d(this->scale_2d());
  new_camera.center_2d(this->center_2d());
  return new_camera;
}


} /* namespace CW */
