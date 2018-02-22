#pragma once

#include <vector>
#include <string>

#include <SketchUpAPI/model/rendering_options.h>

namespace CW {

class TypedValue;

class RenderingOptions
{
  public:
  RenderingOptions();
  RenderingOptions(SURenderingOptionsRef);
  ~RenderingOptions();

  // public member functions

  std::vector<std::string> get_keys() const;
  TypedValue get_value(const std::string& key) const;
  bool set_value(const std::string& key, const TypedValue& tval);

  private:
  SURenderingOptionsRef m_rendering_options;

}; // class RenderingOptions

} // namespace CW
