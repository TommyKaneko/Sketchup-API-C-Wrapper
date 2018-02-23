#include "SUAPI-CppWrapper/model/ShadowInfo.hpp"
#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include <cassert>


namespace CW {

/// @private
ShadowInfo::ShadowInfo()
  : m_shadow_info(SU_INVALID)
{
}

/// @private
CW::ShadowInfo::ShadowInfo(SUShadowInfoRef ref)
  : m_shadow_info(ref)
{
}

ShadowInfo::~ShadowInfo()
{
}


std::vector<std::string> ShadowInfo::get_keys()
{
  size_t len = 0, count = 0;
  SUResult res = SU_ERROR_NONE;

  res = SUShadowInfoGetNumKeys(m_shadow_info, &len);
  assert(res == SU_ERROR_NONE);
  assert(len > 0);

  std::vector<SUStringRef> refs(len, SU_INVALID);
  for(auto i = 0; i < len; i++) {
    res = SUStringCreate(&refs[i]);
    assert(red == SU_ERROR_NONE);
  }
  res = SUShadowInfoGetKeys(m_shadow_info, len, &refs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<std::string> keys;
  for(auto ref : refs) {
    keys.push_back(String(ref).std_string());
  }
  return keys;
}

TypedValue ShadowInfo::get_value(const std::string& key)
{
  SUResult res = SU_ERROR_NONE;
  SUTypedValueRef tval = SU_INVALID;
  SUTypedValueRef* pTval = &tval;
  res = SUTypedValueCreate(pTval);
  assert(res == SU_ERROR_NONE);
  res = SUShadowInfoGetValue(m_shadow_info, key.c_str(), pTval);
  assert(res == SU_ERROR_NONE);
  return TypedValue(*pTval);
}

bool ShadowInfo::set_value(const std::string& key, const TypedValue& tval)
{
  SUResult res = SU_ERROR_NONE;
  SUTypedValueRef tref = tval.ref();
  res = SUShadowInfoSetValue(m_shadow_info, key.c_str(), tref);
  if(res == SU_ERROR_NONE)
    return true;
  else
    return false;
}

} // namespace CW
