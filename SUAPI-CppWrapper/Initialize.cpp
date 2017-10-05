//
//  Initialize.cpp
//  SketchUp API C++ Wrapper
//
//  Created by Tom Kaneko on 04/05/2017.
//  Copyright © 2017 TommyKaneko. All rights reserved.
//

#include "SUAPI-CppWrapper/Initialize.hpp"

namespace CW {
  
extern size_t SU_VERSION_MAJOR = 15;
extern size_t SU_VERSION_MINOR = 0;
extern size_t SU_VERSION_BUILD = 0;

extern size_t SU_API_VERSION_MAJOR = 1;
extern size_t SU_API_VERSION_MINOR = 0;

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
