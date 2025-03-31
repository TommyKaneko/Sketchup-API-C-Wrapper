//  RubyAPI.hpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2022 Tom Kaneko
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

#ifndef RubyAPI_hpp
#define RubyAPI_hpp

#include <stdio.h>

#include <SketchUpAPI/application/ruby_api.h>

namespace CW {
  // Forward Declarations:
  class Entity;
  class ImageRep;

/**
@brief The SketchUp C API defines 'RUBY_VALUE' - a type matching Ruby's own VALUE type and can be used interchangeably.
*/


/*
* RubyAPI wrapper
*/
class RubyAPI {
  
  public:

  
  /**
  @brief Converts a Cpp entity to a Ruby API entity.
  @since SketchUp 2020.2, API 8.2
  @param  entity - Entity object to covert to Ruby
  @return The retrieved Ruby API entity reference.  Returned as a ruby value
  */
  static RUBY_VALUE entity_to_ruby(const Entity& entity);
  

  /**
  @brief Converts a Ruby API entity to a Cpp API entity.
  @since SketchUp 2020.2, API 8.2
  @param  ruby_entity  The Ruby API entity reference.
  @return The retrieved Cpp API entity reference.
  */
  static Entity entity_from_ruby(const RUBY_VALUE& ruby_entity);

  /**
  @brief Converts a Cpp API entity to a Ruby API imagerep.
  @note Ruby takes ownership of the object it should not be released by
        SUImageRepRelease. It will be released when Ruby garbage collects the
        Ruby references to it.
  @since SketchUp 2020.2, API 8.2
  @param  imagerep       The Cpp API imagerep reference.
  @return The retrieved Ruby API imagerep reference.
  */
  static RUBY_VALUE imagerep_to_ruby(const ImageRep& imagerep);

  /**
  @brief Converts a Ruby API imagerep to a Cpp API imagerep.
  @note Ruby retains the ownership of the object it should not be released by
        SUImageRepRelease.
  @warning The returned C API reference points to an object that is owned by the
           Ruby runtime and it may be garbage collected. To avoid this, ensure
           that the Ruby API reference has a longer lifetime than the C API
           reference.
  @since SketchUp 2020.2, API 8.2
  @param  ruby_imagerep  The Ruby API imagerep reference.
  @return The retrieved C API imagerep reference.
  */
  static ImageRep imagerep_from_ruby(const RUBY_VALUE& ruby_entity);

};

} /* namespace CW */

#endif /* RubyAPI_hpp */
