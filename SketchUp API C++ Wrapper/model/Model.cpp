//
//  Model.cpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "Model.hpp"

#include <cassert>

#include "Layer.hpp"
#include "Axes.hpp"
#include "Entities.hpp"
//#include "Behavior.hpp"
#include "Classifications.hpp"
#include "ComponentDefinition.hpp"
#include "Material.hpp"
#include "AttributeDictionary.hpp"
#include "TypedValue.hpp"


namespace CW {


SUModelRef Model::create_model() {
  SUModelRef model = SU_INVALID;
  SU_RESULT res = SUModelCreate(&model);
  assert(res == SU_ERROR_NONE);
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
  m_result(SUModelCreateFromFile(&m_model, file_path.c_str())),
  m_release_on_destroy(true)
{}

Model::Model(const Model& other):
	m_model(other.m_model),
  m_release_on_destroy(other.m_release_on_destroy)
{}

Model::~Model() {
  if (m_release_on_destroy && SUIsValid(m_model)) {
    SUModelRelease(&m_model);
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


Layer Model::active_layer() const {
	SULayerRef layer = SU_INVALID;
  SU_RESULT res = SUModelGetDefaultLayer(m_model, &layer);
  assert(res == SU_ERROR_NONE);
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
	std::vector<ComponentDefinition> defs = {definition};
  return add_definitions(defs);
}


bool Model::add_definitions(std::vector<ComponentDefinition>& definitions) {
  SU_RESULT res = SUModelAddComponentDefinitions(m_model, definitions.size(), definitions[0]);
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


std::vector<AttributeDictionary>	Model::attribute_dictionaries() const {
	size_t count = 0;
  SU_RESULT res = SUModelGetNumAttributeDictionaries(m_model, &count);
  assert(res == SU_ERROR_NONE);
  SUAttributeDictionaryRef dicts[count];
  res = SUModelGetAttributeDictionaries(m_model, count, &dicts[0], &count);
  assert(res == SU_ERROR_NONE);
	std::vector<AttributeDictionary> dictionaries;
  dictionaries.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	dictionaries.push_back(AttributeDictionary(dicts[i]));
  }
  return dictionaries;
}


AttributeDictionary Model::attribute_dictionary(const std::string& dict_name) const {
	SUAttributeDictionaryRef dict = SU_INVALID;
  SU_RESULT res = SUModelGetAttributeDictionary(m_model, dict_name.c_str(), &dict);
  assert(res == SU_ERROR_NONE);
	return AttributeDictionary(dict);
}



Axes Model::axes() const {
	SUAxesRef axes = SU_INVALID;
	SU_RESULT res = SUModelGetAxes(m_model, &axes);
  assert(res == SU_ERROR_NONE);
  return Axes(axes);
}


//Behavior behavior(); // TODO: this may not be possible to retrieve


Classifications Model::classifications() const {
	SUClassificationsRef classifications = SU_INVALID;
	SU_RESULT res = SUModelGetClassifications(m_model, &classifications);
  assert(res == SU_ERROR_NONE);
	return Classifications(classifications);
}

/*
* Returns the description attached to this model.
* @return description string object.
*/
//std::string description();
//bool description(std::string description_string);


std::vector<ComponentDefinition> Model::definitions() const {
	size_t count = 0;
	SU_RESULT res = SUModelGetNumComponentDefinitions(m_model, &count);
  assert(res == SU_ERROR_NONE);
  SUComponentDefinitionRef defs[count];
	res = SUModelGetComponentDefinitions(m_model, count, &defs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<ComponentDefinition> definitions;
  definitions.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	definitions.push_back(ComponentDefinition(defs[i]));
  }
  return definitions;
}


std::vector<ComponentDefinition> Model::group_definitions() const {
	size_t count = 0;
	SU_RESULT res = SUModelGetNumGroupDefinitions(m_model, &count);
  assert(res == SU_ERROR_NONE);
  SUComponentDefinitionRef defs[count];
	res = SUModelGetGroupDefinitions(m_model, count, &defs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<ComponentDefinition> definitions;
  definitions.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	definitions.push_back(ComponentDefinition(defs[i]));
  }
  return definitions;
}


Entities Model::entities() const {
	SUEntitiesRef entities = SU_INVALID;
	SU_RESULT res = SUModelGetEntities(m_model, &entities);
  assert(res == SU_ERROR_NONE);
	return Entities(entities);
}


//find_entity_by_id();  // TODO can this be done?

// TODO - build Location class before this method
/*
bool Model::georeferenced() const {
	SULocationRef loc = SU_INVALID;
	SU_RESULT res = SUModelGetLocation(m_model, &loc);
  assert(res == SU_ERROR_NONE);
  Location location(loc);
  if (loc) {
  	return true;
  }
  return false;
}
*/


TypedValue Model::get_attribute(const AttributeDictionary& dict, const std::string& key, const TypedValue& default_value) const {
	return dict.get_attribute(key, default_value);
}
TypedValue Model::get_attribute(const std::string& dict_name, const std::string& key, const TypedValue& default_value) const {
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
	size_t count = 0;
	SU_RESULT res = SUModelGetNumLayers(m_model, &count);
  assert(res == SU_ERROR_NONE);
  SULayerRef layer_refs[count];
  res = SUModelGetLayers(m_model, count, &layer_refs[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Layer> layers;
  layers.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	layers.push_back(Layer(layer_refs[i]));
  }
  return layers;
}

/*
* Returns the Location object of the model
* @return location Location object. If no location has been assigned to the model, the Location object returned will be invalid.
*/
// Location location();


std::vector<Material> Model::materials() const {
	size_t count = 0;
	SU_RESULT res = SUModelGetNumMaterials(m_model, &count);
  assert(res == SU_ERROR_NONE);
  SUMaterialRef mats[count];
	res = SUModelGetMaterials(m_model, count, &mats[0], &count);
  assert(res == SU_ERROR_NONE);
  std::vector<Material> materials;
  materials.reserve(count);
  for (size_t i=0; i < count; ++i) {
  	materials.push_back(Material(mats[i]));
  }
  return materials;
}


String Model::name() const {
	SUStringRef name = SU_INVALID;
	SUModelGetName(m_model, &name);
  return String(name);
}


bool Model::name(const String& name_string) {
  std::string std_string = name_string;
	SU_RESULT res = SUModelSetName(m_model, std_string.c_str());
  if (res == SU_ERROR_NONE) {
  	return true;
  }
  return false;
}


size_t Model::num_faces() const {
  ModelStatistics model_statistics((*this));
  return model_statistics.num_faces();
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


SU_RESULT Model::save(const std::string& file_path) {
  const char * c_string = file_path.c_str();
  SU_RESULT res = SUModelSaveToFile(m_model, c_string);
  return res;
}


bool Model::save_with_version(const std::string& file_path, SUModelVersion version) {
  SU_RESULT res = SUModelSaveToFileWithVersion(m_model, file_path.c_str(), version);
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
  return dict.set_attribute(key, value);
}

bool Model::set_attribute(const std::string& dict_name, const std::string& key, const TypedValue& value) {
	AttributeDictionary dict = attribute_dictionary(dict_name);
  return set_attribute(dict, key, value);
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
  SU_RESULT res = SUModelGetStatistics(model, &m_model_statistics);
  assert(res == SU_ERROR_NONE);
}
  
int ModelStatistics::num_faces() {
  return m_model_statistics.entity_counts[SUModelStatistics::SUEntityType_Face];
}
  
  

} /* namespace CW */
