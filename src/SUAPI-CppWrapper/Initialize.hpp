//
//  Initialize.hpp
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

#ifndef Initialize_hpp
#define Initialize_hpp

#include <stdio.h>

#include <SketchUpAPI/initialize.h>
namespace CW {
  
  /**
  * The version numbers of the SketchUp application - it must be set prior to using the C++ wrapper, as later versions of SketchUp have added functions.
  */
  extern size_t SU_VERSION_MAJOR;
  extern size_t SU_VERSION_MINOR;
  extern size_t SU_VERSION_BUILD;

  extern size_t SU_API_VERSION_MAJOR;
  extern size_t SU_API_VERSION_MINOR;
  
  /**
  * Equivalent of SUInitialize(), which must be called before the SU API can be used.
  */
  void initialize();
  
  /**
  * Equivalent of SUTerminate(), which must be called after finishing with the API.
  */
  void terminate();
  
  /**
  * Sets the SU_API_VERSION_MAJOR and SU_API_VERSION_MINOR variables.
  */
  void set_api_version();
  
  size_t get_api_version_major();
  size_t get_api_version_minor();
  
}


#endif /* Initialize_hpp */
