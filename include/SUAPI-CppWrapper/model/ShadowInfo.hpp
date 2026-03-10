//
//  ShadowInfo.hpp
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

#ifndef ShadowInfo_hpp
#define ShadowInfo_hpp

#include <vector>
#include <string>
#include <SketchUpAPI/model/shadow_info.h>

namespace CW {

class TypedValue;

/**
 * @brief Wrapper for SUShadowInfoRef, providing access to the
 *        shadow settings of a SketchUp model.
 *
 * Shadow info controls parameters such as shadow time, date,
 * display settings, and light/dark intensities.
 * Options are accessed by key name and stored as TypedValue objects.
 *
 * @see https://extensions.sketchup.com/developer_center/sketchup_c_api/
 */
class ShadowInfo
{
  public:
  /** @brief Default constructor – creates an invalid ShadowInfo. */
  ShadowInfo();

  /**
   * @brief Constructs a ShadowInfo wrapping a raw API ref.
   * @param ref the SUShadowInfoRef to wrap.
   */
  ShadowInfo(SUShadowInfoRef ref);

  /** @brief Destructor. */
  ~ShadowInfo();

  /**
   * @brief Returns all shadow info key names.
   * @return a vector of key name strings.
   */
  std::vector<std::string> get_keys();

  /**
   * @brief Returns the value for the given shadow info key.
   * @param key the key name to look up.
   * @return the TypedValue associated with the key.
   */
  TypedValue get_value(const std::string& key);

  /**
   * @brief Sets the value for the given shadow info key.
   * @param key the key name to set.
   * @param tval the TypedValue to assign.
   * @return true on success, false on failure.
   */
  bool set_value(const std::string& key, const TypedValue& tval);

  private:
  SUShadowInfoRef m_shadow_info;
}; // class ShadowInfo

} // namespace CW

#endif