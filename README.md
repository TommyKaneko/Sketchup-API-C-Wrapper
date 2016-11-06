C++ Wrapper for SketchUp C API

================
= Introduction =
================
SketchUp C API is made for the C programming language: http://extensions.sketchup.com/developer_center/sketchup_c_api/sketchup/index.html

For C++ programmers, the non-object-orientated nature of the C API means simple operations for retrieving objects through is time consuming.  This project provides a wrapper class for every SketchUp C API object that considerably shortens the amount of code required to perform simple tasks.

For example, to get an array of faces from an entities object, you could write this:
  SUEntitiesRef entities = SU_INVALID;
  SUModelGetEntities(model, &entities);
  size_t faceCount = 0;
  SUEntitiesGetNumFaces(entities, &faceCount);
  std::vector<SUFaceRef> faces[faceCount];
  SUEntitiesGetFaces(entities, faceCount, &faces[0], &faceCount);

With the C++ Wrapper, to achieve the above, you just need to write this:
	#include <CWrapper/model/Model.hpp>
	#include <CWrapper/model/Face.hpp>
	std::vector<CW::Face> faces = CW::Model(model).get_entities().get_faces();

Added to this benefit, the wrapper classes have helper functions to help with geometric manipulations, much in the style of the SketchUp Ruby API

======================
= Project Objectives =
======================
The aims of this set of wrapper classes are:
- to make the SketchUp C API quicker and easier to use, from a C++ programmer's perspective.
- to provide all methods in the SketchUp C API, but in a simplified form.
- to provide all methods that are given in the equivalent classes in the SketchUp Ruby API (http://www.sketchup.com/intl/en/developer/docs/index ), if it is possible to deliver through the SketchUp C API.


==========================
= Programming Principles =
==========================

DATA STRUCTURE
Each C++ Wrapper class object holds its equivalent C API class object as a member variable (ie - CW::Face holds SUFaceRef).  Further variables should be kept to a minimum, if at all.

RELEASING OBJECTS
With the use of the C++ Wrapper, the SketchUp API should "feel" like a native C++ API.  This means the idea of "releasing" objects that are not attached to a model object, for example, should not have to be considered by the programmer.  Instead, the wrapper class will deal with releasing of an object when the object goes out of scope.

The only exception to this rule is if the wrapped object (CW::Face, for example), is converted to a regular C object ( SUFaceRef, for example), in which case it can be assumed that the programmer is taking charge of the C object and will be dealing with any releasing.

METHOD NAMES
Method names take inspiration mostly from the SketchUp Ruby API.  That said, these are not wholy consistent, and some liberties have been taken to make the method names make sense.

