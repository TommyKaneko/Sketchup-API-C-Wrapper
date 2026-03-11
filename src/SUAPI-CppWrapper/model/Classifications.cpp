//
//  Classifications.cpp
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

#include "SUAPI-CppWrapper/model/Classifications.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/model/Schema.hpp"

namespace CW {

/******************************
** Constructors / Destructor **
*******************************/
Classifications::Classifications():
  m_classifications(SU_INVALID)
{}


Classifications::Classifications(SUClassificationsRef classifications):
  m_classifications(classifications)
{}


/*******************
** Public Methods **
********************/
SUClassificationsRef Classifications::ref() const {
  return m_classifications;
}


bool Classifications::operator!() const {
  return SUIsInvalid(m_classifications);
}


void Classifications::load_schema(const std::string& schema_file_name) {
  if (!(*this)) {
    throw std::logic_error("CW::Classifications::load_schema(): Classifications is null");
  }
  SUResult res = SUClassificationsLoadSchema(m_classifications, schema_file_name.c_str());
  if (res != SU_ERROR_NONE) {
    throw std::invalid_argument("CW::Classifications::load_schema(): Invalid schema file path");
  }
}


Schema Classifications::get_schema(const std::string& schema_name) const {
  if (!(*this)) {
    throw std::logic_error("CW::Classifications::get_schema(): Classifications is null");
  }
  SUSchemaRef schema = SU_INVALID;
  SUResult res = SUClassificationsGetSchema(m_classifications, schema_name.c_str(), &schema);
  if (res != SU_ERROR_NONE) {
    return Schema(); // Not found
  }
  return Schema(schema);
}


size_t Classifications::num_schemas() const {
  if (!(*this)) {
    throw std::logic_error("CW::Classifications::num_schemas(): Classifications is null");
  }
  size_t count = 0;
  SUResult res = SUClassificationsGetNumSchemas(m_classifications, &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  return count;
}


std::vector<Schema> Classifications::schemas() const {
  if (!(*this)) {
    throw std::logic_error("CW::Classifications::schemas(): Classifications is null");
  }
  size_t count = 0;
  SUResult res = SUClassificationsGetNumSchemas(m_classifications, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Schema>();
  }
  std::vector<SUSchemaRef> refs(count, SU_INVALID);
  res = SUClassificationsGetSchemas(m_classifications, count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Schema> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.push_back(Schema(refs[i]));
  }
  return result;
}

} /* namespace CW */
