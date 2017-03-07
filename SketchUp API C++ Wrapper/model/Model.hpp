//
//  Model.hpp
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

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include <SketchUpAPI/model/model.h>

namespace CW {

  // Forward Declarations:
  class Entities;
  class Behavior;
  class Classifications;
  class ComponentDefinition;
  class Layer;
  class Axes;
  class AttributeDictionary;
  class TypedValue;
  class Material;
  class String;

class Model {
	private:
  SUModelRef m_model;
  bool m_release_on_destroy;
  
  SU_RESULT m_result = SU_ERROR_NONE; // the result on creation of model
  
  static SUModelRef create_model();
  
  public:
  Model();
  Model(SUModelRef model_ref, bool release_on_destroy = false);
  Model(std::string file_path);
	
  /** Copy Constructor **/
  Model(const Model& other);
  
  ~Model();
  
  /** Copy Assignment Operator **/
  Model& operator=(const Model& other);

  /**
  * Returns SUModelRef object for the Model.
  */
  SUModelRef ref() const;

  /*
  * The class object can be converted to a SUModelRef without loss of data.
  */
  operator SUModelRef() const;
  operator SUModelRef*();
  
  /*
  * Returns true if object is valid.
  */
  operator bool() const;
  
  /**
  * NOT operator.  Checks if the SUModelRef is invalid.
  * @return true if the model is invalid
  */
	bool operator!() const;
  
  /*
  * Returns active, or 'default' Layer object
  * @return layer Layer object that is the active layer
  */
  Layer active_layer() const;
  
  /*
  * Sets the active layer of the model.
  * @param default_layer the Layer object to be the active layer on first openign the model
  * @return status true if succsessful
  * TODO: default layer cannot be set through API
  */
  //bool active_layer(Layer default_layer);
  
  
  /**
  * Adds the Component Definition to the model
  * @param definition the ComponentDefinition object to add to the model.
  * @return true if definition was succeffully added, false, if error.
  */
  bool add_definition(const ComponentDefinition& definition);
  bool add_definitions(const std::vector<ComponentDefinition>& definitions);
  
  /*
  * The attribute_dictionaries method is used to retrieve the AttributeDictionaries collection attached to the model.
  * @return vector of AttributeDictionary objects associated with the model. If no AttributeDictionary objects are associated with the entity, an empty vector will be returned.
  */
  std::vector<AttributeDictionary>	attribute_dictionaries() const;
  
  /*
  * Retrieves an attribute dictionary object with a given name that is attached to an Entity.
  * @param dict_name string name of the AttributeDictionary object to get.  If it does not exist, a new AttributeDictionary object will be created with the name.
  * @return attribtue_dictionary AttributeDictionary of the smae name.
  */
  AttributeDictionary attribute_dictionary(const std::string& dict_name) const;
  
  /*
  * Returns the Axes object of the model.
  * @return axes Axes object of the model.
  */
  Axes axes() const;
  
  
  //Behavior behavior(); // TODO: this may not be possible to retrieve
  
  /*
	* Returns the Classifications object that is tied to the model.
  */
  Classifications classifications() const;
  
  /*
  * Returns the description attached to this model.
  * @return description string object.
  */
  //std::string description();
  //bool description(std::string description_string);

	/*
  * Returns the list of ComponentDefinitions in this model
  * @return definitions vector array of definitions.
  */
  std::vector<ComponentDefinition> definitions() const;
  
  /*
  * Returns the Entities object for this model.
  * @return entities Entities object
  */
  Entities entities() const;
	
  //find_entity_by_id();  // TODO can this be done?
  
  /*
  * Determine whether the model has been geolocated
  * @return true if georeferenced (the model is assigned a location), false if not.
  */
  // TODO build Location class before enablign this method;
  //bool georeferenced() const;
  
  /*
  * Returns the value of the specified attribute.
  * @param dict AttributeDictionary object in which to search for the attribute
  * @param key string key to find.
  * @param default_value if no attribute found, the default value to return
  */
  TypedValue get_attribute(const AttributeDictionary& dict, const std::string& key, const TypedValue& default_value) const;
  TypedValue get_attribute(const std::string& dict_name, const std::string& key, const TypedValue& default_value) const;
  
  /*
  * Returns the GUID of the model.
  */
  //guid();
  
  /*
  * Returns the list of layers in the model.
  * @return layers a vector array of Layer objects in the model.
  */
  std::vector<Layer> layers() const;
 	
  /*
  * Returns the Location object of the model
  * @return location Location object. If no location has been assigned to the model, the Location object returned will be invalid.
  */
 	// Location location();
 	
  /*
  * Returns the list of materials in the model.
  * @return materials vector array of Material objects in the model.
  */
  std::vector<Material> materials() const;
	
  /*
  * Returns the name of the model
  * @return name string of the model name.
  */
	String name() const;
  
  /*
  * Sets the name of the model.
  * @param name_string string to name the model with.
  */
	bool name(const String& name_string);
	
  /*
  * The number of faces in the model.  Useful for statistics.
  */
	size_t num_faces() const;
	
  /*
  * Returns a key=>value list of options for the model.
  * @see SUOptionsProviderRef
  */
  // TODO
  //std::vector<std::pair<std::string, std::string>> options() const;
  
	/*
  * Returns the path of the model.
  */
  // TODO - probably delete this, as there is no way to get the path of the model through the API.
  //std::string path() const;
  
  /*
  * Returns the first Entity object that a ray from a given point and direction vector will hit.
  */
  //Entity raytest(Point3D point, Vector3D vector);
  
  /*
  * Saves the model in the file path given.
  * @param file_path string path to the file
  * @return SU_RESULT - SU_ERROR_NONE on success. @see SUModelSaveFileToFile for possible return values
  */
  SU_RESULT save(const std::string& file_path);

  /*
  * Saves the model in the file path given, in the given SU version.
  * @param file_path string path to the file
  * @param version SUModelVersion to save
  * @return true on success, false on failure
  */
  bool save_with_version(const std::string& file_path, SUModelVersion version);

  
  /*
  * Returns the array of Scene objects attached to the model.
  * @return scenes array of Scene objects.
  */
  // std::vector<Scene> scenes();
  
  /*
  * Sets an attribute of the model.
  * @param AttributeDictionary object or string name of AttributeDictionary object that the attribute is in.
  * @param std::string attribute key.
  * @param std::string value to set
  * @return true on success, false on failure
  */
  bool set_attribute(AttributeDictionary& dict, const std::string& key, const TypedValue& value);
  bool set_attribute(const std::string& dict_name, const std::string& key, const TypedValue& value);
  
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
 
};

class ModelStatistics {
  private:
  SUModelStatistics m_model_statistics;
  
  public:
  ModelStatistics(SUModelStatistics model_statistics);
  ModelStatistics(const Model& model);
  
  /**
  * Return the number of faces in the model.
  */
  int num_faces();
  
};

} /* namespace CW */

#endif /* Model_hpp */
