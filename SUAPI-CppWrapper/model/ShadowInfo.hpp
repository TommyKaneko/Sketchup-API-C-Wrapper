#pragma once

#include <vector>
#include <string>
#include <SketchUpAPI/model/shadow_info.h>

namespace CW {

class TypedValue;

class ShadowInfo
{
  public:
  ShadowInfo();
  ShadowInfo(SUShadowInfoRef);
  ~ShadowInfo();

  // public member functions

  std::vector<std::string> get_keys();
  TypedValue get_value(const std::string& key);
  bool set_value(const std::string& key, const TypedValue& tval);

  private:
  SUShadowInfoRef m_shadow_info;
}; // class ShadowInfo

} // namespace CW
