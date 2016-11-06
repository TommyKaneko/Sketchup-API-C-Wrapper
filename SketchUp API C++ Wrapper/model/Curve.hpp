//
//  Curve.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 08/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Curve_hpp
#define Curve_hpp

#include <stdio.h>
#include <vector>

#include <SketchUpAPI/model/curve.h>

#include "Edge.hpp"

namespace CW {

class Curve :public Entity {
	private:
  SUCurveRef m_curve;
  SUCurveType m_curve_type;
  SU_RESULT m_create_result;
  
  static SUCurveRef create_curve(std::vector<Edge> edges, SU_RESULT &result);
  
  public:
  /**
  * A curve is created from an array of Edge objects.
  * @param edges - a vector array of Edge objects.
  * @param curve_type (optional) the type of curve
  */
  Curve(std::vector<Edge> edges, SUCurveType curve_type = SUCurveType_Simple);
  Curve(SUCurveRef curve, SUCurveType curve_type);
  
  ~Curve();
  
  /**
  * Returns SUCurveRef object of this curve.
  */
  SUCurveRef ref();
  
  /**
  * Returns the series of Edge objects in the Curve.
  */
  std::vector<Edge> get_edges() const;

  /**
  * Returns the type of Curve.
  */
  SUCurveType get_type();
	
  /*
  * Returns the SU_RESULT object form when the Curve was created.
  * @return	* SU_ERROR_NONE on success
            * SU_ERROR_NULL_POINTER_INPUT if edges is NULL
            * SU_ERROR_OUT_OF_RANGE if len is 0
            * SU_ERROR_NULL_POINTER_OUTPUT if curve is NULL
            * SU_ERROR_OVERWRITE_VALID if curve already references a valid object
            * SU_ERROR_GENERIC if edge array contains an invalid edge, if the edges in the array are not connected, if any of the edges are associated with a face object, or the edges describe a loop
  */
	SU_RESULT get_result() const;
  
  /*
  * Returns whether the class is a valid object.
  */
  operator bool() const;
  /**
  * NOT operator.  Checks if the CurveRef is valid.
  * @return true if the curve is invalid
  */
	bool operator!() const;
  
};

} /* namespace CW */

#endif /* Curve_hpp */
