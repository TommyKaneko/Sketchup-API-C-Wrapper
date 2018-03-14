//
//  OptionsManager.cpp
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
#include <cassert>

#include "SUAPI-CppWrapper/model/OptionsManager.hpp"

namespace CW {

OptionsManager::OptionsManager()
{
}
OptionsManager::OptionsManager(SUOptionsManagerRef ref): m_options_manager(ref)
{}

std::vector<std::string> OptionsManager::get_provider_names() const
{
  size_t num_requested = 0;
  size_t num_returned = 0;
  SUResult res = SUOptionsManagerGetNumOptionsProviders(m_options_manager, &num_requested);
  std::vector<SUStringRef> refs(num_requested);
  for(auto i = 0; i < num_requested; i++) {
    res = SUStringCreate(&refs[i]);
    assert(res == SU_ERROR_NONE);
  }
  res = SUOptionsManagerGetOptionsProviderNames(m_options_manager, num_requested, &refs[0], &num_returned);
  assert(res == SU_ERROR_NONE);
  std::vector<std::string> keys;
  for(auto i = 0; i < refs.size(); i++) {
    keys.push_back(String(refs[i]));
  }
  return keys;
}

OptionsProvider OptionsManager::get_provider(const std::string& name)
{
  SUOptionsProviderRef provider = SU_INVALID;
  SUResult res = SUOptionsManagerGetOptionsProviderByName(m_options_manager, &name[0], &provider);
  assert(res == SU_ERROR_NONE);
  return OptionsProvider(provider);
}


//
// OptionsProvider
//

OptionsProvider::OptionsProvider()
{
}

OptionsProvider::OptionsProvider(SUOptionsProviderRef provider_ref):
  m_options_provider(provider_ref)
{ }

std::vector<std::string> OptionsProvider::keys() const
{
  std::vector<std::string> keys;
  size_t num_requested = 0;
  SUResult res = SUOptionsProviderGetNumKeys(m_options_provider, &num_requested);
  assert(res == SU_ERROR_NONE);
  if(num_requested == 0)
    return keys;

  size_t num_returned = 0;
  std::vector<SUStringRef> refs(num_requested);
  for(auto i = 0; i < num_requested; i++) {
    res = SUStringCreate(&refs[i]);
    assert(res == SU_ERROR_NONE);
  }
  res = SUOptionsProviderGetKeys(m_options_provider, num_requested, &refs[0], &num_returned);
  assert(res == SU_ERROR_NONE);

  for(auto i = 0; i < refs.size(); i++) {
    keys.push_back(String(refs[i]));
  }
  return keys;
}


TypedValue OptionsProvider::get_value(std::string key) const
{
  const char* key_char = key.c_str();
  TypedValue tval;
  SUTypedValueRef *t = tval;
  SUResult res = SUOptionsProviderGetValue(m_options_provider, key_char, t);
  assert(res == SU_ERROR_NONE);
  return tval;
}

bool OptionsProvider::set_value(std::string key, const TypedValue& tval)
{
  SUTypedValueRef t = tval.ref();
  SUResult res = SUOptionsProviderSetValue(m_options_provider, key.c_str(), t);
  if(res == SU_ERROR_NONE)
    return true;
  else
    return false;
}

} /* namespace CW */
