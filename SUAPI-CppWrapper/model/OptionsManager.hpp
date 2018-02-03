//
//  OptionsManager.hpp
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

#ifndef OptionsManager_hpp
#define OptionsManager_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include <SketchUpAPI/model/options_manager.h>
#include <SketchUpAPI/model/options_provider.h>
#include "SUAPI-CppWrapper/String.hpp"
//#include "SUAPI-CppWrapper/model/TypedValue.hpp"


namespace CW {

// Forward Declarations:
//class Model;

class OptionsProvider {
  private:
  SUOptionsProviderRef m_options_provider;
  public:
  OptionsProvider();
  OptionsProvider(SUOptionsProviderRef);
  std::vector<std::string> keys() const;
  TypedValue get_value(std::string key) const;
  bool set_value(std::string key, const TypedValue& tval);
};

class OptionsManager {
  private:
  SUOptionsManagerRef m_options_manager;

  public:
  OptionsManager();
  OptionsManager(SUOptionsManagerRef options_manager);

  // Returns a list of the names of OptionProviders
  std::vector<std::string> get_provider_names() const;

  // Return an OptionsProvider by name
  OptionsProvider get_provider(const std::string&);

};


} /* namespace CW */

#endif /* OptionsManager_hpp */
