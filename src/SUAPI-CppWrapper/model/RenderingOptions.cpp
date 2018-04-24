//
//  RenderingOptions.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/RenderingOptions.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include <cassert>


namespace CW {
/// @private
RenderingOptions::RenderingOptions()
  : m_rendering_options(SU_INVALID)
{
}

/// @private
RenderingOptions::RenderingOptions(SURenderingOptionsRef ref)
  : m_rendering_options(ref)
{
}

RenderingOptions::~RenderingOptions()
{
}

std::vector<std::string> RenderingOptions::get_keys() const
{
  size_t len = 0, count = 0;
  SUResult res = SU_ERROR_NONE;

  res = SURenderingOptionsGetNumKeys(m_rendering_options, &len);
  assert(res == SU_ERROR_NONE);
  assert(len > 0);

  std::vector<SUStringRef> refs(len, SU_INVALID);
  for(auto i = 0; i < len; i++) {
    res = SUStringCreate(&refs[i]);
    assert(res == SU_ERROR_NONE);
  }
  res = SURenderingOptionsGetKeys(m_rendering_options, len, &refs[0], &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<std::string> keys;
  for(auto ref : refs) {
    keys.push_back(String(ref).std_string());
  }
  return keys;
}

TypedValue RenderingOptions::get_value(const std::string& key) const
{
  SUResult res = SU_ERROR_NONE;
  SUTypedValueRef tval = SU_INVALID;
  SUTypedValueRef* pTval = &tval;
  res = SUTypedValueCreate(pTval);
  assert(res == SU_ERROR_NONE);
  res = SURenderingOptionsGetValue(m_rendering_options, key.c_str(), pTval);
  assert(res == SU_ERROR_NONE); _unused(res);
  return TypedValue(*pTval);
}

bool CW::RenderingOptions::set_value(const std::string& key, const TypedValue& tval)
{
  SUResult res = SU_ERROR_NONE;
  SUTypedValueRef tref = tval.ref();
  res = SURenderingOptionsSetValue(m_rendering_options, key.c_str(), tref);
  if(res == SU_ERROR_NONE)
    return true;
  else
    return false;
}

} // namespace CW
