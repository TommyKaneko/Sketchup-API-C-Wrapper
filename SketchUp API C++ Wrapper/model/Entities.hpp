//
//  Entities.hpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef Entities_hpp
#define Entities_hpp

#include <stdio.h>
#include <vector>

#include <SketchUpAPI/model/entities.h>

#include "String.hpp"

namespace CW {

// Forward declarations
class Entity;
class Edge;
class Face;
class ComponentInstance;
class Group;
class ComponentDefinition;
class GeometryInput;
class Transformation;
class String;

/*
* Entities wrapper
*/
class Entities {
	private:
  SUEntitiesRef m_entities;
  
  public:
  Entities(SUEntitiesRef entities);
  
  
  /**
  * Null Entities object
  */
  Entities();
  
  /**
  * Fills an Entities object with geometry in GeometryInput object.
  */
	SU_RESULT fill(GeometryInput &geom_input);

  std::vector<Face> faces() const;
  std::vector<Edge> edges(bool stray_only = true) const;
  std::vector<ComponentInstance> instances() const;
  std::vector<Group> groups() const;
  
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
  std::vector<Face> add_faces(const std::vector<Face>& faces);
  Face add_face(const Face& face);

  /*
  * Creates edges in the Entities object.
  *
  * Note that this function does not merge overlapping geometry. @see GeometryInput and Entities::fill() for merging functionality.
  * @param vector of Edge objects to add
	*/
  std::vector<Edge> add_edges(const std::vector<Edge>& edges);
  Edge add_edge(const Edge& edge);

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
  * Transforms given entities by the transforamtion object
  * @param elems - vector array of Entity objects
  * @param transform - the transformation to apply to the elements.
  * @return true if the operation was successful.  False, if transform failed for any reason.
  */
  bool transform_entities(std::vector<Entity>& elems, const Transformation& transform);
  
  /*
  * The class object can be converted to a SUEntitiesRef without loss of data.
  */
  operator SUEntitiesRef();

};

} /* namespace CW */
#endif /* Entities_hpp */
