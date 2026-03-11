//
//  Schema.cpp
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

#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Schema.hpp"

#include <cassert>
#include <stdexcept>

#include <SketchUpAPI/model/schema_type.h>
#include "SUAPI-CppWrapper/String.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
Schema::Schema():
  m_schema(SU_INVALID)
{}


Schema::Schema(SUSchemaRef schema):
  m_schema(schema)
{}


/*******************
** Public Methods **
********************/
SUSchemaRef Schema::ref() const {
  return m_schema;
}


bool Schema::operator!() const {
  return SUIsInvalid(m_schema);
}


String Schema::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::Schema::name(): Schema is null");
  }
  String name;
  SUStringRef name_ref = name.ref();
  SUResult res = SUSchemaGetSchemaName(m_schema, &name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return name;
}


SUSchemaTypeRef Schema::get_schema_type(const std::string& schema_type_name) const {
  if (!(*this)) {
    throw std::logic_error("CW::Schema::get_schema_type(): Schema is null");
  }
  SUSchemaTypeRef type_ref = SU_INVALID;
  SUResult res = SUSchemaGetSchemaType(m_schema, schema_type_name.c_str(), &type_ref);
  if (res != SU_ERROR_NONE) {
    return SU_INVALID;
  }
  return type_ref;
}

} /* namespace CW */
