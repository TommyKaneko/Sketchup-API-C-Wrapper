//
//  Camera.hpp
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

#ifndef Camera_hpp
#define Camera_hpp

#include <SketchUpAPI/model/camera.h>

#include "SUAPI-CppWrapper/Geometry.hpp"
#include "SUAPI-CppWrapper/Transformation.hpp"

namespace CW {

// Forward declarations
class String;

class Camera {
  private:
  SUCameraRef m_camera;
  bool m_attached;

  static SUCameraRef create_camera();
  static SUCameraRef copy_reference(const Camera& other);

  public:
  /**
  * @brief Constructs a NULL Camera object.
  */
  Camera();

  /**
  * @brief Constructs a new standalone Camera with the given orientation.
  * @param eye - the camera eye position.
  * @param target - the target position.
  * @param up - the up direction.
  */
  Camera(const Point3D& eye, const Point3D& target, const Vector3D& up);

  /**
  * @brief Constructs a Camera from an existing SUCameraRef.
  * @param camera - SUCameraRef object.
  * @param attached - flag indicating whether the camera is owned by a scene
  *   or model (true) or is a standalone object (false).
  */
  Camera(SUCameraRef camera, bool attached = true);

  /** @brief Copy Constructor */
  Camera(const Camera& other);

  /** @brief Destructor */
  ~Camera();

  /** @brief Copy assignment operator */
  Camera& operator=(const Camera& other);

  /**
  * @brief Returns the SUCameraRef object.
  */
  SUCameraRef ref() const;
  operator SUCameraRef() const;
  operator SUCameraRef*();

  /**
  * @brief Returns true if this is an invalid/null camera.
  */
  bool operator!() const;

  /**
  * @brief Returns whether this camera is attached to a parent object (scene or model).
  */
  bool attached() const;

  /**
  * @brief Sets the attached flag.
  * @param attach - true if the camera is owned by a parent object.
  */
  void attached(bool attach);

  /**
  * @brief Retrieves the orientation of the camera object.
  * @since SketchUp 2014, API 2.0
  * @param position - the camera eye position retrieved.
  * @param target - the target position retrieved.
  * @param up_vector - the up direction retrieved.
  */
  void orientation(Point3D& position, Point3D& target, Vector3D& up_vector) const;

  /**
  * @brief Sets the position of the camera object.
  * @since SketchUp 2014, API 2.0
  * @param position - the new eye position.
  * @param target - the new target position.
  * @param up_vector - the new up direction.
  */
  void set_orientation(const Point3D& position, const Point3D& target, const Vector3D& up_vector);

  /**
  * @brief Retrieves the camera's eye (position) point.
  * @since SketchUp 2014, API 2.0
  */
  Point3D eye() const;

  /**
  * @brief Retrieves the camera's target point.
  * @since SketchUp 2014, API 2.0
  */
  Point3D target() const;

  /**
  * @brief Retrieves the camera's up vector.
  * @since SketchUp 2014, API 2.0
  */
  Vector3D up() const;

  /**
  * @brief Retrieves the camera's direction vector.
  * @since SketchUp 2017, API 5.0
  */
  Vector3D direction() const;

  /**
  * @brief Retrieves the look at matrix of the camera object.
  * @since SketchUp 2017, API 5.0
  */
  Transformation view_transformation() const;

  /**
  * @brief Sets the field of view angle of the camera object. If the camera
  *   object is an orthographic camera, the camera subsequently becomes a
  *   perspective camera. The field of view is measured along the vertical
  *   direction of the camera.
  * @since SketchUp 2014, API 2.0
  * @param fov_degrees - the field of view angle in degrees.
  */
  void fov(double fov_degrees);

  /**
  * @brief Retrieves the field of view in degrees of the camera object. The
  *   field of view is measured along the vertical direction of the camera.
  * @since SketchUp 2014, API 2.0
  * @throws std::logic_error if the camera is orthographic.
  */
  double fov() const;

  /**
  * @brief Sets the aspect ratio of the camera object.
  * @since SketchUp 2017, API 5.0
  * @param ratio - the aspect ratio to be set.
  */
  void aspect_ratio(double ratio);

  /**
  * @brief Retrieves the aspect ratio of the camera object.
  * @since SketchUp 2014, API 2.0
  * @throws std::logic_error if the camera uses the screen aspect ratio.
  */
  double aspect_ratio() const;

  /**
  * @brief Sets the height of the camera view which is used to calculate the
  *   orthographic projection. If the camera object is a perspective camera,
  *   the camera subsequently becomes an orthographic camera.
  * @since SketchUp 2014, API 2.0
  * @param height - the height of the camera view.
  */
  void orthographic_height(double height);

  /**
  * @brief Retrieves the height of an orthographic camera object.
  * @since SketchUp 2014, API 2.0
  * @throws std::logic_error if the camera is perspective.
  */
  double orthographic_height() const;

  /**
  * @brief Sets the camera perspective or orthographic.
  * @since SketchUp 2014, API 2.0
  * @param is_perspective - true for perspective, false for orthographic.
  */
  void perspective(bool is_perspective);

  /**
  * @brief Retrieves whether the camera object is a perspective camera or not
  *   (i.e. orthographic).
  * @since SketchUp 2014, API 2.0
  */
  bool perspective() const;

  /**
  * @brief Retrieves the near and far clipping distances of the camera object.
  * @since SketchUp 2017, API 5.0
  * @return a pair containing (znear, zfar) clipping distances.
  */
  std::pair<double, double> clipping_distances() const;

  /**
  * @brief Sets whether the field of view value represents the camera view height.
  * @since SketchUp 2017, API 5.0
  * @param is_fov_height - the field of view flag to set.
  */
  void fov_is_height(bool is_fov_height);

  /**
  * @brief Retrieves whether the field of view value represents the camera view
  *   height.
  * @since SketchUp 2017, API 5.0
  */
  bool fov_is_height() const;

  /**
  * @brief Sets the size of the image on the "film" for a perspective camera.
  *   The value is given in millimeters. It is used in the conversions between
  *   field of view and focal length.
  * @since SketchUp 2017, API 5.0
  * @param width - the width to set in millimeters.
  */
  void image_width(double width);

  /**
  * @brief Retrieves the size of the image on the image plane of the Camera.
  *   Unlike most length values in SketchUp, this width is specified in
  *   millimeters rather than in inches.
  * @since SketchUp 2017, API 5.0
  */
  double image_width() const;

  /**
  * @brief Sets the description of the camera object. Assumed to be UTF-8 encoded.
  * @since SketchUp 2017, API 5.0
  * @param desc - the description to be set.
  */
  void description(const String& desc);
  void description(const std::string& desc);

  /**
  * @brief Retrieves the description of the camera object.
  * @since SketchUp 2017, API 5.0
  */
  String description() const;

  /**
  * @brief Sets whether the camera is two dimensional. 2 point perspective mode
  *   and PhotoMatch mode are 2D cameras.
  * @since SketchUp 2017, API 5.0
  * @param make_2d - true to make the camera 2D.
  */
  void is_2d(bool make_2d);

  /**
  * @brief Retrieves whether the camera object is two dimensional.
  * @since SketchUp 2017, API 5.0
  */
  bool is_2d() const;

  /**
  * @brief Sets the camera's 2D scale factor.
  * @since SketchUp 2017, API 5.0
  * @param scale - the scale to be set.
  */
  void scale_2d(double scale);

  /**
  * @brief Retrieves the camera's 2D scale factor.
  * @since SketchUp 2017, API 5.0
  */
  double scale_2d() const;

  /**
  * @brief Sets the camera's 2D center point. The point coordinates are in
  *   screen space. Since this is setting the 2D center point the z component
  *   of the provided point is ignored.
  * @since SketchUp 2017, API 5.0
  * @param center - the center point to be set.
  */
  void center_2d(const Point3D& center);

  /**
  * @brief Retrieves the camera's 2D center point. Since this is accessing a
  *   2D point with a 3D point structure the z coordinate is always set to 0.0.
  * @since SketchUp 2017, API 5.0
  */
  Point3D center_2d() const;

  /**
  * @brief Creates a copy of this camera object, duplicating all properties.
  * @return a new unattached Camera object with the same properties.
  * @throws std::logic_error if this camera is null.
  */
  Camera copy() const;
};

} /* namespace CW */

#endif /* Camera_hpp */
