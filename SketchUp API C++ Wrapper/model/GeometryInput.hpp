//
//  GeometryInput.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 04/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef GeometryInput_hpp
#define GeometryInput_hpp

#include <stdio.h>
#include <algorithm>
#include <vector>

#include "Face.hpp"
#include "Edge.hpp"
#include "LoopInput.hpp"
#include "MaterialInput.hpp"

#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/model/entities.h>

namespace CW {

// Forward declarations:
class Entities;
  
/**
* Geometry Input class is an abstraction of Sketchup C API's SUGeometryInputRef object.  It allows a much quicker way for programmers to build Sketchup geometry within this class, before exporting it into a built SUGeometryInputRef object.
* See below for an example of GeometryInput operates:
GeometryInput geom_input;
SUEntitiesRef entities; // This is the entities object that we wish to add the geometry to.
std::vector<SUPoint3D> outer_loop{SUPoint3D(0.0,0.0,0.0), SUPoint3D(1.0,0.0,0.0), SUPoint3D(0.0,1.0,0.0)};
std::vector<std::vector<SUPoint3D>> inner_loops{{SUPoint3D(0.1,0.1,0.0), SUPoint3D(0.1,0.2,0.0), SUPoint3D(0.2,0.3,0.0)}};
GIFace* face = geom_input.add_face(outer_loop, inner_loops); // GeometryInput handles the interface with Sketchup C API when creating geometry.
SU_RESULT result = geom_input.output(entities); // the geometry must be output into a SUEntitiesRef object.

*/
class GeometryInput {
	private:
  SUGeometryInputRef m_geometry_input;
  
  /**
  * As Geometry Input does not always copy all information about an Entity to an Entities object, things like Attributes must be copied into the new Geometry after SUEntitiesFill() is called.  So it is necessary to store information about each entity that was added below, to allow this.
  */
  std::vector<Face> m_faces;
  //std::vector<std::pair<size_t, Face>> m_faces;
  //std::vector<std::pair<size_t, Edge>> m_edges;
  //std::vector<std::vector<Edge*>> m_curves; // curves are simply collections of Edges.
  
  size_t m_vertex_index = 0;
 	/**
  * Creates and returns new SUGeometryInputRef object, used for initializing m_geometry_input. The SUGeometryInputRef object must be released with SUGeometryInputRelease() before this class is destroyed.
  */
  static SUGeometryInputRef create_geometry_input();


	/*
  * Adds a constructed LoopInput object into the GeometryInput object.
  * @see add_face() for the details on how this is used.
  */
	SU_RESULT add_loop(LoopInput &loop);

  //std::vector<SUPoint3D> m_vertices;
  /**
  * Faces are stored as a series of nested vectors of vertices.
  * 1st level - array of faces
  * 2nd level - array of loops within the face.  The first loop is the outer loop.  All subsequent arrays represent inner loops.
  * 3rd level - array of indexes that refer to the vertices in the m_vertices array.
  */
  //std::vector<std::vector<std::vector<size_t>>> m_faces;

  /**
  * Edges are stored a series of nested vectors of vertices.
  * 1st level - array of edges/curves
  * 2nd level - array of indexes that refer to the vertices in the m_vertices array, which represent edges/curves (curves have more than two vertices)
  */
  //std::vector<std::vector<size_t>> m_edges;
  
  public:
  GeometryInput();
  ~GeometryInput();
 	
  /**
  * Returns Raw SUGeometryInputRef that is stored.
  */
  SUGeometryInputRef input_ref() {return m_geometry_input;}
 
  /**
  * Returns Face objects stored in this object.
  */
  std::vector<Face> faces() {return m_faces;}
  
  
  /**
  * Adds a vertex to the Input object, and returns the index of the added Vertex.
  * @param vertex Point3D object.
  * @return index of the vertex that was added.
  */
  size_t add_vertex(Point3D vertex);
	std::vector<size_t> add_vertices(std::vector<Point3D> vertices);
  
  /**
  * Adds a face to the Geometry Input object.
  * @return index to the added face.
  */
  size_t add_face(Face &face);
  /*
  Face add_face(const std::vector<Loop> &loops);
  Face add_face(const std::vector<SUPoint3D> &outer_loop);
  Face add_face(const std::vector<Edge*> &outer_edges);
  */
  
  /**
  * Adds an Edge to the Geometry Input object.
  * @return index to the added edge.
  */
  /*
  // TODO you can't add edges (I don't think - check this!)
  size_t add_line(Edge &edge);
  size_t add_edge(Edge &edge) { return add_line(edge); }
  Edge add_line(const SUPoint3D start, const SUPoint3D end);
  Edge add_edge(const SUPoint3D start, const SUPoint3D end);

  std::vector<Edge*> add_edges(std::vector<Edge> &edges);
  std::vector<Edge*> add_edges(const std::vector<SUPoint3D> &points);

  std::vector<Edge*> add_curve(const std::vector<SUPoint3D> &points);
  */
};

/**
class SU {
	public:
  static SUFaceRef copyFace(const SUFaceRef &old_face) {
    SUFaceRef new_face = SU_INVALID;
    size_t num_vertices = 0;
    SU_RESULT res = SUFaceGetNumVertices(old_face, &num_vertices);
    assert(res == SU_ERROR_NONE);
    // Build vertices
    SUVertexRef vertices[num_vertices];
    size_t num = 0;
    res = SUFaceGetVertices(old_face, num_vertices, &vertices[0], &num);
    SUPoint3D points[num_vertices];
    for (size_t i = 0; i < num_vertices; ++i) {
    	res = SUVertexGetPosition(vertices[i], &points[i]);
    	assert(res == SU_ERROR_NONE);
    }
    // Build loops
    SULoopInputRef loop_input = SU_INVALID;
    res = SULoopInputCreate(&loop_input);
    // TODO - There may be another approach using Geometry Input which is more fruitful
    
    SULoopInputRef outer_loop;
    SUFaceCreate(&new_face, const struct SUPoint3D vertices3d[], SULoopInputRef* outer_loop);
  }
  // TODO
	static SUGeometryInputRef fill_geometry();
  
};
*/

} /* namespace CW */

#endif /* GeometryInput_hpp */
