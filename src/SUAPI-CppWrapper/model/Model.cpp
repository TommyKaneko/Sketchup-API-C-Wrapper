//
//  Model.cpp
//
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

// Macro for getting rid of unused variables commonly for assert checking
#define _unused(x) ((void)(x))

#include "SUAPI-CppWrapper/model/Model.hpp"

#include <cassert>

#include "SUAPI-CppWrapper/model/Layer.hpp"
#include "SUAPI-CppWrapper/model/Axes.hpp"
#include "SUAPI-CppWrapper/model/Entities.hpp"
//#include "SUAPI-CppWrapper/Behavior.hpp"
//#include "SUAPI-CppWrapper/model/Classifications.hpp"
#include "SUAPI-CppWrapper/model/ComponentDefinition.hpp"
#include "SUAPI-CppWrapper/model/InstancePath.hpp"
#include "SUAPI-CppWrapper/model/Material.hpp"
#include "SUAPI-CppWrapper/model/AttributeDictionary.hpp"
#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include "SUAPI-CppWrapper/model/OptionsManager.hpp"
#include "SUAPI-CppWrapper/model/RenderingOptions.hpp"
#include "SUAPI-CppWrapper/model/ShadowInfo.hpp"


namespace CW {


SUModelRef Model::create_model() {
  SUModelRef model = SU_INVALID;
  SUResult res = SUModelCreate(&model);
  assert(res == SU_ERROR_NONE); _unused(res);
  return model;
}


Model::Model():
  m_model(create_model()),
  m_release_on_destroy(true)
{}


Model::Model(SUModelRef model_ref, bool release_on_destroy):
  m_model(model_ref),
  m_release_on_destroy(release_on_destroy)
{}

Model::Model(std::string file_path):
  m_model(SU_INVALID),
  m_release_on_destroy(true),
  m_result(SUModelCreateFromFile(&m_model, file_path.c_str()))
{}

Model::Model(const Model& other):
  m_model(other.m_model),
  m_release_on_destroy(other.m_release_on_destroy)
{}

Model::~Model() {
  if (m_release_on_destroy && SUIsValid(m_model)) {
    SUResult res = SUModelRelease(&m_model);
    assert(res == SU_ERROR_NONE); _unused(res);
  }
}


SUModelRef Model::ref() const {
  return m_model;
}


Model::operator SUModelRef() const {
  return ref();
}


Model::operator SUModelRef*() {
  return &m_model;
}


Model::operator bool() const {
  if (m_result == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


bool Model::operator!() const {
  return !bool(*this);
}

std::string Model::version_string() const
{
  int major = 0, minor = 0, build = 0;
  SUResult res = SUModelGetVersion(m_model, &major, &minor, &build);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::string version = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(build);
  return version;
}


Layer Model::active_layer() const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::active_layer(): Model is null");
  }
  SULayerRef layer = SU_INVALID;
  SUResult res = SUModelGetDefaultLayer(m_model, &layer);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Layer(layer);
}

/*
* Sets the active layer of the model.
* @param default_layer the Layer object to be the active layer on first opening the model
* @return status true if succsessful
  * TODO: default layer cannot be set through API
*/
//bool active_layer(Layer default_layer) {}


bool Model::add_definition(ComponentDefinition& definition) {
  if(!(*this)) {
    throw std::logic_error("CW::Model::add_definition(): Model is null");
  }
  std::vector<ComponentDefinition> defs = {definition};
  return add_definitions(defs);
}


bool Model::add_definitions(std::vector<ComponentDefinition>& definitions) {
  if(!(*this)) {
    throw std::logic_error("CW::Model::add_definitions(): Model is null");
  }
  std::vector<SUComponentDefinitionRef> defs(definitions.size(), SU_INVALID);
  std::transform(definitions.begin(), definitions.end(), defs.begin(),
    [](const ComponentDefinition& definition) {
      return definition.ref();
    }
  );
  SUResult res = SUModelAddComponentDefinitions(m_model, definitions.size(), defs.data());
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


std::vector<AttributeDictionary>  Model::attribute_dictionaries() const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::attribute_dictionaries(): Model is null");
  }
  size_t count = 0;
  SUResult res = SUModelGetNumAttributeDictionaries(m_model, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUAttributeDictionaryRef> dict_refs(count, SU_INVALID);
  res = SUModelGetAttributeDictionaries(m_model, count, dict_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<AttributeDictionary> dicts(count);
  std::transform(dict_refs.begin(), dict_refs.end(), dicts.begin(),
    [](const SUAttributeDictionaryRef& value){
      return AttributeDictionary(value);
    });
  return dicts;
}


AttributeDictionary Model::attribute_dictionary(const std::string& dict_name) const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::attribute_dictionary(): Model is null");
  }
  SUAttributeDictionaryRef dict = SU_INVALID;
  SUResult res = SUModelGetAttributeDictionary(m_model, dict_name.c_str(), &dict);
  assert(res == SU_ERROR_NONE); _unused(res);
  return AttributeDictionary(dict);
}



Axes Model::axes() const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::axes(): Model is null");
  }
  SUAxesRef axes = SU_INVALID;
  SUResult res = SUModelGetAxes(m_model, &axes);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Axes(axes);
}


//Behavior behavior(); // TODO: this may not be possible to retrieve

/*
Classifications Model::classifications() const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::classifications(): Model is null");
  }
  SUClassificationsRef classifications = SU_INVALID;
  SUResult res = SUModelGetClassifications(m_model, &classifications);
  assert(res == SU_ERROR_NONE);
  return Classifications(classifications);
}
*/

/*
* Returns the description attached to this model.
* @return description string object.
*/
//std::string description();
//bool description(std::string description_string);


std::vector<ComponentDefinition> Model::definitions() const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::definitions(): Model is null");
  }
  size_t count = 0;
  SUResult res = SUModelGetNumComponentDefinitions(m_model, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUComponentDefinitionRef> def_refs(count, SU_INVALID);
  res = SUModelGetComponentDefinitions(m_model, count, def_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<ComponentDefinition> defs(count);
  std::transform(def_refs.begin(), def_refs.end(), defs.begin(),
    [](const SUComponentDefinitionRef& value){
      return ComponentDefinition(value);
    });
  return defs;
}


std::vector<ComponentDefinition> Model::group_definitions() const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::group_definitions(): Model is null");
  }
  size_t count = 0;
  SUResult res = SUModelGetNumGroupDefinitions(m_model, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SUComponentDefinitionRef> def_refs(count, SU_INVALID);
  res = SUModelGetGroupDefinitions(m_model, count, def_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<ComponentDefinition> defs(count);
  std::transform(def_refs.begin(), def_refs.end(), defs.begin(),
    [](const SUComponentDefinitionRef& value){
      return ComponentDefinition(value);
    });
  return defs;
}


InstancePath Model::instance_path(const String& persistent_id) const {
  SUInstancePathRef instance_path_ref = SU_INVALID;
  SUResult res = SUInstancePathCreate(&instance_path_ref);
  assert(res == SU_ERROR_NONE);
  res = SUModelGetInstancePathByPid(m_model, persistent_id.ref(), &instance_path_ref);
  if (res == SU_ERROR_GENERIC || res == SU_ERROR_NO_DATA) {
    // Probably passed an empty or invalid string - return an empty instance path
    return InstancePath();
  }
  assert(res == SU_ERROR_NONE); _unused(res);
  return InstancePath(instance_path_ref);
}


Entities Model::entities() const {
  if(!(*this)) {
    throw std::logic_error("CW::Model::entities(): Model is null");
  }
  SUEntitiesRef entities = SU_INVALID;
  SUResult res = SUModelGetEntities(m_model, &entities);
  assert(res == SU_ERROR_NONE); _unused(res);
  return Entities(entities, m_model);
}


//find_entity_by_id();  // TODO can this be done?

// TODO - build Location class before this method
/*
bool Model::georeferenced() const {
  SULocationRef loc = SU_INVALID;
  SUResult res = SUModelGetLocation(m_model, &loc);
  assert(res == SU_ERROR_NONE);
  Location location(loc);
  if (loc) {
    return true;
  }
  return false;
}
*/


TypedValue Model::get_attribute(const AttributeDictionary& dict, const std::string& key, const TypedValue& default_value) const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::get_attribute(): Model is null");
  }
  return dict.get_attribute(key, default_value);
}

TypedValue Model::get_attribute(const std::string& dict_name, const std::string& key, const TypedValue& default_value) const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::get_attribute(): Model is null");
  }
  AttributeDictionary dictionary = attribute_dictionary(dict_name);
  return get_attribute(dictionary, key, default_value);
}


/*
* Returns the GUID of the model.
*/
//guid();

/*
* Returns the list of layers in the model.
* @return layers a vector array of Layer objects in the model.
*/
std::vector<Layer> Model::layers() const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::layers(): Model is null");
  }
  size_t count = 0;
  SUResult res = SUModelGetNumLayers(m_model, &count);
  assert(res == SU_ERROR_NONE);
  std::vector<SULayerRef> layer_refs(count, SU_INVALID);
  res = SUModelGetLayers(m_model, count, layer_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Layer> layers(count);
  std::transform(layer_refs.begin(), layer_refs.end(), layers.begin(),
    [](const SULayerRef& value){
      return Layer(value);
    });
  return layers;
}


void Model::add_layers(std::vector<Layer>& layers) {
  for (size_t i=0; i < layers.size(); i++) {
    // Check that each material is not attached to another model
    if (layers[i].attached()) {
      throw std::invalid_argument("CW::Model::add_layers(): At least one of the Layer objects passed is attached to another model.  Use Layer::copy() to create a new unattached Layer object and try again.");
    }
  }
  std::vector<SULayerRef> layer_refs(layers.size(), SU_INVALID);
  std::transform(layers.begin(), layers.end(), layer_refs.begin(),
    [](const Layer& value){
      return value.ref();
    });
  SUResult res = SUModelAddLayers(m_model, layers.size(), layer_refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (size_t i=0; i < layers.size(); i++) {
    layers[i].attached(true);
  }
}


bool Model::layer_exists(const Layer& layer) const {
  std::vector<Layer> layers = this->layers();
  for (auto& lay : layers) {
    if (lay == layer) {
      return true;
    }
  }
  return false;
}

/*
* Returns the Location object of the model
* @return location Location object. If no location has been assigned to the model, the Location object returned will be invalid.
*/
// Location location();


std::vector<Material> Model::materials() const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::materials(): Model is null");
  }
  size_t count = 0;
  SUResult res = SUModelGetNumMaterials(m_model, &count);
  assert(res == SU_ERROR_NONE);
  if (count == 0) {
    return std::vector<Material> {};
  }
  std::vector<SUMaterialRef> material_refs(count, SU_INVALID);
  res = SUModelGetMaterials(m_model, count, material_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Material> materials(count);
  std::transform(material_refs.begin(), material_refs.end(), materials.begin(),
    [](const SUMaterialRef& value){
      return Material(value);
    });
  return materials;
}


void Model::add_materials(std::vector<Material>& materials) {
  for (size_t i=0; i < materials.size(); i++) {
    // Check that each material is not attached to another model
    if (materials[i].attached()) {
      throw std::invalid_argument("CW::Model::add_materials(): At least one of the Material objects passed is attached to another model.  Use Material::copy() to create a new unattached Material object and try again.");
    }
  }
  std::vector<SUMaterialRef> material_refs(materials.size(), SU_INVALID);
  std::transform(materials.begin(), materials.end(), material_refs.begin(),
    [](const Material& value){
      return value.ref();
    });
  SUResult res = SUModelAddMaterials(m_model, materials.size(), material_refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (size_t i=0; i < materials.size(); i++) {
    materials[i].attached(true);
  }
}


bool Model::material_exists(const Material& material) const {
  std::vector<Material> materials = this->materials();
  for (auto& mat : materials) {
    if (mat == material) {
      return true;
    }
  }
  return false;
}


String Model::name() const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::name(): Model is null");
  }
  SUStringRef name = SU_INVALID;
  SUModelGetName(m_model, &name);
  return String(name);
}


bool Model::name(const String& name_string) {
  if (!(*this)) {
    throw std::logic_error("CW::Model::name(): Model is null");
  }
  std::string std_string = name_string;
  SUResult res = SUModelSetName(m_model, std_string.c_str());
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


size_t Model::num_faces() const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::num_faces(): Model is null");
  }
  ModelStatistics model_statistics((*this));
  return model_statistics.faces();
}

OptionsManager Model::options()
{
  SUOptionsManagerRef options_manager = SU_INVALID;
  SUResult res = SUModelGetOptionsManager(m_model, &options_manager);
  assert(res == SU_ERROR_NONE); _unused(res);
  return OptionsManager(options_manager);
}

/*
* Returns a key=>value list of options for the model.
* @see SUOptionsProviderRef
*/
//std::vector<std::pair<std::string, std::string>> Model::options() const;

/*
* Returns the path of the model.
*/
// TODO - probably delete this, as there is no way to get the path of the model through the API.
// std::string Model::path() const {}

/*
* Returns the first Entity object that a ray from a given point and direction vector will hit.
*/
//Entity raytest(Point3D point, Vector3D vector);


SUResult Model::save(const std::string& file_path) {
  if (!(*this)) {
    throw std::logic_error("CW::Model::save(): Model is null");
  }
  const char * c_string = file_path.c_str();
  SUResult res = SUModelSaveToFile(m_model, c_string);
  return res;
}


bool Model::save_with_version(const std::string& file_path, SUModelVersion version) {
  if (!(*this)) {
    throw std::logic_error("CW::Model::save_with_version(): Model is null");
  }
  SUResult res = SUModelSaveToFileWithVersion(m_model, file_path.c_str(), version);
  if (res == SU_ERROR_NONE) {
    return true;
  }
  return false;
}


/*
* Returns the array of Scene objects attached to the model.
* @return scenes array of Scene objects.
*/
// std::vector<Scene> scenes();


bool Model::set_attribute(AttributeDictionary& dict, const std::string& key, const TypedValue& value) {
  if (!(*this)) {
    throw std::logic_error("CW::Model::set_attribute(): Model is null");
  }
  return dict.set_attribute(key, value);
}

bool Model::set_attribute(const std::string& dict_name, const std::string& key, const TypedValue& value) {
  if (!(*this)) {
    throw std::logic_error("CW::Model::set_attribute(): Model is null");
  }
  AttributeDictionary dict = attribute_dictionary(dict_name);
  return set_attribute(dict, key, value);
}

RenderingOptions Model::rendering_options()
{
  SURenderingOptionsRef ref = SU_INVALID;
  SUResult res = SUModelGetRenderingOptions(m_model, &ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return RenderingOptions(ref);
}

ShadowInfo Model::shadow_info()
{
  SUShadowInfoRef ref = SU_INVALID;
  SUResult res = SUModelGetShadowInfo(m_model, &ref);
  assert(res == SU_ERROR_NONE); _unused(res);
  return ShadowInfo(ref);
}


// set_datum()

/*
* Returns the ShadowInfo object of the model.
*/
// ShadowInfo shadow_info();

/*
* Returns the list of styles in the model.
* @return styles vector array of Style objects
*/
// std::vector<Style> styles();

// tags
// tags=

//std::string title() { return name();}
//std::string title(std::string name_value) { return name(name_value);}

/******************
* ModelStatistics *
*******************/
ModelStatistics::ModelStatistics(SUModelStatistics model_statistics):
  m_model_statistics(model_statistics)
{}

ModelStatistics::ModelStatistics(const Model& model):
  m_model_statistics(SUModelStatistics{})
{
  SUResult res = SUModelGetStatistics(model, &m_model_statistics);
  assert(res == SU_ERROR_NONE); _unused(res);
}
  
int ModelStatistics::edges() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_Edge];
}

int ModelStatistics::faces() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_Face];
}
int ModelStatistics::instances() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_ComponentInstance];
}
int ModelStatistics::groups() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_Group];
}
int ModelStatistics::definitions() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_ComponentDefinition];
}
int ModelStatistics::layers() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_Layer];
}
int ModelStatistics::materials() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_Material];
}
int ModelStatistics::images() const
{
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_Image];
}

  

} /* namespace CW */
