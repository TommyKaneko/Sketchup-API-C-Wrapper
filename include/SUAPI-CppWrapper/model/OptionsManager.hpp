//
//  OptionsManager.hpp
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

#ifndef OptionsManager_hpp
#define OptionsManager_hpp

#include <string>
#include <vector>

#include <SketchUpAPI/model/options_manager.h>
#include <SketchUpAPI/model/options_provider.h>
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"


namespace CW {

// Forward Declarations:
//class Model;

/**
 * @brief Wrapper for SUOptionsProviderRef, providing access to a named
 *        set of key-value option pairs.
 *
 * An OptionsProvider is obtained from an OptionsManager and exposes
 * the keys and typed values for a particular provider
 * (e.g. "PageOptions", "UnitsOptions", etc.).
 *
 * @see OptionsManager
 * @see https://extensions.sketchup.com/developer_center/sketchup_c_api/
 */
class OptionsProvider {
  private:
  SUOptionsProviderRef m_options_provider;
  public:
  /** @brief Default constructor – creates an invalid OptionsProvider. */
  OptionsProvider();

  /**
   * @brief Constructs an OptionsProvider wrapping a raw API ref.
   * @param ref the SUOptionsProviderRef to wrap.
   */
  OptionsProvider(SUOptionsProviderRef ref);

  /**
   * @brief Returns all key names for this provider.
   * @return a vector of key name strings.
   */
  std::vector<std::string> keys() const;

  /**
   * @brief Returns the value for the given key.
   * @param key the key name to look up.
   * @return the TypedValue associated with the key.
   */
  TypedValue get_value(std::string key) const;

  /**
   * @brief Sets the value for the given key.
   * @param key the key name to set.
   * @param tval the TypedValue to assign.
   * @return true on success, false on failure.
   */
  bool set_value(std::string key, const TypedValue& tval);
};

/**
 * @brief Wrapper for SUOptionsManagerRef, providing access to named
 *        option providers in a SketchUp model.
 *
 * The OptionsManager is obtained from a Model and gives access to
 * providers such as "PageOptions", "UnitsOptions", "NamedOptions", etc.
 *
 * @see OptionsProvider
 * @see https://extensions.sketchup.com/developer_center/sketchup_c_api/
 */
class OptionsManager {
  private:
  SUOptionsManagerRef m_options_manager;

  public:
  /** @brief Default constructor – creates an invalid OptionsManager. */
  OptionsManager();

  /**
   * @brief Constructs an OptionsManager wrapping a raw API ref.
   * @param options_manager the SUOptionsManagerRef to wrap.
   */
  OptionsManager(SUOptionsManagerRef options_manager);

  /**
   * @brief Returns the names of all option providers.
   * @return a vector of provider name strings.
   */
  std::vector<std::string> get_provider_names() const;

  /**
   * @brief Returns the OptionsProvider with the given name.
   * @param name the provider name (e.g. "PageOptions").
   * @return the OptionsProvider for the given name.
   */
  OptionsProvider get_provider(const std::string& name);

};


} /* namespace CW */

#endif /* OptionsManager_hpp */
