//
//  Model.hpp
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
  class RenderingOptions;
  class ShadowInfo;
  class OptionsManager;
  class InstancePath;


class Model {
  private:
  SUModelRef m_model;
  bool m_release_on_destroy;

  SUResult m_result = SU_ERROR_NONE; // the result on creation of model

  static SUModelRef create_model();

  public:
  Model();
  Model(SUModelRef model_ref, bool release_on_destroy = true);
  Model(std::string file_path);

  /**
   * A note on Copy Constructors and Move.
   * SketchUp C API requires that models are explicitly released to free memory, once all calls to it are completed.
   * This presents challenges when trying to manage ownership of the underlying SUModelRef in C++ wrapper classes.
   * To address this, one approach may be for the Model class to provides both copy and move semantics.  However, as testing has given inconsistent results, the recommendation is to allocate a model like this:
   CW::Model* model = new CW::Model();
   // Do some work with model
   delete model;
   *
   * To complicate matters further, when using the SketchUp Live C API, it is important that the model is NOT released, as this is owned by the Sketchup Application.  See CW::Application::get_active_model()
   */
  /** Copy Constructor - non-owning copy (does not release on destruction) **/
  //private:
  Model(const Model& other);

  /** Move Constructor - transfers ownership **/
  Model(Model&& other) noexcept;

  ~Model();

  /** Copy Assignment Operator - non-owning copy **/
  Model& operator=(const Model& other);

  /** Move Assignment - transfers ownership **/
  Model& operator=(Model&& other) noexcept;

  /**
  * @brief Equality operator for two entity objects
  */
  bool operator==(const Model& rhs) const;

  /**
  * @brief Non-equality operator for two entity objects
  */
  bool operator!=(const Model& rhs) const;

  //public:
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

  /**
  * Returns a string of the model version in the form "major.minor.build"
  */
  std::string version_string() const;

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
  bool add_definition(ComponentDefinition& definition);
  bool add_definitions(std::vector<ComponentDefinition>& definitions);

  /*
  * The attribute_dictionaries method is used to retrieve the AttributeDictionaries collection attached to the model.
  * @return vector of AttributeDictionary objects associated with the model. If no AttributeDictionary objects are associated with the entity, an empty vector will be returned.
  */
  std::vector<AttributeDictionary>  attribute_dictionaries() const;

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
  * Returns the list of Group ComponentDefinitions in this model
  * @return definitions vector array of definitions.
  */
  std::vector<ComponentDefinition> group_definitions() const;

  /**
  * Returns the InstancePath of the given persistent ID in this model.
  * @return InstancePath object of the given persistent ID.
  */
  InstancePath instance_path(const String& persistent_id) const;

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

  /**
   * @brief Returns the number of layers in the model
   */
  size_t num_layers() const;

  /*
  * Returns the list of layers in the model.
  * @return layers a vector array of Layer objects in the model.
  */
  std::vector<Layer> layers() const;

  /**
  * Add layers to the model.
  * @param layers - vector of layer objects to add.  The layers must not be attached to any other model object.
  * @param overwrite_existing - where an added layer matches the name of an existing layer, overwrite the properties of the existing layer with the added layer.
  */
  void add_layers(std::vector<Layer>& layers, bool overwrite_existing = false);

  /**
  * Checks if the given layer is in the list of layers in this model.  This is an expensive method, and is recommended to be used only in debugging.
  * @param layer - the layer object to check
  * @param strict - default true.  If true, the check will ensure the layers being compared are the same object.  If false, only the layer names will be checked
  * @return true if the layer is in the model.
  */
  bool layer_exists(const Layer& layer, bool strict = true) const;

  /*
  * Returns the Location object of the model
  * @return location Location object. If no location has been assigned to the model, the Location object returned will be invalid.
  */
   // Location location();

  /**
   * @brief Returns the number of materials in the model
   */
  size_t num_materials() const;

  /*
  * Returns the list of materials in the model.
  * @return materials vector array of Material objects in the model.
  */
  std::vector<Material> materials() const;

  /**
  * Add materials to the model.
  * @param materials - vector of material objects which must not be attached to any other model object.
  * @param overwrite_existing - where an added material matches the name of an existing material, overwrite the properties of the existing material with the added material.
  */
  void add_materials(std::vector<Material>& materials, bool overwrite_existing = false);

  /**
  * Checks if the given material is in the list of materials in this model.  This is a very expensive method, and is recommended to be used only in debugging.
  * @param material - the material object to check
  * @return true if the material is in the model.
  */
  bool material_exists(const Material& material) const;

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
  OptionsManager options();

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
  * @return SUResult - SU_ERROR_NONE on success. @see SUModelSaveFileToFile for possible return values
  */
  SUResult save(const std::string& file_path);

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
  ShadowInfo shadow_info();

  /*
  * Returns the list of styles in the model.
  * @return styles vector array of Style objects
  */
  // std::vector<Style> styles();

  // tags
  // tags=

  //std::string title() { return name();}
  //std::string title(std::string name_value) { return name(name_value);}

  /// RenderingOptions
  RenderingOptions rendering_options();

}; // class Model


class ModelStatistics {
  private:
  SUModelStatistics m_model_statistics;

  public:
  ModelStatistics(SUModelStatistics model_statistics);
  ModelStatistics(const Model& model);

  /**
  * Return the number of faces in the model.
  */
  int faces() const;
  int edges() const;
  int instances() const;
  int groups() const;
  int images() const;
  int definitions() const;
  int layers() const;
  int materials() const;
};

} /* namespace CW */

#endif /* Model_hpp */
