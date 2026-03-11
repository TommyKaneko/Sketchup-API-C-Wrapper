//
//  DynamicComponentInfo.hpp
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

#ifndef DynamicComponentInfo_hpp
#define DynamicComponentInfo_hpp

#include <vector>
#include <SketchUpAPI/model/dynamic_component_info.h>

namespace CW {

// Forward declarations
class DynamicComponentAttribute;

/**
 * @brief C++ wrapper for SUDynamicComponentInfoRef.
 *
 * Contains information about a dynamic component's attributes. Created from
 * a ComponentInstance and must be released when no longer needed.
 *
 * This object is non-copyable and moveable. It owns the underlying ref
 * and releases it on destruction.
 *
 * @see SUDynamicComponentInfoRef
 * @since SketchUp 2016, API 4.0
 */
class DynamicComponentInfo {
  private:
  SUDynamicComponentInfoRef m_dc_info;

  public:
  /**
  * @brief Constructs a NULL DynamicComponentInfo.
  */
  DynamicComponentInfo();

  /**
  * @brief Constructs from an existing SUDynamicComponentInfoRef. Takes ownership.
  * @param dc_info - the SUDynamicComponentInfoRef to wrap (will be released on destruction).
  */
  DynamicComponentInfo(SUDynamicComponentInfoRef dc_info);

  /** @brief Non-copyable. */
  DynamicComponentInfo(const DynamicComponentInfo&) = delete;
  DynamicComponentInfo& operator=(const DynamicComponentInfo&) = delete;

  /** @brief Move constructor. */
  DynamicComponentInfo(DynamicComponentInfo&& other) noexcept;

  /** @brief Move assignment. */
  DynamicComponentInfo& operator=(DynamicComponentInfo&& other) noexcept;

  /** @brief Destructor. Releases the dynamic component info. */
  ~DynamicComponentInfo();

  /**
  * @brief Returns the underlying SUDynamicComponentInfoRef.
  */
  SUDynamicComponentInfoRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null DC info.
  */
  bool operator!() const;

  /**
  * @brief Retrieves the number of dynamic component attributes.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the info is null.
  */
  size_t num_attributes() const;

  /**
  * @brief Retrieves all dynamic component attributes.
  * @since SketchUp 2016, API 4.0
  * @throws std::logic_error if the info is null.
  */
  std::vector<DynamicComponentAttribute> attributes() const;
};

} /* namespace CW */

#endif /* DynamicComponentInfo_hpp */
