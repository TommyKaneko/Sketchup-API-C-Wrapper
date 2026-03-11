//
//  Entities.hpp
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

#ifndef Entities_hpp
#define Entities_hpp

#ifndef SketchUpAPI_VERSION_MAJOR
#error "SketchUpAPI_VERSION_MAJOR must be defined to include SUAPI-CppWrapper headers"
#endif

#include <vector>

#include <SketchUpAPI/model/entities.h>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"

namespace CW {

// Forward declarations
class Model;
class Entity;
class Edge;
class Face;
class ComponentInstance;
class Group;
class ComponentDefinition;
class GeometryInput;
class Transformation;
class String;
class BoundingBox3D;
class GuideLine;
class GuidePoint;
class SectionPlane;
class Image;
class Text;
class Dimension;
class ArcCurve;

/**
 * @brief C++ wrapper for SUEntitiesRef.
 *
 * An Entities object holds the geometric entities (faces, edges,
 * component instances, groups, etc.) that belong to a model or
 * component definition.
 * @see SUEntitiesRef
 */
class Entities {
  private:
  SUEntitiesRef m_entities;
  // Store associated model, for type checking purposes
  #if SketchUpAPI_VERSION_MAJOR < 2021
  Model m_model;
  #endif

  public:
  #if SketchUpAPI_VERSION_MAJOR < 2021
  /**
  * Default constructor.
  * @param model - SUModelRef object that this entities object resides in. Used for checking that entities added to the entities object is valid for adding to this model.
  */
  Entities(SUEntitiesRef entities, const SUModelRef model);
  Entities(SUEntitiesRef entities, const Model model);
  #else
  /**
  * Default constructor.
  * @param model - SUModelRef object that this entities object resides in. Used for checking that entities added to the entities object is valid for adding to this model.
  */
  Entities(SUEntitiesRef entities);
  #endif

  /**
  * Null Entities object
  */
  Entities();

  /**
   * @brief Fills this Entities object with geometry from a GeometryInput object.
   * @param geom_input  The GeometryInput to merge into this Entities.
   * @throws std::logic_error if the Entities is null.
   */
  void fill(GeometryInput &geom_input);

  /**
   * Gets the Faces in the Entities object.
   * @return std::vector<Face>
   */
  std::vector<Face> faces() const;

  /**
   * Gets the Edges in the Entities object.
   * @param stray_only - if true, only stray edges (not bounding faces) are returned.
   * @return std::vector<Face>
   */
  std::vector<Edge> edges(bool stray_only = true) const;

  /**
  * Return the ComponentInstances in the Entities object.
  */
  std::vector<ComponentInstance> instances() const;

  /**
  * Return the Groups in the Entities object.
  */
  std::vector<Group> groups() const;

  /**
  * Return the BoundingBox of the Entities object.
  */
  BoundingBox3D bounding_box() const;

  /**
  * Returns the number of entities that exist in the entities object.
  */
  size_t size() const;

  /**
  * Adds the contents of an entities object into this one.
  */
  void add(const Entities& other);

  /*
  * Creates faces in the Entities object.
  * NOTE: that this function does not merge overlapping geometry, which will likely create an invalid SketchUp model.  It is recommended to use @see GeometryInput and Entities::fill() generally for adding geometry to an Entities object.
  * @param vector - of Face objects from CW
  */
  std::vector<Face> add_faces(std::vector<Face>& faces);
  Face add_face(Face& face);

  /*
  * Creates edges in the Entities object.
  *
  * NOTE: that this function does not merge overlapping geometry, which will likely create an invalid SketchUp model.  It is recommended to use @see GeometryInput and Entities::fill() generally for adding geometry to an Entities object.
  * @param vector of Edge objects to add
  */
  std::vector<Edge> add_edges(std::vector<Edge>& edges);
  Edge add_edge(Edge& edge);

  /*
  * Creates a ComponentInstance in the Entities object.
  * @param instance ComponentInstance object to add to the Entities object.
  */
  void add_instance(ComponentInstance& instance);

  /*
  * Creates a ComponentInstance in the Entities object.
  * @param definition ComponentDefinition object to create an instance of
  * @param transformation transformation of the definition (placement, rotation and scale)
  */
  ComponentInstance add_instance(const ComponentDefinition& definition, const Transformation& transformation, const String& name = "");

  /*
  * Creates a Group in the Entities object.
  * @param definition ComponentDefinition object to create an group of
  * @param transformation transformation of the definition (placement, rotation and scale)
  */
  // TODO: this needs to be revised.
  Group add_group(const ComponentDefinition& definition, const Transformation& transformation);
  Group add_group();

  /**
  * Gets the GuidePoints in the Entities object.
  * @since SketchUp 2014 M1, API 2.1
  * @return std::vector of GuidePoint objects.
  */
  std::vector<GuidePoint> guide_points() const;

  /**
  * Adds guide points to the Entities object.
  * @since SketchUp 2014 M1, API 2.1
  * @param points - vector of GuidePoint objects to add. The guide points must
  *   not already be associated with a parent object.
  */
  void add_guide_points(std::vector<GuidePoint>& points);

  /**
  * Gets the GuideLines in the Entities object.
  * @since SketchUp 2016, API 4.0
  * @return std::vector of GuideLine objects.
  */
  std::vector<GuideLine> guide_lines() const;

  /**
  * Adds guide lines to the Entities object.
  * @since SketchUp 2016, API 4.0
  * @param lines - vector of GuideLine objects to add. The guide lines must
  *   not already be associated with a parent object.
  */
  void add_guide_lines(std::vector<GuideLine>& lines);

  /**
  * Gets the SectionPlanes in the Entities object.
  * @since SketchUp 2016, API 4.0
  * @return std::vector of SectionPlane objects.
  */
  std::vector<SectionPlane> section_planes() const;

  /**
  * Adds section planes to the Entities object.
  * @since SketchUp 2016, API 4.0
  * @param planes - vector of SectionPlane objects to add. The section planes
  *   must not already be associated with a parent object.
  */
  void add_section_planes(std::vector<SectionPlane>& planes);

  /**
  * Gets the Images in the Entities object.
  * @since SketchUp 2017 M0, API 5.0
  * @return std::vector of Image objects.
  */
  std::vector<Image> images() const;

  /**
  * Adds an image to the Entities object.
  * @since SketchUp 2017 M0, API 5.0
  * @param image - Image object to add. The image must not already be
  *   associated with a parent object.
  */
  void add_image(Image& image);

  /**
  * Gets the Text annotations in the Entities object.
  * @since SketchUp 2018, API 6.0
  * @return std::vector of Text objects.
  */
  std::vector<Text> texts() const;

  /**
  * Adds text annotations to the Entities object.
  * @since SketchUp 2018, API 6.0
  * @param texts - vector of Text objects to add. The texts must not
  *   already be associated with a parent object.
  */
  void add_texts(std::vector<Text>& texts);

  /**
  * Gets the Dimensions in the Entities object. This returns all dimensions
  * (both linear and radial) as base Dimension objects.
  * @since SketchUp 2017, API 5.0
  * @return std::vector of Dimension objects.
  * @note The SketchUp C API does not provide a function to add dimensions
  *   to entities. Dimensions can only be retrieved from existing models.
  */
  std::vector<Dimension> dimensions() const;

  #if SketchUpAPI_VERSION_MAJOR >= 2026
  /**
   * @brief Adds dimensions to the Entities object.
   * @since SketchUp 2026.1, API 14.1
   * @param dimensions - vector of Dimension objects to add. The dimensions must
   *   not already be associated with a parent object.
   */
  void add_dimensions(std::vector<Dimension>& dimensions);
  #endif
  /**
  * Gets the ArcCurves in the Entities object.
  * @since SketchUp 2016, API 4.0
  * @return std::vector of ArcCurve objects.
  */
  std::vector<ArcCurve> arc_curves() const;

  /**
  * Adds arc curves to the Entities object.
  * @since SketchUp 2016, API 4.0
  * @param arcs - vector of ArcCurve objects to add. The arc curves must
  *   not already be associated with a parent object.
  */
  void add_arc_curves(std::vector<ArcCurve>& arcs);


  /**
  * Transforms given entities by the transformation object
  * @since SketchUp 2017, API 5.0
  * @param elems - vector array of Entity objects
  * @param transform - the transformation to apply to the elements.
  * @return true if the operation was successful.  false, if transform failed.
  */
  bool transform_entities(std::vector<Entity>& elems, const Transformation& transform);

  /**
  * Transforms given entities each by their transformation objects.
  * @param elems - vector array of Entity objects
  * @param transforms - the vector array of transformations to apply to the elements.  The number of Transformation objects must match the number of elements given.
  * @return true if the operation was successful.  false, if transform failed.
  */
  bool transform_entities(std::vector<Entity>& elems, std::vector<Transformation>& transforms);

  /**
  * Returns the model object that contains this entities object.
  */
  Model model() const;

  /*
  * The class object can be converted to a SUEntitiesRef without loss of data.
  */
  operator SUEntitiesRef();

};

} /* namespace CW */
#endif /* Entities_hpp */
