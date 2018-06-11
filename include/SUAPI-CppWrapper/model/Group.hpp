//
//  Group.hpp
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

#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>

#include <SketchUpAPI/model/group.h>

#include "SUAPI-CppWrapper/model/ComponentInstance.hpp"

namespace CW {

// Forward Declarations
class Entities;
class Transformation;
class ComponentDefinition;
class String;

class Group :public ComponentInstance {
  private:
  /**
  * Create new SUGroupRef object
  */
  static SUGroupRef create_group();
  
  static SUGroupRef copy_reference(const Group& other);

  public:
  /**
  * Construct a new, empty Group object.
  */
  Group();
  
  /**
  * Construct a Group from an existing SUGroupRef object.  Groups must always be attached to a parent (there is no release function for Groups)
  */
  Group(SUGroupRef group, bool attached = true);
  
  /**
  * Construct a Group from instance object.  If the instance is not a group, you will get errors, as the constructor does not check for this.
  */
  Group(const ComponentInstance& instance);
  
  /** Copy constructor */
  Group(const Group& other);
  
  /** Destructor */
  ~Group();

  /** Copy assignment operator */
  Group& operator=(const Group& other);

  /**
  * Returns the raw SUGroupRef object.
  */
  SUGroupRef ref() const;

  /*
  * The class object can be converted to a SUGroupRef.
  */
  operator SUGroupRef() const;
  operator SUGroupRef*();
  
  /**
  * Return the ComponentDefinition of the Group.
  */
  ComponentDefinition definition() const;
  
  /**
  * Return the Entities object of the Group.
  */
  Entities entities() const;
  
  /**
  * Return the name of the Group.
  */
  String name() const;
  
  /**
  * Set the name of this Group.
  */
  void name(const String& string);
  
  /**
  * Return the Transformation object of the group.
  */
  Transformation transformation() const;

  /**
  * Set the Transformation of the group.
  */
  void transformation(const Transformation& transform);
  
};

} /* namespace CW */

#endif /* Group_hpp */
