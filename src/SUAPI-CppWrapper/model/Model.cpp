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
#include "SUAPI-CppWrapper/model/Texture.hpp"
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
  m_release_on_destroy(true)
{
  enum SUModelLoadStatus status;
  m_result = SUModelCreateFromFileWithStatus(&m_model, file_path.c_str(), &status);
  switch (m_result) {
    case SU_ERROR_NONE:
      // Model loaded successfully
      break;
    case SU_ERROR_NULL_POINTER_INPUT:
      throw std::invalid_argument{"No model exists in the path: "} + file_path;
      break;
    case SU_ERROR_NULL_POINTER_OUTPUT:
      throw std::logic_error{"Model failed to load"}; // this shoud never happen
      break;
    case SU_ERROR_SERIALIZATION:
      throw std::logic_error{"Model failed to load"};
      break;
    case SU_ERROR_MODEL_INVALID:
      throw std::invalid_argument{"Tried to open an invalid model: "}  + file_path;
      break;
    case SU_ERROR_MODEL_VERSION:
      throw std::invalid_argument{"This model is from a newer version of Sketchup: "}  + file_path;
      break;
    default:
      throw std::logic_error{"unhandled exception"};
      break;
  }
  // TODO: deal with the status issue.
}

Model::Model(const Model& other):
  m_model(other.m_model),
  m_release_on_destroy(false),
  m_result(other.m_result)
{}

Model::Model(Model&& other) noexcept:
  m_model(other.m_model),
  m_release_on_destroy(other.m_release_on_destroy),
  m_result(other.m_result)
{
  other.m_model = SU_INVALID;
  other.m_release_on_destroy = false;
}


Model::~Model() {
  if (m_release_on_destroy && SUIsValid(m_model)) {
    SUResult res = SUModelRelease(&m_model);
    assert(res == SU_ERROR_NONE); _unused(res);
    m_model = SU_INVALID;
    m_release_on_destroy = false;
  }
}

Model& Model::operator=(const Model& other) {
  if (this != &other) {
    if (m_release_on_destroy && SUIsValid(m_model)) {
      SUResult res = SUModelRelease(&m_model);
      assert(res == SU_ERROR_NONE); _unused(res);
      m_model = SU_INVALID;
      m_release_on_destroy = false;
    }
    m_model = other.m_model;
    m_release_on_destroy = false; // copies are non-owning
    m_result = other.m_result;
  }
  return *this;
}

Model& Model::operator=(Model&& other) noexcept {
  if (this != &other) {
    if (m_release_on_destroy && SUIsValid(m_model)) {
      SUResult res = SUModelRelease(&m_model);
      assert(res == SU_ERROR_NONE); _unused(res);
    }
    m_model = other.m_model;
    m_release_on_destroy = other.m_release_on_destroy;
    m_result = other.m_result;
    other.m_model = SU_INVALID;
    other.m_release_on_destroy = false;
  }
  return *this;
}


bool Model::operator==(const Model& rhs) const {
  return (this->m_model.ptr == rhs.m_model.ptr);
}


bool Model::operator!=(const Model& rhs) const {
  return !operator==(rhs);
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


void Model::add_definition(ComponentDefinition& definition) {
  if(!(*this)) {
    throw std::logic_error("CW::Model::add_definition(): Model is null");
  }
  std::vector<ComponentDefinition> defs = {definition};
  return add_definitions(defs);
}


void Model::add_definitions(std::vector<ComponentDefinition>& definitions) {
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
  switch (res) {
    case SU_ERROR_NONE:
      // success
      return;
      break;
    case SU_ERROR_NULL_POINTER_INPUT:
      throw std::invalid_argument("CW::Model::add_definition(s): component definitions(s) passed as parametersare invalid");
      break;
    case SU_ERROR_INVALID_INPUT:
      throw std::logic_error("CW::Model::add_definition(s): model is invalid");
      break;
    default:
      throw std::logic_error("CW::Model::add_definition(s): unhandled exception");
      break;
  }
  return;
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
  #if SketchUpAPI_VERSION_MAJOR < 2021
  return Entities(entities, m_model);
  #else
  return Entities(entities);
  #endif
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


size_t Model::num_layers() const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::layers(): Model is null");
  }
  size_t count = 0;
  SUResult res = SUModelGetNumLayers(m_model, &count);
  assert(res == SU_ERROR_NONE);
  return count;
}

/*
* Returns the list of layers in the model.
* @return layers a vector array of Layer objects in the model.
*/
std::vector<Layer> Model::layers() const {
  size_t count = this->num_layers();
  std::vector<SULayerRef> layer_refs(count, SU_INVALID);
  SUResult res = SUModelGetLayers(m_model, count, layer_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Layer> layers(count);
  std::transform(layer_refs.begin(), layer_refs.end(), layers.begin(),
    [](const SULayerRef& value){
      return Layer(value);
    });
  return layers;
}


void Model::add_layers(std::vector<Layer>& layers, bool overwrite_existing) {
  std::vector<Layer> existing_layers = this->layers();
  std::vector<Layer> layers_to_add; layers_to_add.reserve(layers.size());
  for (const Layer& lay : layers) {
    if (!lay) {
      continue;
    }
    // Find a layer matched by name in the model
    const auto found_layer = std::find_if(existing_layers.begin(), existing_layers.end(),
          [lay](const Layer& value2){ return lay.name() == value2.name();});
    if (found_layer != existing_layers.end()) {
      // Layer to add is already in the model
      if (overwrite_existing) {
        // Update properties of existing layer
        // TODO: as more properties are added to Layer, they should be updated here
      }
      else {
        // Skip adding this layer as it already exists
        continue;
      }
    }
    else {
      // No layer with matching name found - add it
      // Check that each layer is not attached to another model
      if (lay.attached()) {
        layers_to_add.push_back(lay.copy());
      }
      else {
        layers_to_add.push_back(lay);
      }
    }
  }
  std::vector<SULayerRef> layer_refs(layers_to_add.size(), SU_INVALID);
  std::transform(layers_to_add.begin(), layers_to_add.end(), layer_refs.begin(),
    [](const Layer& value){
      return value.ref();
    });
  SUResult res = SUModelAddLayers(m_model, layers_to_add.size(), layer_refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (size_t i=0; i < layers_to_add.size(); i++) {
    layers_to_add[i].attached(true);
  }
}


bool Model::layer_exists(const Layer& layer, bool strict) const {
  std::vector<Layer> layers = this->layers();
  if (strict) {
    // For strict checking, the layer object must be the same
    for (auto& lay : layers) {
      if (lay == layer) {
        return true;
      }
    }
  }
  else {
    // For non-strict checking, we simply compare the name of the layer
    for (auto& lay : layers) {
      if (lay.name() == layer.name()) {
        return true;
      }
    }
  }
  return false;
}


size_t Model::num_materials() const {
  if (!(*this)) {
    throw std::logic_error("CW::Model::materials(): Model is null");
  }
  size_t count = 0;
  SUResult res = SUModelGetNumMaterials(m_model, &count);
  assert(res == SU_ERROR_NONE);
  return count;
}


/*
* Returns the Location object of the model
* @return location Location object. If no location has been assigned to the model, the Location object returned will be invalid.
*/
// Location location();


std::vector<Material> Model::materials() const {
  size_t count = this->num_materials();
  if (count == 0) {
    return std::vector<Material> {};
  }
  std::vector<SUMaterialRef> material_refs(count, SU_INVALID);
  SUResult res = SUModelGetMaterials(m_model, count, material_refs.data(), &count);
  assert(res == SU_ERROR_NONE); _unused(res);
  std::vector<Material> materials(count);
  std::transform(material_refs.begin(), material_refs.end(), materials.begin(),
    [](const SUMaterialRef& value){
      return Material(value);
    });
  return materials;
}


void Model::add_materials(std::vector<Material>& materials,  bool overwrite_existing) {
  if (materials.size() == 0) {
    throw std::invalid_argument("CW::Model::add_materials(): No Material objects were passed to add to the model.");
  }
  std::vector<Material> existing_materials = this->materials();
  std::vector<Material> materials_to_add; materials_to_add.reserve(materials.size());
  for (Material& mat : materials) {
    if (!mat) {
      continue;
    }
    // Find a matching material in the model
    const auto found_material = std::find_if(existing_materials.begin(), existing_materials.end(),
          [mat](const Material& value2){ return mat.name() == value2.name();});
    if (found_material != existing_materials.end()) {
      if (overwrite_existing) {
        // Update properties of existing material
        // TODO: as more properties are added to Material, they should be updated here
        found_material->opacity(mat.opacity());
        found_material->type(mat.type());
        found_material->use_alpha(mat.use_alpha());
        Texture texture = mat.texture();
        if (!!texture) {
          found_material->texture(texture);
        }
        found_material->copy_attributes_from(mat);
      }
      else {
        // Skip adding this material as it already exists
        continue;
      }
    }
    else {
      // Check that each material is not attached to another model
      if (mat.attached()) {
        materials_to_add.push_back(mat.copy());
      }
      else {
        materials_to_add.push_back(mat);
      }
    }
  }
  if (materials_to_add.size() == 0) {
    // Nothing to add
    return;
  }
  // for (size_t i=0; i < materials.size(); i++) {
  //   // Check that each material is not attached to another model
  //   if (materials[i].attached()) {
  //     throw std::invalid_argument("CW::Model::add_materials(): At least one of the Material objects passed is attached to another model.  Use Material::copy() to create a new unattached Material object and try again.");
  //   }
  // }
  std::vector<SUMaterialRef> material_refs(materials_to_add.size(), SU_INVALID);
  std::transform(materials_to_add.begin(), materials_to_add.end(), material_refs.begin(),
    [](const Material& value){
      return value.ref();
    });
  SUResult res = SUModelAddMaterials(m_model, materials_to_add.size(), material_refs.data());
  assert(res == SU_ERROR_NONE); _unused(res);
  for (size_t i=0; i < materials_to_add.size(); i++) {
    materials_to_add[i].attached(true);
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
