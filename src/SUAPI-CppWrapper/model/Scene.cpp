//
//  Scene.cpp
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

#include "SUAPI-CppWrapper/model/Scene.hpp"

#include <cassert>
#include <stdexcept>

#include "SUAPI-CppWrapper/String.hpp"
#include "SUAPI-CppWrapper/model/Camera.hpp"
#include "SUAPI-CppWrapper/model/Layer.hpp"

namespace CW {

/***************************
** Private Static Methods **
****************************/
SUSceneRef Scene::create_scene() {
  SUSceneRef scene = SU_INVALID;
  SUResult res = SUSceneCreate(&scene);
  assert(res == SU_ERROR_NONE); _unused(res);
  return scene;
}


SUSceneRef Scene::copy_reference(const Scene& other) {
  if (other.m_attached || SUIsInvalid(other.m_entity)) {
    return other.ref();
  }
  SUSceneRef new_scene = create_scene();
  // Copy basic properties from the source scene
  SUSceneRef other_ref = other.ref();
  // Copy name
  SUStringRef name = SU_INVALID;
  SUStringCreate(&name);
  if (SUSceneGetName(other_ref, &name) == SU_ERROR_NONE) {
    size_t len = 0;
    SUStringGetUTF8Length(name, &len);
    if (len > 0) {
      std::string name_str(len, '\0');
      size_t out_len = 0;
      SUStringGetUTF8(name, len + 1, &name_str[0], &out_len);
      SUSceneSetName(new_scene, name_str.c_str());
    }
  }
  SUStringRelease(&name);
  // Copy description
  SUStringRef desc = SU_INVALID;
  SUStringCreate(&desc);
  if (SUSceneGetDescription(other_ref, &desc) == SU_ERROR_NONE) {
    size_t len = 0;
    SUStringGetUTF8Length(desc, &len);
    if (len > 0) {
      std::string desc_str(len, '\0');
      size_t out_len = 0;
      SUStringGetUTF8(desc, len + 1, &desc_str[0], &out_len);
      SUSceneSetDescription(new_scene, desc_str.c_str());
    }
  }
  SUStringRelease(&desc);
  // Copy camera
  SUCameraRef other_cam = SU_INVALID;
  if (SUSceneGetCamera(other_ref, &other_cam) == SU_ERROR_NONE) {
    SUSceneSetCamera(new_scene, other_cam);
  }
  // Copy flags
  uint32_t flags = 0;
  if (SUSceneGetFlags(other_ref, &flags) == SU_ERROR_NONE) {
    SUSceneSetFlags(new_scene, flags);
  }
  return new_scene;
}


/******************************
** Constructors / Destructor **
*******************************/
Scene::Scene():
  Entity(SU_INVALID, false)
{}


Scene::Scene(const std::string& name):
  Entity(SUSceneToEntity(create_scene()), false)
{
  this->name(name);
}


Scene::Scene(SUSceneRef scene, bool attached):
  Entity(SUSceneToEntity(scene), attached)
{}


Scene::Scene(const Scene& other):
  Entity(SUSceneToEntity(copy_reference(other)), other.m_attached)
{}


Scene::~Scene() {
  if (!m_attached && SUIsValid(m_entity)) {
    SUSceneRef scene = this->ref();
    SUResult res = SUSceneRelease(&scene);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


/*******************
** Public Methods **
********************/
Scene& Scene::operator=(const Scene& other) {
  if (!m_attached && SUIsValid(m_entity)) {
    SUSceneRef scene = this->ref();
    SUResult res = SUSceneRelease(&scene);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
  m_entity = SUSceneToEntity(copy_reference(other));
  Entity::operator=(other);
  return (*this);
}


SUSceneRef Scene::ref() const {
  return SUSceneFromEntity(m_entity);
}

Scene::operator SUSceneRef() const {
  return this->ref();
}

Scene::operator SUSceneRef*() {
  return reinterpret_cast<SUSceneRef*>(&m_entity);
}


bool Scene::operator!() const {
  return SUIsInvalid(m_entity);
}


void Scene::name(const String& name) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::name(): Scene is null");
  }
  SUResult res = SUSceneSetName(this->ref(), name.std_string().c_str());
  switch (res) {
    case SU_ERROR_INVALID_ARGUMENT: {
      throw std::invalid_argument("CW::Scene::name(): the name already exists in the scene's model");
    }
    default: {
      assert(res == SU_ERROR_NONE); _unused(res);
    }
  }
}


void Scene::name(const std::string& name) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::name(): Scene is null");
  }
  SUResult res = SUSceneSetName(this->ref(), name.c_str());
  switch (res) {
    case SU_ERROR_INVALID_ARGUMENT: {
      throw std::invalid_argument("CW::Scene::name(): the name already exists in the scene's model");
    }
    default: {
      assert(res == SU_ERROR_NONE); _unused(res);
    }
  }
}


String Scene::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::name(): Scene is null");
  }
  String string;
  SUResult res = SUSceneGetName(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


void Scene::description(const String& desc) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::description(): Scene is null");
  }
  SUResult res = SUSceneSetDescription(this->ref(), desc.std_string().c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Scene::description(const std::string& desc) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::description(): Scene is null");
  }
  SUResult res = SUSceneSetDescription(this->ref(), desc.c_str());
  assert(res == SU_ERROR_NONE); _unused(res);
}


String Scene::description() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::description(): Scene is null");
  }
  String string;
  SUResult res = SUSceneGetDescription(this->ref(), string);
  assert(res == SU_ERROR_NONE); _unused(res);
  return string;
}


Camera Scene::camera() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::camera(): Scene is null");
  }
  SUCameraRef camera = SU_INVALID;
  SUResult res = SUSceneGetCamera(this->ref(), &camera);
  assert(res == SU_ERROR_NONE); _unused(res);
  // The camera is owned by the scene, so it is attached
  return Camera(camera, true);
}


void Scene::camera(const Camera& camera) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::camera(): Scene is null");
  }
  SUResult res = SUSceneSetCamera(this->ref(), camera.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_camera() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_camera(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseCamera(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_camera(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_camera(): Scene is null");
  }
  SUResult res = SUSceneSetUseCamera(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::include_in_animation() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::include_in_animation(): Scene is null");
  }
  bool include;
  SUResult res = SUSceneGetIncludeInAnimation(this->ref(), &include);
  assert(res == SU_ERROR_NONE); _unused(res);
  return include;
}


void Scene::include_in_animation(bool include) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::include_in_animation(): Scene is null");
  }
  SUResult res = SUSceneSetIncludeInAnimation(this->ref(), include);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_shadow_info() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_shadow_info(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseShadowInfo(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_shadow_info(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_shadow_info(): Scene is null");
  }
  SUResult res = SUSceneSetUseShadowInfo(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_rendering_options() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_rendering_options(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseRenderingOptions(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_rendering_options(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_rendering_options(): Scene is null");
  }
  SUResult res = SUSceneSetUseRenderingOptions(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_hidden_layers() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_hidden_layers(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseHiddenLayers(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_hidden_layers(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_hidden_layers(): Scene is null");
  }
  SUResult res = SUSceneSetUseHiddenLayers(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_hidden_geometry() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_hidden_geometry(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseHiddenGeometry(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_hidden_geometry(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_hidden_geometry(): Scene is null");
  }
  SUResult res = SUSceneSetUseHiddenGeometry(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_hidden_objects() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_hidden_objects(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseHiddenObjects(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_hidden_objects(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_hidden_objects(): Scene is null");
  }
  SUResult res = SUSceneSetUseHiddenObjects(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_axes() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_axes(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseAxes(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_axes(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_axes(): Scene is null");
  }
  SUResult res = SUSceneSetUseAxes(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


bool Scene::use_section_planes() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_section_planes(): Scene is null");
  }
  bool use;
  SUResult res = SUSceneGetUseSectionPlanes(this->ref(), &use);
  assert(res == SU_ERROR_NONE); _unused(res);
  return use;
}


void Scene::use_section_planes(bool use) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::use_section_planes(): Scene is null");
  }
  SUResult res = SUSceneSetUseSectionPlanes(this->ref(), use);
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Scene::flags(uint32_t flags) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::flags(): Scene is null");
  }
  SUResult res = SUSceneSetFlags(this->ref(), flags);
  assert(res == SU_ERROR_NONE); _unused(res);
}


uint32_t Scene::flags() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::flags(): Scene is null");
  }
  uint32_t f;
  SUResult res = SUSceneGetFlags(this->ref(), &f);
  assert(res == SU_ERROR_NONE); _unused(res);
  return f;
}


bool Scene::sketch_axes_displayed() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::sketch_axes_displayed(): Scene is null");
  }
  bool displayed;
  SUResult res = SUSceneGetSketchAxesDisplayed(this->ref(), &displayed);
  assert(res == SU_ERROR_NONE); _unused(res);
  return displayed;
}


void Scene::sketch_axes_displayed(bool displayed) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::sketch_axes_displayed(): Scene is null");
  }
  SUResult res = SUSceneSetSketchAxesDisplayed(this->ref(), displayed);
  assert(res == SU_ERROR_NONE); _unused(res);
}


std::vector<Layer> Scene::layers() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::layers(): Scene is null");
  }
  size_t count = 0;
  SUResult res = SUSceneGetNumLayers(this->ref(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  if (count == 0) {
    return std::vector<Layer>();
  }
  std::vector<SULayerRef> layer_refs(count, SU_INVALID);
  res = SUSceneGetLayers(this->ref(), count, layer_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Layer> result;
  result.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    result.emplace_back(layer_refs[i], true);
  }
  return result;
}


void Scene::add_layer(const Layer& layer) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::add_layer(): Scene is null");
  }
  SUResult res = SUSceneAddLayer(this->ref(), layer.ref());
  switch (res) {
    case SU_ERROR_NO_DATA: {
      throw std::logic_error("CW::Scene::add_layer(): Scene is not owned by a valid model");
    }
    case SU_ERROR_GENERIC: {
      throw std::logic_error("CW::Scene::add_layer(): the scene's model does not contain the layer");
    }
    case SU_ERROR_INVALID_ARGUMENT: {
      throw std::invalid_argument("CW::Scene::add_layer(): the layer already exists in the scene");
    }
    default: {
      assert(res == SU_ERROR_NONE); _unused(res);
    }
  }
}


void Scene::remove_layer(const Layer& layer) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::remove_layer(): Scene is null");
  }
  SUResult res = SUSceneRemoveLayer(this->ref(), layer.ref());
  if (res == SU_ERROR_INVALID_ARGUMENT) {
    throw std::invalid_argument("CW::Scene::remove_layer(): the layer doesn't exist in the scene");
  }
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Scene::clear_layers() {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::clear_layers(): Scene is null");
  }
  SUResult res = SUSceneClearLayers(this->ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Scene::copy_from(const Scene& other) {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::copy_from(): Scene is null");
  }
  if (!other) {
    throw std::invalid_argument("CW::Scene::copy_from(): source Scene is null");
  }
  SUResult res = SUSceneCopy(this->ref(), other.ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


void Scene::activate() {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::activate(): Scene is null");
  }
  SUResult res = SUSceneActivate(this->ref());
  assert(res == SU_ERROR_NONE); _unused(res);
}


Scene Scene::copy() const {
  if (!(*this)) {
    throw std::logic_error("CW::Scene::copy(): Scene is null");
  }
  Scene new_scene(std::string(this->name()));
  new_scene.description(this->description());
  new_scene.flags(this->flags());
  // Note: camera is not copied here because SUSceneSetCamera only works
  // reliably on scenes that have been added to a model. Use
  // scene.camera(source.camera()) after adding the scene to a model.
  new_scene.copy_attributes_from(*this);
  return new_scene;
}


} /* namespace CW */
