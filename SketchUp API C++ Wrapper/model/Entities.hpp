//
//  Entities.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Entities_hpp
#define Entities_hpp

#include <stdio.h>

#include <SketchUpAPI/model/entities.h>

#include "Entity.hpp"
#include "Face.hpp"
#include "ComponentInstance.hpp"
#include "Group.hpp"
#include "ComponentDefinition.hpp"
#include "GeometryInput.hpp"
#include "Geometry.hpp"
#include "Transformation.hpp"

namespace CW {

/*
* Entities wrapper
*/
class Entities {
	private:
  SUEntitiesRef m_entities;
  
  public:
  Entities(SUEntitiesRef entities);
  
  /**
  * Fills an Entities object with geometry in GeometryInput object.
  */
	SU_RESULT fill(GeometryInput &geom_input);


  std::vector<Face> faces();
  std::vector<Edge> edges(bool stray_only = true);
  std::vector<ComponentInstance> instances();
  std::vector<Group> groups();
    
  /*
  * Creates faces in the Entities object.
  *
  * Note that this function does not merge overlapping geometry. See GeometryInput for merging functionality.
  * @param vector of Face objects from CW
	*/
  std::vector<Face> add_faces(std::vector<Face> faces);
  Face add_face(Face face);

  /*
  * Creates edges in the Entities object.
  *
  * Note that this function does not merge overlapping geometry. See GeometryInput for merging functionality.
  * @param vector of Face objects from CW
	*/
  std::vector<Edge> add_edges(std::vector<Edge> edges);
  Face add_edge(Edge edge);

  /*
  * Creates a ComponentInstance in the Entities object.
  * @param definition ComponentDefinition object to create an instance of
  * @param transformation transformation of the definition (placement, rotation and scale)
	*/
  ComponentInstance add_instance(ComponentDefinition definition, Transformation transformation);

  /*
  * Creates a Group in the Entities object.
  * @param definition ComponentDefinition object to create an group of
  * @param transformation transformation of the definition (placement, rotation and scale)
  */
  Group add_group(ComponentDefinition definition, Transformation transformation);
  
  /*
  * The class object can be converted to a SUEntitiesRef without loss of data.
  */
  operator SUEntitiesRef&();
  operator SUEntitiesRef();

};

} /* namespace CW */
#endif /* Entities_hpp */
