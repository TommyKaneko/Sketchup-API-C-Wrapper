//
//  Model.hpp
//  SUEX_SkinUp
//
//  Created by Hidetomo Kaneko on 12/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>

#include <SketchUpAPI/model/model.h>

#include "Entities.hpp"
#include "Behavior.hpp"
#include "Classifications.hpp"
#include "ComponentDefinition.hpp"
#include "Layer.hpp"

namespace CW {

class Model {
	private:
  SUModelRef m_model;
  
  public:
  Model();
  Model(SUModelRef model_ref);
	
  /**
  * Returns SUModelRef object for the Model.
  */
  SUEdgeRef ref() const;

  /*
  * The class object can be converted to a SUModelRef without loss of data.
  */
  operator SUModelRef();
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
  Layer active_layer();
  
  /*
  * Sets the active layer of the model.
  * @param default_layer the Layer object to be the active layer on first openign the model
  * @return status true if succsessful
  */
  bool active_layer(Layer default_layer);
  
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
  AttributeDictionary attribute_dictionary(const std::string dict_name) const;
  
  /*
  * Returns the Axes object of the model.
  * @return axes Axes object of the model.
  */
  Axes axes();
  
  
  //Behavior behavior(); // TODO: this may not be possible to retrieve
  
  /*
	* Returns the Classifications object that is tied to the model.
  */
  Classifications classifications();
  
  /*
  * Returns the description attached to this model.
  * @return description string object.
  */
  //std::string description();
  //bool description(std::string description_string);
  
  /*
  * Returns the Entities object for this model.
  * @return entities Entities object
  */
  Entities entities();

	/*
  * Returns the list of ComponentDefinitions in this model
  * @return definitions vector array of definitions.
  */
  std::vector<ComponentDefinition> definitions();
	
  //find_entity_by_id();  // TODO can this be done?
  
  /*
  * Determine whether the model has been geolocated
  * @return true if georeferenced (the model is assigned a location), false if not.
  */
  bool georeferenced();
  
  /*
  * Returns the value of the specified attribute.
  * @param dict AttributeDictionary object in which to search for the attribute
  * @param key string key to find.
  * @param default_value if no attribute found, the default value to return
  */
  std::string get_attribute(AttributeDictionary dict, std::string key, std::string default_value);
  
  /*
  * Returns the GUID of the model.
  */
  //guid();
  
  /*
  * Returns the list of layers in the model.
  * @return layers a vector array of Layer objects in the model.
  */
  std::vector<Layer> layers();
 	
  /*
  * Returns the Location object of the model
  * @return location Location object. If no location has been assigned to the model, the Location object returned will be invalid.
  */
 	Location location();
 	
  /*
  * Returns the list of materials in the model.
  * @return materials vector array of Material objects in the model.
  */
  std::vector<Material> materials();
	
  /*
  * Returns the name of the model
  * @return name string of the model name.
  */
	String name();
  
  /*
  * Sets the name of the model.
  * @param name_string string to name the model with.
  */
	bool name(String name_string);
	
  /*
  * The number of faces in the model.  Useful for statistics.
  */
	size_t number_faces();
	
  /*
  * Returns a key=>value list of options for the model.
  * @see SUOptionsProviderRef
  */
  std::vector<std::pair<std::string, std::string>> options();
  
	/*
  * Returns the path of the model.
  */
  std::string path();
  
  /*
  * Returns the first Entity object that a ray from a given point and direction vector will hit.
  */
  //Entity raytest(Point3D point, Vector3D vector);
  
  /*
  * Saves the model in the file path given.
  * @param file_path string path to the file
  * @return true on success, false on failure
  */
  bool save(std::string file_path);

  /*
  * Saves the model in the file path given, in the given SU version.
  * @param file_path string path to the file
  * @param version SUModelVersion to save
  * @return true on success, false on failure
  */
  bool save_with_version(std::string file_path, SUModelVersion version);

  
  /*
  * Returns the array of Scene objects attached to the model.
  * @return scenes array of Scene objects.
  */
  std::vector<Scene> scenes();
  
  /*
  * Sets an attribute of the model.
  * @param AttributeDictionary object or string name of AttributeDictionary object that the attribute is in.
  * @param std::string attribute key.
  * @param std::string value to set
  * @return true on success, false on failure
  */
  bool set_attribute(AttributeDictionary dict, std::string key, std::string value);

	// set_datum()
	
  /*
  * Returns the ShadowInfo object of the model.
  */
  ShadowInfo shadow_info();
	
  /*
  * Returns the list of styles in the model.
  * @return styles vector array of Style objects
  */
  std::vector<Style> styles();
	
  // tags
	// tags=
  
  std::string title() { return name();}
  std::string title(std::string name_value) { return name(name_value);}
 
};

} /* namespace CW */

#endif /* Model_hpp */
