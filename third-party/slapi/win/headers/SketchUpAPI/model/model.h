// Copyright 2013 Trimble Navigation Ltd. All Rights Reserved.

#ifndef SKETCHUP_MODEL_MODEL_H_
#define SKETCHUP_MODEL_MODEL_H_

#include <SketchUpAPI/common.h>
#include <SketchUpAPI/unicodestring.h>
#include <SketchUpAPI/model/defs.h>

#pragma pack(push, 8)

#if defined(__APPLE__)
// This is added to remove the missing declarations warning on the mac
// for an enum defined inside a structure. [SUEntityType]
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
@struct SUModelRef
@brief A SketchUp model.
*/

/**
@struct SUModelStatistics
@brief Contains an array of entity counts that can be indexed per entity type.
*/
struct SUModelStatistics {
/**
@enum SUEntityType
@brief Types of \ref SUEntityRef objects.
*/
  enum SUEntityType {
    SUEntityType_Edge = 0,
    SUEntityType_Face,
    SUEntityType_ComponentInstance,
    SUEntityType_Group,
    SUEntityType_Image,
    SUEntityType_ComponentDefinition,
    SUEntityType_Layer,
    SUEntityType_Material,
    SUNumEntityTypes
  };
  int entity_counts[SUNumEntityTypes];
};

/**
@enum SUModelUnits
@brief Units options settings
*/
enum SUModelUnits {
  SUModelUnits_Inches,
  SUModelUnits_Feet,
  SUModelUnits_Millimeters,
  SUModelUnits_Centimeters,
  SUModelUnits_Meters
};

/**
@enum SUModelVersion
@brief SketchUp model file format version
@since SketchUp 2014, API 2.0
*/
enum SUModelVersion {
  SUModelVersion_SU3,
  SUModelVersion_SU4,
  SUModelVersion_SU5,
  SUModelVersion_SU6,
  SUModelVersion_SU7,
  SUModelVersion_SU8,
  SUModelVersion_SU2013,
  SUModelVersion_SU2014,
  SUModelVersion_SU2015,
  SUModelVersion_SU2016,
  SUModelVersion_SU2017,
  SUModelVersion_SU2018
};

/**
@brief Creates an empty model object for the purposes of writing a SketchUp
        document. This model object must be released with \ref SUModelRelease.
@param[out] model The model object created.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_OUTPUT if model is NULL
- \ref SU_ERROR_OVERWRITE_VALID if model is already a valid object
*/
SU_RESULT SUModelCreate(SUModelRef* model);

/**
@brief Creates a model from a SketchUp file on local disk.  This model object
       must be released with \ref SUModelRelease.
@param[out] model     The model object created.
@param[in]  file_path The source file path of the SketchUp file. Assumed to be
                      UTF-8 encoded.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_NULL_POINTER_INPUT if file_path is NULL
- \ref SU_ERROR_NULL_POINTER_OUTPUT if model is NULL
- \ref SU_ERROR_OVERWRITE_VALID if model is already a valid object
- \ref SU_ERROR_SERIALIZATION if an error occurs during reading of the file
- \ref SU_ERROR_MODEL_INVALID if the file specified by file_path is an invalid
  model. (since SketchUp 2014, API 2.0)
- \ref SU_ERROR_MODEL_VERSION if the file has objects that have a newer version
  than is supported by the current build of slapi. (since SketchUp 2014, API 2.0)
*/
SU_RESULT SUModelCreateFromFile(SUModelRef* model, const char* file_path);


/**
@brief Creates a model from a SketchUp skp file buffer.  This model object must 
       be released with \ref SUModelRelease.
@since SketchUp 2017 M2, API 5.2
@param[out] model       The model object created.
@param[in]  buffer      The SketchUp file buffer.
@param[in]  buffer_size The SketchUp file buffer size.
 @return
 - \ref SU_ERROR_NONE on success
 - \ref SU_ERROR_NULL_POINTER_INPUT if buffer is NULL
 - \ref SU_ERROR_NULL_POINTER_OUTPUT if model is NULL
 - \ref SU_ERROR_OVERWRITE_VALID if model is already a valid object
 - \ref SU_ERROR_SERIALIZATION if an error occurs during reading of the file
 - \ref SU_ERROR_MODEL_INVALID if the file specified by buffer is an invalid
 model.
 - \ref SU_ERROR_MODEL_VERSION if the file has objects that have a newer version
 than is supported by the current build of slapi.
 */
SU_RESULT SUModelCreateFromBuffer(SUModelRef* model,
                                  const unsigned char* buffer,
                                  size_t buffer_size);

/**
@brief Releases a model object and its associated resources. The root component
       of the model object and all its child objects must not be released
       explicitly
@param[in] model The model object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_INPUT if model is NULL
*/
SU_RESULT SUModelRelease(SUModelRef* model);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/**
@brief Returns a model reference for a given internal model representation.
        This model object must NOT be released with \ref SUModelRelease.
@param[in] data Internal model representation.
@return The created model reference.
*/
SU_EXPORT SUModelRef SUModelFromExisting(uintptr_t data);
#endif // DOXYGEN_SHOULD_SKIP_THIS

/**
@brief Retrieves model entities.
@param[in]  model    The model object.
@param[out] entities The entities retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if entities is NULL
*/
SU_RESULT SUModelGetEntities(SUModelRef model,
                             SUEntitiesRef* entities);

/**
@brief Retrieves the number of materials in a model object.
@param[in]  model The model object.
@param[out] count The number of material objects available.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUModelGetNumMaterials(SUModelRef model, size_t* count);

/**
@brief Retrieves all the materials associated with a model object.
@param[in]  model     The model object.
@param[in]  len       The number of material objects to retrieve.
@param[out] materials The material objects retrieved.
@param[out] count     The number of material objects retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if materials or count is NULL
*/
SU_RESULT SUModelGetMaterials(SUModelRef model, size_t len,
                              SUMaterialRef materials[], size_t* count);

/**
@brief Adds materials to a model object.
@param[in] model     The model object.
@param[in] len       The number of material objects to add.
@param[in] materials The array of material objects to add.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if materials is NULL
*/
SU_RESULT SUModelAddMaterials(SUModelRef model, size_t len,
                              const SUMaterialRef materials[]);

/**
@brief Retrieves the number of components associated with a model.
@param[in]  model The model object.
@param[out] count The number of components available.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUModelGetNumComponentDefinitions(SUModelRef model,
                                            size_t* count);

/**
@brief Retrieves the component definitions that define component instances but
        not groups.
@param[in]  model       The model object.
@param[in]  len         The number of component definitions to retrieve.
@param[out] definitions The component definitions retrieved.
@param[out] count       The number of component definitions retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if definitions or count is NULL
*/
SU_RESULT SUModelGetComponentDefinitions(SUModelRef model, size_t len,
                                         SUComponentDefinitionRef definitions[],
                                         size_t* count);

/**
@brief Retrieves the number of component definitions that define groups.
@param[in]  model The model object.
@param[out] count The number of component definitions available.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUModelGetNumGroupDefinitions(SUModelRef model,
                                        size_t* count);

/**
@brief Retrieves the component definitions that define groups.
@param[in]  model       The model object.
@param[in]  len         The number of component definitions to retrieve.
@param[out] definitions The component definitions retrieved.
@param[out] count       The number of component definitions retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if definitions or count is NULL
*/
SU_RESULT SUModelGetGroupDefinitions(SUModelRef model, size_t len,
                                     SUComponentDefinitionRef definitions[],
                                     size_t* count);

/**
@brief Adds component definitions to a model object.
@param[in] model      The model object.
@param[in] len        The number of component definitions to add.
@param[in] components The array of component definitions to add.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if components is NULL
*/
SU_RESULT SUModelAddComponentDefinitions(SUModelRef model, size_t len,
    const SUComponentDefinitionRef components[]);

/**
@brief Saves the model to a file.
@param[in] model     The model object.
@param[in] file_path The file path destination of the serialization operation.
                     Assumed to be UTF-8 encoded.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if file_path is NULL
- \ref SU_ERROR_SERIALIZATION if the serialization operation itself fails
*/
SU_RESULT SUModelSaveToFile(SUModelRef model, const char* file_path);

/**
@brief Saves the model to a file using a specific SketchUp version format.
@since SketchUp 2014, API 2.0
@param[in] model     The model object.
@param[in] file_path The file path destination of the serialization operation.
                     Assumed to be UTF-8 encoded.
@param[in] version   The SKP file format version to use when saving.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_MODEL_VERSION if version is invalid
- \ref SU_ERROR_NULL_POINTER_INPUT if file_path is NULL
- \ref SU_ERROR_SERIALIZATION if the serialization operation itself fails
*/
SU_RESULT SUModelSaveToFileWithVersion(SUModelRef model, const char* file_path,
                                       enum SUModelVersion version);

/**
@brief Retrieves the camera of a model object. The returned camera object
        points to model's internal camera. So it must not be released via
        \ref SUCameraRelease.
@param[in]  model  The model object.
@param[out] camera The camera object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if camera is NULL
*/
SU_RESULT SUModelGetCamera(SUModelRef model, SUCameraRef* camera);

/**
@brief Sets the current camera of a model object.
@since SketchUp 2016, API 4.0
@param[in] model  The model object.
@param[in] camera The camera object. This reference will become invalid when
                  this function returns.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_INVALID_INPUT if camera is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if camera is NULL
*/
SU_RESULT SUModelSetCamera(SUModelRef model, SUCameraRef* camera);

/**
@brief Retrieves the number of scene cameras of a model object.
@param[in]  model      The model object.
@param[out] num_scenes The number of scenes available.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if num_scenes is NULL
*/
SU_RESULT SUModelGetNumScenes(SUModelRef model, size_t* num_scenes);

/**
@brief Retrieves the number of layers in a model object.
@param[in]  model The model object.
@param[out] count The number of layers available.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT of model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUModelGetNumLayers(SUModelRef model, size_t* count);

/**
@brief Retrieves the layers in a model object.
@param[in]  model  The model object.
@param[in]  len    The number of layers to retrieve.
@param[out] layers The layers retrieved.
@param[out] count  The number of layers retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT of model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if layers or count is NULL
*/
SU_RESULT SUModelGetLayers(SUModelRef model, size_t len,
                           SULayerRef layers[], size_t* count);

/**
@brief Adds layer objects to a model object.
@param[in] model  The model object.
@param[in] len    The number of layers to add.
@param[in] layers The layers to add.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT of model is not a valid object
- \ref SU_ERROR_INVALID_INPUT if any item in layers is not a valid object
*/
SU_RESULT SUModelAddLayers(SUModelRef model, size_t len,
                           const SULayerRef layers[]);

/**
@brief Retrieves the default layer object of a model object.
@param[in]  model The model object.
@param[out] layer The layer object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT of model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if layer is NULL
*/
SU_RESULT SUModelGetDefaultLayer(SUModelRef model, SULayerRef* layer);

/**
@brief Retrieves the version of a model object.  The version consists of three
numbers: major version number, minor version number, and the build number.
@param[in]  model The model object.
@param[out] major The major version number retrieved.
@param[out] minor The minor version number retrieved.
@param[out] build The build version number retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT of model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if major, minor, or build is NULL
*/
SU_RESULT SUModelGetVersion(SUModelRef model, int* major, int* minor,
                            int* build);

/**
@brief Retrieves the number of attribute dictionaries of a model object.
@param[in]  model The model object.
@param[out] count The number of attribute dictionaries available.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUModelGetNumAttributeDictionaries(SUModelRef model,
                                             size_t* count);

/**
@brief Retrieves the attribute dictionaries of a model object.
@param[in]  model        The model object.
@param[in]  len          The number of attribute dictionaries to retrieve.
@param[out] dictionaries The dictionaries retrieved.
@param[out] count        The number of attribute dictionaries retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if dictionaries or count is NULL
*/
SU_RESULT SUModelGetAttributeDictionaries(
    SUModelRef model,
    size_t len,
    SUAttributeDictionaryRef dictionaries[],
    size_t* count);

/**
@brief Retrieves the attribute dictionary of a model object that has the given
        name. If a dictionary with the given name does not exist, one is added
        to the model object.
@param[in] model       The model object.
@param[in] name        The name of the attribute dictionary to retrieve. Assumed
                       to be UTF-8 encoded.
@param[out] dictionary The dictionary object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if name is NULL
- \ref SU_ERROR_NULL_POINTER_OUTPUT if dictionary is NULL
*/
SU_RESULT SUModelGetAttributeDictionary(SUModelRef model, const char* name,
                                        SUAttributeDictionaryRef* dictionary);

/**
@brief Retrieves whether the model is georeferenced.
@since SketchUp 2017, API 5.0
@param[in]  model      The model object.
@param[out] is_geo_ref The flag retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if is_used is NULL
*/
SU_RESULT SUModelIsGeoReferenced(SUModelRef model, bool* is_geo_ref);

/**
@brief Retrieves the location information of a given model.
@param[in]  model    The model object.
@param[out] location The location retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if location is NULL
*/
SU_RESULT SUModelGetLocation(SUModelRef model,
                             SULocationRef* location);

/**
@brief Calculates the sum of all entities by type in the model.
@param[in]  model      The model object.
@param[out] statistics The \ref SUModelStatistics struct that will be populated
                       with the number of each entity type in the model.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if statistics is NULL
*/
SU_RESULT SUModelGetStatistics(SUModelRef model,
                               struct SUModelStatistics* statistics);

/**
@brief Georeferences the model.
@param[in] model               The model object.
@param[in] latitude            Latitude of the model.
@param[in] longitude           Longitude of the model.
@param[in] altitude            Altitude of the model.
@param[in] is_z_value_centered Indicates if z value should be centered.
@param[in] is_on_ocean_floor   Indicates whether the model is on the ocean
                               floor.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object or if latitude or
  longitude does not lie within a valid range
*/
SU_RESULT SUModelSetGeoReference(SUModelRef model, double latitude,
                                 double longitude, double altitude,
                                 bool is_z_value_centered,
                                 bool is_on_ocean_floor);

/**
@brief Retrieves the rendering options of a model object.
@param[in]  model             The model object.
@param[out] rendering_options The rendering options object retrieved. This
                              object is owned by the model and must not be
                              explicitly released.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if rendering_options is NULL
- \ref SU_ERROR_NO_DATA if no rendering options is available
*/
SU_RESULT SUModelGetRenderingOptions(SUModelRef model,
                                     SURenderingOptionsRef* rendering_options);

/**
@brief Retrieves the shadow info of a model object.
@since SketchUp 2015, API 3.0
@param[in]  model       The model object.
@param[out] shadow_info The shadow info object retrieved. This object is owned
                        by the model and must not be explicitly released.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if shadow_info is NULL
- \ref SU_ERROR_NO_DATA if no shadow info is available
*/
SU_RESULT SUModelGetShadowInfo(SUModelRef model,
                               SUShadowInfoRef* shadow_info);

/**
@brief Retrieves options manager associated with the model.
@param[in]  model           The model object.
@param[out] options_manager The options manager object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if options_manager is NULL
*/
SU_RESULT SUModelGetOptionsManager(SUModelRef model,
                                   SUOptionsManagerRef* options_manager);

/**
@brief Retrieves the angle which will rotate the north direction to the y-axis
        for a given model.
@param[in]  model            The model object.
@param[out] north_correction The north correction angle retrieved (in degrees).
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if north_angle is NULL
*/
SU_RESULT SUModelGetNorthCorrection(SUModelRef model,
                                    double* north_correction);

/**
@brief Merges all adjacent, coplanar faces in the model.
@param[in] model The model object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid
*/
SU_RESULT SUModelMergeCoplanarFaces(SUModelRef model);

/**
@brief Retrieves all the scenes associated with a model object.
@param[in]  model  The model object.
@param[in]  len    The number of scene objects to retrieve.
@param[out] scenes The scene objects retrieved.
@param[out] count  The number of scene objects retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if scenes or count is NULL
- \ref SU_ERROR_OVERWRITE_VALID if any element of scenes is already a valid
  object
- \ref SU_ERROR_NO_DATA if there are no scene objects to retrieve.
*/
SU_RESULT SUModelGetScenes(SUModelRef model, size_t len,
                           SUSceneRef scenes[], size_t* count);

/**
@brief Retrieves the scenes with the given name associated with a model object.
@param[in]  model The model object.
@param[in]  name  The name of scene object to retrieve.
@param[out] scene The scene object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if name is NULL
- \ref SU_ERROR_NULL_POINTER_OUTPUT if scene is NULL
- \ref SU_ERROR_NO_DATA if there are no scene objects to retrieve.
*/
SU_RESULT SUModelGetSceneWithName(SUModelRef model, const char* name,
                                  SUSceneRef* scene);

/**
@brief Adds scenes to a model object.
@warning *** Breaking Change: The behavior of SUModelAddScenes changed in
         SketchUp SDK 2018 API 6.0 to return SU_ERROR_INVALID_ARGUMENT if at 
         least one scene name already exists in the model or if there are 
         duplicated names in the scenes array.
@param[in] model  The model object.
@param[in] len    The number of scene objects to add.
@param[in] scenes The array of scene objects to add.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if scenes is NULL
- \ref SU_ERROR_INVALID_ARGUMENT if the names of the given scenes are not unique
       among themselves or among existing scenes
*/
SU_RESULT SUModelAddScenes(SUModelRef model, size_t len,
                           const SUSceneRef scenes[]);

/**
@brief Adds scenes to a model object.
@warning *** Breaking Change: The behavior of SUModelAddScene changed in 
         SketchUp SDK 2018 API 6.0 to return SU_ERROR_INVALID_ARGUMENT if the 
         given scene name already exists in the model.
@param[in]  model     The model object.
@param[in]  index     Where in the list to add the scene. -1 to place at the end.
@param[in]  scene     The scene object to add.
@param[out] out_index The index that the scene was added at.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model or scene are not a valid object
- \ref SU_ERROR_INVALID_ARGUMENT if a scene with the same name already exists
*/
SU_RESULT SUModelAddScene(SUModelRef model, int index,
                          SUSceneRef scene, int* out_index);

/**
@brief Retrieves the active scene associated with a model object.
@since SketchUp 2016, API 4.0
@param[in]  model The model object.
@param[out] scene The scene object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if scene is NULL
- \ref SU_ERROR_OVERWRITE_VALID if scene is already a valid object
- \ref SU_ERROR_NO_DATA if there is no active scene to retrieve
*/
SU_RESULT SUModelGetActiveScene(SUModelRef model, SUSceneRef *scene);

/**
@brief Sets the provided scene as the active scene.
@since SketchUp 2016, API 4.0
@param[in] model The model object.
@param[in] scene The scene object to be set as the active scene.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model or scene is not a valid object
- \ref SU_ERROR_GENERIC if try to activate a scene which is not in the model
*/
SU_RESULT SUModelSetActiveScene(SUModelRef model, SUSceneRef scene);


/**
@brief Adds a single matched photo scene to a model object.
@since SketchUp 2015, API 3.0
@param[in]  model      The model object.
@param[in]  image_file The full path of the image associated with this scene.
@param[in]  camera     The camera associated with this scene.
@param[in]  scene_name The name of the scene to add.
@param[out] scene      The scene object created.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model or camera is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if scene_name or image_file is NULL
- \ref SU_ERROR_INVALID_OUTPUT if scene is NULL
- \ref SU_ERROR_GENERIC if image_file is invalid or not found
*/
SU_RESULT SUModelAddMatchPhotoScene(SUModelRef model,
                                    const char* image_file,
                                    SUCameraRef camera,
                                    const char* scene_name,
                                    SUSceneRef *scene);

/**
@brief Retrieves the name of a model object.
@param[in]  model The model object.
@param[out] name  The destination of the retrieved name object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if name is NULL
- \ref SU_ERROR_INVALID_OUTPUT if name does not point to a valid \ref
  SUStringRef object
*/
SU_RESULT SUModelGetName(SUModelRef model, SUStringRef* name);

/**
@brief Sets the name of a model object.
@param[in] model The model object.
@param[in] name  The name of the model object. Assumed to be UTF-8 encoded.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_INPUT if name is NULL
*/
SU_RESULT SUModelSetName(SUModelRef model, const char* name);

/**
@brief Retrieves the file path of a model object.
@since SketchUp 2018, API 6.0
@param[in]  model  The model object.
@param[out] path   The destination of the retrieved path object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if path is NULL
- \ref SU_ERROR_INVALID_OUTPUT if path does not point to a valid \ref
  SUStringRef object
*/
SU_RESULT SUModelGetPath(SUModelRef model, SUStringRef* path);

/**
@brief Retrieves the title of a model object.
@since SketchUp 2018, API 6.0
@param[in]  model  The model object.
@param[out] title  The destination of the retrieved title object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if title is NULL
- \ref SU_ERROR_INVALID_OUTPUT if title does not point to a valid \ref
  SUStringRef object
*/
SU_RESULT SUModelGetTitle(SUModelRef model, SUStringRef* title);

/**
@brief Retrieves the description of a model object.
@param[in]  model        The model object.
@param[out] description  The destination of the retrieved description object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if description is NULL
- \ref SU_ERROR_INVALID_OUTPUT if description does not point to a valid \ref
  SUStringRef object
*/
SU_RESULT SUModelGetDescription(SUModelRef model, SUStringRef* description);

/**
@brief Sets the description of a model object.
@param[in] model        The model object.
@param[in] description  The description of the model object. Assumed to be UTF-8
                        encoded.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_INPUT if description is NULL
*/
SU_RESULT SUModelSetDescription(SUModelRef model, const char* description);

/**
@brief Returns the units associated with the given model.
@param[in]  model The model object.
@param[out] units The units retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is an invalid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if the units argument is invalid
*/
SU_RESULT SUModelGetUnits(SUModelRef model, enum SUModelUnits* units);

/**
@brief Retrieves the classifications of a model object.
@param[in]  model            The model object.
@param[out] classifications  The classifications object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if classifications is NULL
*/
SU_RESULT SUModelGetClassifications(SUModelRef model,
  SUClassificationsRef* classifications);

/**
@brief Retrieves the axes of a model object.
@param[in]  model The model object.
@param[out] axes  The axes object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if axes is NULL
*/
SU_RESULT SUModelGetAxes(SUModelRef model, SUAxesRef* axes);

/**
@brief Retrieves the styles of a model object.
@since SketchUp 2017, API 5.0
@param[in]  model  The model object.
@param[out] styles The styles object retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if manager is NULL
*/
SU_RESULT SUModelGetStyles(SUModelRef model, SUStylesRef* styles);

/**
@brief Retrieves the instance path (including an entity) corresponding
       to a given persistent id.
@param[in]  model             The model object.
@param[in]  pid_ref            Persistent id of the entity.
@param[out] instance_path_ref  Instance path to the entity.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model or pid_ref are not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if instance_path_ref is NULL
- \ref SU_ERROR_INVALID_OUTPUT if instance_path_ref is not a valid object
- \ref SU_ERROR_PARTIAL_SUCCESS if an instance path can not be is fully traced
- \ref SU_ERROR_GENERIC on general failure
*/
SU_RESULT SUModelGetInstancePathByPid(SUModelRef model, SUStringRef pid_ref,
                                      SUInstancePathRef* instance_path_ref);

/**
@brief Retrieves the number of fonts in a model object.
@since SketchUp 2017, API 5.0
@param[in]  model The model object.
@param[out] count The number of font objects available.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if count is NULL
*/
SU_RESULT SUModelGetNumFonts(SUModelRef model, size_t* count);

/**
@brief Retrieves all the materials associated with a model object.
@since SketchUp 2017, API 5.0
@param[in]  model The model object.
@param[in]  len   The number of material objects to retrieve.
@param[out] fonts The font objects retrieved.
@param[out] count The number of material objects retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if fonts or count is NULL
*/
SU_RESULT SUModelGetFonts(SUModelRef model, size_t len, SUFontRef fonts[],
    size_t* count);

/**
@brief Retrieves the dimension style associated with a model object.
@since SketchUp 2017, API 5.0
@param[in]  model The model object.
@param[out] style The dimension style retrieved.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if style is NULL
*/
SU_RESULT SUModelGetDimensionStyle(SUModelRef model,
    SUDimensionStyleRef* style);

/**
@brief Retrieves length formatter settings from the model. The given length 
       formatter object must have been constructed using \ref
       SULengthFormatterCreate. It must be released using \ref
       SULengthFormatterRelease.
@since SketchUp 2018, API 6.0
@param[in]  model     The model object.
@param[out] formatter The formatter used to retrieve the settings.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_OUTPUT if formatter is NULL
- \ref SU_ERROR_INVALID_OUTPUT if formatter does not point to a valid \ref
       SULengthFormatterRef object
*/
SU_RESULT SUModelGetLengthFormatter(SUModelRef model,
    SULengthFormatterRef* formatter);

/**
@brief Retrieves a unique material name from the model that is based on the
       provided one. If the provided name is unique it will be returned,
       otherwise any trailing indices will be replaced by a new index.
@since SketchUp 2018, API 6.0
@param[in]  model    The model object.
@param[in]  in_name  The suggested name.
@param[out] out_name The returned name.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
- \ref SU_ERROR_NULL_POINTER_INPUT if in_name is NULL
- \ref SU_ERROR_NULL_POINTER_OUTPUT if out_name is NULL
- \ref SU_ERROR_INVALID_OUTPUT if out_name does not point to a valid \ref
       SUStringRef object
*/
SU_RESULT SUModelGenerateUniqueMaterialName(SUModelRef model,
    const char* in_name, SUStringRef* out_name);

/**
@brief Fixes any errors found in the given model.
@since SketchUp 2018, API 6.0
@param[in]  model  The model object.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
*/
SU_RESULT SUModelFixErrors(SUModelRef model);
  
/**
@brief Updates the faces in the model so that they are oriented
       consistently.
@since SketchUp 2018, API 6.0
@param[in]  model              The model object.
@param[in]  recurse_components Orient components of the model.
@return
- \ref SU_ERROR_NONE on success
- \ref SU_ERROR_INVALID_INPUT if model is not a valid object
*/
SU_RESULT SUModelOrientFacesConsistently(SUModelRef model, 
    bool recurse_components);
#ifdef __cplusplus
}
#endif

#if defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#pragma pack(pop)

#endif  // SKETCHUP_MODEL_MODEL_H_
