//
//  Initialize.cpp
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

#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW {
  
size_t SU_VERSION_MAJOR = 18;
size_t SU_VERSION_MINOR = 0;
size_t SU_VERSION_BUILD = 0;

size_t SU_API_VERSION_MAJOR = 1;
size_t SU_API_VERSION_MINOR = 0;

void initialize() {
  SUInitialize();
  set_api_version();
}


void terminate() {
  SUTerminate();
}


void set_api_version() {
  SUGetAPIVersion(&SU_API_VERSION_MAJOR, &SU_API_VERSION_MINOR);
}

size_t get_api_version_major() {
  size_t major;
  size_t minor;
  SUGetAPIVersion(&major, &minor);
  return major;
}

size_t get_api_version_minor() {
  size_t major;
  size_t minor;
  SUGetAPIVersion(&major, &minor);
  return minor;
}
  
}
