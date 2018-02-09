#include "RenderingOptions.hpp"
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
  std::vector<std::string> keys;
  SUResult res = SU_ERROR_NONE;

  res = SURenderingOptionsGetNumKeys(m_rendering_options, &len);
  assert(res == SU_ERROR_NONE);
  assert(len > 0);

  std::vector<SUStringRef> refs(len);
  for(auto i = 0; i < len; i++) {
    assert(SUStringCreate(&refs[i]) == SU_ERROR_NONE);
  }
  res = SURenderingOptionsGetKeys(m_rendering_options, len, &refs[0], &count);
  assert(res == SU_ERROR_NONE);
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
  assert(res == SU_ERROR_NONE);
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