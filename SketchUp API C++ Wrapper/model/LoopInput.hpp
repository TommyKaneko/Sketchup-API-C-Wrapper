//
//  LoopInput.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 08/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef LoopInput_hpp
#define LoopInput_hpp

#include <stdio.h>
#include <vector>

#include <SketchUpAPI/model/geometry_input.h>

namespace CW {
// Forward declarations
class GeometryInput;
class Edge;
class Curve;
class Material;
class Layer;


class LoopInput {
	private:
  SULoopInputRef m_loop_input;
  size_t m_edge_num = 0;
  
  //std::vector<Edge> m_edges;
  //std::vector<Curve> m_curves;
  
  static SULoopInputRef create_loop_input_ref();
  
  public:
	/**
  * Create empty LoopInput object
  */
  LoopInput();
  
  /**
  * Create LoopInput object from preexisting SULoopInputRef object
  */
  LoopInput(SULoopInputRef loop_input);
  
  /**
  * Create LoopInput object from vector of edges that can form a loop.
  * @param loop_edges - vector of edges from which properties will be copied into the new loop input.
  * @param vertex_index - 0 by default.  This is the first index of the vertex to be added to the loop.  Only when using SUGeometryInputRef object would you use an index higher than 0.
  */
  LoopInput(const std::vector<Edge>& loop_edges, size_t vertex_index = 0);

	/** Copy constructor */
	LoopInput(const LoopInput& other);

  ~LoopInput();
  
  /** Copy assignment operator **/
  LoopInput& operator=(const LoopInput& other);
  
  /*
  * Returns the stored SULoopInputRef object.
  */
  SULoopInputRef ref() const;

  /*
  * The class object can be converted to a SULoopInputRef without loss of data.
  */
  operator SULoopInputRef() const;
  operator SULoopInputRef*();
  
  /**
  * Indicates whether a LoopInput is valid, or empty.
  * @return true if valid and has three or more vertices (the minimum for a loop).  Returns false if invalid or has two or less vertices.
  */
  operator bool() const;
  
	/**
  * Adds a vertex index to the loop. LoopInput does not hold the point locations of vertices.  It holds information on the edges that form the loop.
  * @param index - the vertex index of the loop.  When used with SUCreateFace(), it would be a zero-based index.  When used with SUGeometryInputAddFace(), the vertex index would need to correspond to specific indices of the vertices that have been added to SUGeometryInputRef object.
  */
  LoopInput& add_vertex_index(const size_t index);

  LoopInput& set_edge_hidden(const size_t edge_index, const bool hidden);
  LoopInput& set_edge_soft(const size_t edge_index, const bool soft);
  LoopInput& set_edge_smooth(const size_t edge_index, const bool smooth);
  LoopInput& set_edge_material(const size_t edge_index, const Material& material);
  LoopInput& set_edge_layer(const size_t edge_index, const Layer& layer);
  


  /**
  * Creates a LoopInput object, which will store the properties of the sequence of Edge objects.
  */
  //LoopInput(std::vector<Edge> edges);

  /*
  * Creates a Loop object which is used as a LoopInputRef object (i.e. it is not attached to a Face yet)
  * @param vector of SUPoint3D objects, tracing the outline of the loop.
  */
  /*
  * Returns the stored m_edges array.
  */
  //std::vector<Edge> get_edges();
  
  /*
  * Returns an array of vertices that represent the loop.
  */
  //std::vector<Point3D> get_vertices();
  
  /*
  * Adds the loop to the given 
  */
  // TODO: det
  //SU_RESULT add_to_geometry_input(GeometryInput &geom_input);
  
  /*
  * Adds a series of vertices that represent the loop.
  */
  //LoopInput& add_vertices(std::vector<Point3D> points);
  
  /*
  * Adds an edge on the end of the Loop
  */
  /*
  LoopInput& add_edge(Edge edge);
  LoopInput& add_edges(std::vector<Edge> edges);
  */
  //LoopInput& add_curve(Curve curve);
  //LoopInput& add_curves(std::vector<Curve> curves);

};

} /* namespace CW */

#endif /* LoopInput_hpp */
