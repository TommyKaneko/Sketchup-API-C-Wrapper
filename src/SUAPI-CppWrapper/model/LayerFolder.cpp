//
//  LayerFolder.cpp
//
// Sketchup C++ Wrapper for C API
// MIT License
//
// Copyright (c) 2026 Tom Kaneko
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

#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/LayerFolder.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Model.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SULayerFolderRef LayerFolder::create_layer_folder(const std::string& name) {
  SULayerFolderRef folder = SU_INVALID;
  SUResult res = SULayerFolderCreate(&folder, name.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
  return folder;
}


SULayerFolderRef LayerFolder::copy_reference(const LayerFolder& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  // Create a new folder with the same name
  SUStringRef name_ref = SU_INVALID;
  SUStringCreate(&name_ref);
  SULayerFolderGetName(other.ref(), &name_ref);
  size_t length = 0;
  SUStringGetUTF8Length(name_ref, &length);
  std::string name(length, '\0');
  SUStringGetUTF8(name_ref, length + 1, &name[0], &length);
  SUStringRelease(&name_ref);
  return create_layer_folder(name);
}


/******************************
** Constructors / Destructor **
*******************************/
LayerFolder::LayerFolder():
  Entity(SU_INVALID, true)
{}


LayerFolder::LayerFolder(const std::string& name):
  Entity(SULayerFolderToEntity(create_layer_folder(name)), false)
{}


LayerFolder::LayerFolder(SULayerFolderRef layer_folder, bool attached):
  Entity(SULayerFolderToEntity(layer_folder), attached)
{}


LayerFolder::LayerFolder(const LayerFolder& other):
  Entity(other, SULayerFolderToEntity(copy_reference(other)))
{}


LayerFolder::~LayerFolder() {
  if (!m_attached && SUIsValid(m_entity)) {
    SULayerFolderRef folder = this->ref();
    SUResult res = SULayerFolderRelease(&folder);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


LayerFolder& LayerFolder::operator=(const LayerFolder& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SULayerFolderRef folder = this->ref();
    SUResult res = SULayerFolderRelease(&folder);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SULayerFolderToEntity(copy_reference(other));
  Entity::operator=(other);
  return *this;
}


/*******************
** Public Methods **
********************/
SULayerFolderRef LayerFolder::ref() const {
  return SULayerFolderFromEntity(m_entity);
}


bool LayerFolder::operator!() const {
  return SUIsInvalid(m_entity);
}

LayerFolder LayerFolder::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::copy(): LayerFolder is null");
  }
  // Create a new detached folder with the same name
  std::string folder_name = std::string(this->name());
  LayerFolder new_folder(folder_name);
  return new_folder;
}


String LayerFolder::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::name(): LayerFolder is null");
  }
  SUStringRef name_ref = SU_INVALID;
  SUStringCreate(&name_ref);
  SUResult res = SULayerFolderGetName(this->ref(), &name_ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return String(name_ref);
}


void LayerFolder::name(const std::string& name) {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::name(): LayerFolder is null");
  }
  if (name.empty()) {
    throw std::invalid_argument("CW::LayerFolder::name(): name must be non-empty");
  }
  SUResult res = SULayerFolderSetName(this->ref(), name.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool LayerFolder::visible() const {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::visible(): LayerFolder is null");
  }
  bool vis;
  SUResult res = SULayerFolderGetVisibility(this->ref(), &vis);
  assert(res == SU_ERROR_NONE); _unused(res);
  return vis;
}


void LayerFolder::visible(bool visible) {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::visible(): LayerFolder is null");
  }
  SUResult res = SULayerFolderSetVisibility(this->ref(), visible);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool LayerFolder::visible_on_new_scenes() const {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::visible_on_new_scenes(): LayerFolder is null");
  }
  bool vis;
  SUResult res = SULayerGroupGetVisibleOnNewScenes(this->ref(), &vis);
  assert(res == SU_ERROR_NONE); _unused(res);
  return vis;
}


void LayerFolder::visible_on_new_scenes(bool visible) {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::visible_on_new_scenes(): LayerFolder is null");
  }
  SUResult res = SULayerGroupSetVisibleOnNewScenes(this->ref(), visible);
  assert(res == SU_ERROR_NONE); _unused(res);
}


void LayerFolder::add_layer(Layer& layer) {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::add_layer(): LayerFolder is null");
  }
  // SU API fails silently if the layer is not attached to the model already.
  if (layer.attached() == false) {
    throw std::invalid_argument("CW::LayerFolder::add_layer(): layer must be attached to a model");
  }
  else if (layer.model() != this->model()) {
    throw std::invalid_argument("CW::LayerFolder::add_layer(): layer must belong to the same model as the layer folder");
  }
  SUResult res = SULayerFolderAddLayer(this->ref(), layer.ref());
  switch (res) {
    case SU_ERROR_INVALID_INPUT: {
      throw std::invalid_argument("CW::LayerFolder::add_layer(): layer is not a valid object");
    }
    case SU_ERROR_INVALID_ARGUMENT: {
      throw std::invalid_argument("CW::LayerFolder::add_layer(): layer can not be added to, or is already a member of, layer_folder");
    }
    default: {
      assert(res == SU_ERROR_NONE); _unused(res);
    }
  }
}


void LayerFolder::remove_layer(Layer& layer) {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::remove_layer(): LayerFolder is null");
  }
  SUResult res = SULayerFolderRemoveLayer(this->ref(), layer.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


std::vector<Layer> LayerFolder::layers() const {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::layers(): LayerFolder is null");
  }
  size_t count = 0;
  SUResult res = SULayerFolderGetNumLayers(this->ref(), &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Layer>(0);
  }
  std::vector<SULayerRef> refs(count, SU_INVALID);
  res = SULayerFolderGetLayers(this->ref(), count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Layer> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.emplace_back(refs[i], true);
  }
  return result;
}


void LayerFolder::add_layer_folder(LayerFolder& folder) {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::add_layer_folder(): LayerFolder is null");
  }
  if (!folder) {
    throw std::invalid_argument("CW::LayerFolder::add_layer_folder(): folder is null");
  }
  else if (folder.attached() == false) {
    throw std::invalid_argument("CW::LayerFolder::add_layer_folder(): folder must be attached to a model before adding to LayerFolder");
  }
  else if (folder.model() != this->model()) {
    throw std::invalid_argument("CW::LayerFolder::add_layer_folder(): folder must belong to the same model as the layer folder");
  }
  SUResult res = SULayerFolderAddLayerFolder(this->ref(), folder.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


std::vector<LayerFolder> LayerFolder::sub_folders() const {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::sub_folders(): LayerFolder is null");
  }
  size_t count = 0;
  SUResult res = SULayerFolderGetNumLayerFolders(this->ref(), &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<LayerFolder>(0);
  }
  std::vector<SULayerFolderRef> refs(count, SU_INVALID);
  res = SULayerFolderGetLayerFolders(this->ref(), count, refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<LayerFolder> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.emplace_back(refs[i], true);
  }
  return result;
}


LayerFolder LayerFolder::parent_folder() const {
  if (!(*this)) {
    throw std::logic_error("CW::LayerFolder::parent_folder(): LayerFolder is null");
  }
  SULayerFolderRef parent = SU_INVALID;
  SUResult res = SULayerFolderGetParentLayerFolder(this->ref(), &parent);
  if (res == SU_ERROR_NO_DATA || SUIsInvalid(parent)) {
    return LayerFolder(); // No parent
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return LayerFolder(parent, true);
}


} /* namespace CW */
