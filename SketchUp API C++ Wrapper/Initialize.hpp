//
//  Initialize.hpp
//  SketchUp API C++ Wrapper
//
//  Created by Tom Kaneko on 04/05/2017.
//  Copyright © 2017 TommyKaneko. All rights reserved.
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
  
  
  size_t get_api_version_major();
  size_t get_api_version_minor();
  
}


#endif /* Initialize_hpp */
