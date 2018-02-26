#pragma once

#define SU(api_function_call) {\
  auto su_api_result = api_function_call;\
  assert(SU_ERROR_NONE == su_api_result);\
}\

