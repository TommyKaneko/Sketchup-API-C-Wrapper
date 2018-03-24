//
//  Entities.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2017 Tom Kaneko
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

#include <stdio.h>
#include <vector>

#include <SketchUpAPI/model/entities.h>

#include "SUAPI-CppWrapper/String.hpp"

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

/*
* Entities wrapper
*/
class Entities {
  private:
  SUEntitiesRef m_entities;
  // Store associated model ref, for type checking purposes
  SUModelRef m_model;
  
  public:
  /**
  * Default constructor.
  * @param model - SUModelRef object that this entities object resides in. Used for checking that entities added to the entities object is valid for adding to this model.
  */
  Entities(SUEntitiesRef entities, const SUModelRef model);
  
  /**
  * Null Entities object
  */
  Entities();
  
  /**
  * Fills an Entities object with geometry in GeometryInput object.
  */
  SUResult fill(GeometryInput &geom_input);

  std::vector<Face> faces() const;
  std::vector<Edge> edges(bool stray_only = true) const;
  std::vector<ComponentInstance> instances() const;
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
  *
  * Note that this function does not merge overlapping geometry. See GeometryInput for merging functionality.
  * @param vector of Face objects from CW
  */
  std::vector<Face> add_faces(std::vector<Face>& faces);
  Face add_face(Face& face);

  /*
  * Creates edges in the Entities object.
  *
  * Note that this function does not merge overlapping geometry. @see GeometryInput and Entities::fill() for merging functionality.
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
  * Returns the model object that conrtains this entities object.
  */
  Model model() const;
  
  /*
  * The class object can be converted to a SUEntitiesRef without loss of data.
  */
  operator SUEntitiesRef();

};

} /* namespace CW */
#endif /* Entities_hpp */
