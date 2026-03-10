//
//  Scene.hpp
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

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include <vector>

#include <SketchUpAPI/model/scene.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class String;
class Camera;
class Layer;

class Scene :public Entity {
  private:
  static SUSceneRef create_scene();
  static SUSceneRef copy_reference(const Scene& other);

  public:
  /**
  * @brief Constructs a NULL Scene object.
  */
  Scene();

  /**
  * @brief Constructs a new standalone Scene with the given name.
  * @param name - the name of the scene. Should be unique within a model.
  */
  Scene(const std::string& name);

  /**
  * @brief Constructs a Scene from an existing SUSceneRef.
  * @param scene - SUSceneRef object.
  * @param attached - flag indicating whether the scene is owned by a model.
  */
  Scene(SUSceneRef scene, bool attached = true);

  /** @brief Copy Constructor */
  Scene(const Scene& other);

  /** @brief Destructor */
  ~Scene();

  /** @brief Copy assignment operator */
  Scene& operator=(const Scene& other);

  /**
  * @brief Returns the SUSceneRef object.
  */
  SUSceneRef ref() const;
  operator SUSceneRef() const;
  operator SUSceneRef*();

  /**
  * @brief Returns true if this is an invalid/null scene.
  */
  bool operator!() const;

  /**
  * @brief Sets the name of the scene object. Assumed to be UTF-8 encoded.
  *   Should be set before adding to a model. After adding to a model, setting
  *   a name that already exists will fail.
  * @since SketchUp 2016, API 4.0
  * @param name - the name to be set.
  */
  void name(const String& name);
  void name(const std::string& name);

  /**
  * @brief Retrieves the name of the scene object.
  * @since SketchUp 2016, API 4.0
  */
  String name() const;

  /**
  * @brief Sets the description of the scene object. Assumed to be UTF-8
  *   encoded.
  * @since SketchUp 2022.0, API 10.0
  * @param desc - the description to be set.
  */
  void description(const String& desc);
  void description(const std::string& desc);

  /**
  * @brief Retrieves the description of the scene object.
  * @since SketchUp 2022.0, API 10.0
  */
  String description() const;

  /**
  * @brief Retrieves the camera of the scene object. The returned camera
  *   object points to the scene's internal camera and must not be released.
  * @since SketchUp 2016, API 4.0
  */
  Camera camera() const;

  /**
  * @brief Sets the camera of the scene object. The scene does not take
  *   ownership of the provided camera, it just copies the properties to
  *   the scene's owned camera.
  * @since SketchUp 2016, API 4.0
  * @param camera - the camera object to be set.
  */
  void camera(const Camera& camera);

  /**
  * @brief Retrieves the "use camera" setting of the scene object.
  * @since SketchUp 2016, API 4.0
  */
  bool use_camera() const;

  /**
  * @brief Sets the "use camera" setting of the scene object.
  * @since SketchUp 2016, API 4.0
  * @param use - the setting for whether or not the camera is used.
  */
  void use_camera(bool use);

  /**
  * @brief Retrieves the "include in animation" setting of the scene object.
  * @since SketchUp 2018, API 6.0
  */
  bool include_in_animation() const;

  /**
  * @brief Sets the "include in animation" setting of the scene object.
  * @since SketchUp 2018, API 6.0
  * @param include - the new setting for whether or not to include in animation.
  */
  void include_in_animation(bool include);

  /**
  * @brief Retrieves the "use shadow info" setting of the scene object.
  * @since SketchUp 2015, API 3.0
  */
  bool use_shadow_info() const;

  /**
  * @brief Sets the "use shadow info" setting of the scene object.
  * @since SketchUp 2015, API 3.0
  * @param use - the new setting for whether or not shadow info is used.
  */
  void use_shadow_info(bool use);

  /**
  * @brief Retrieves the "use rendering options" setting of the scene object.
  * @since SketchUp 2016, API 4.0
  */
  bool use_rendering_options() const;

  /**
  * @brief Sets the "use rendering options" setting of the scene object.
  * @since SketchUp 2016, API 4.0
  * @param use - the new setting for whether or not rendering options are used.
  */
  void use_rendering_options(bool use);

  /**
  * @brief Retrieves the "use hidden layers" setting of the scene object.
  * @since SketchUp 2016, API 4.0
  */
  bool use_hidden_layers() const;

  /**
  * @brief Sets the "use hidden layers" setting of the scene object.
  * @since SketchUp 2016, API 4.0
  * @param use - the new setting for whether or not hidden layers are used.
  */
  void use_hidden_layers(bool use);

  /**
  * @brief Gets whether the scene uses the hidden geometry properties of
  *   root-level entities.
  * @since SketchUp 2020.1, API 8.1
  */
  bool use_hidden_geometry() const;

  /**
  * @brief Sets whether the scene uses the hidden properties of root-level
  *   entities.
  * @since SketchUp 2020.1, API 8.1
  * @param use - the new setting.
  */
  void use_hidden_geometry(bool use);

  /**
  * @brief Gets whether the scene uses the hidden properties of objects.
  * @since SketchUp 2020.1, API 8.1
  */
  bool use_hidden_objects() const;

  /**
  * @brief Sets whether the scene uses the hidden properties of objects.
  * @since SketchUp 2020.1, API 8.1
  * @param use - the new setting.
  */
  void use_hidden_objects(bool use);

  /**
  * @brief Retrieves the "use axes" setting of the scene object.
  * @since SketchUp 2017, API 5.0
  */
  bool use_axes() const;

  /**
  * @brief Sets the "use axes" setting of the scene object.
  * @since SketchUp 2017, API 5.0
  * @param use - the new setting for whether or not the axes is used.
  */
  void use_axes(bool use);

  /**
  * @brief Gets whether the scene uses section planes.
  * @since SketchUp 2018 M0, API 6.0
  */
  bool use_section_planes() const;

  /**
  * @brief Sets whether the scene uses section planes.
  * @since SketchUp 2018 M0, API 6.0
  * @param use - the new setting for whether or not section planes are used.
  */
  void use_section_planes(bool use);

  /**
  * @brief Sets the flags for the scene object. Flags are a bitwise
  *   combination of SUSceneFlags constants.
  * @since SketchUp 2017, API 5.0
  * @note If this function is called after the scene has been added to the
  *   model, the API user must call SUSceneUpdate().
  * @param flags - the flags to set.
  */
  void flags(uint32_t flags);

  /**
  * @brief Retrieves the flags of the scene object.
  * @since SketchUp 2017, API 5.0
  */
  uint32_t flags() const;

  /**
  * @brief Retrieves the "sketch axes displayed" setting of the scene object.
  * @since SketchUp 2017, API 5.0
  */
  bool sketch_axes_displayed() const;

  /**
  * @brief Sets the "sketch axes displayed" setting of the scene object.
  * @since SketchUp 2017, API 5.0
  * @param displayed - the new setting for whether or not sketch axes should
  *   be displayed.
  */
  void sketch_axes_displayed(bool displayed);

  /**
  * @brief Retrieves the layers associated with this scene.
  * @since SketchUp 2016, API 4.0
  */
  std::vector<Layer> layers() const;

  /**
  * @brief Adds the specified layer to the scene. Adding a layer to a scene
  *   will flag the layer to be the opposite of its default scene visibility.
  *   This function does not take ownership of the specified layer.
  * @since SketchUp 2016, API 4.0
  * @param layer - the layer to be added to the scene.
  */
  void add_layer(const Layer& layer);

  /**
  * @brief Removes the specified layer from the scene. Removing a layer from
  *   a scene will flag the layer to use its default scene visibility. Scenes
  *   do not own their layers so removing them doesn't release them.
  * @since SketchUp 2016, API 4.0
  * @param layer - the layer to be removed from the scene.
  */
  void remove_layer(const Layer& layer);

  /**
  * @brief Removes all of the layers from the scene. Scenes do not own their
  *   layers so removing them does not release them.
  * @since SketchUp 2016, API 4.0
  */
  void clear_layers();

  /**
  * @brief Copies the data from another scene to this scene.
  * @since SketchUp 2017, API 5.0
  * @param other - the scene to be copied.
  */
  void copy_from(const Scene& other);

  /**
  * @brief Activates this scene.
  * @since SketchUp 2017, API 5.0
  */
  void activate();

  /**
  * @brief Creates a copy of this scene object, duplicating all properties.
  * Note: camera is not copied in this method because SUSceneSetCamera only works
  * reliably on scenes that have been added to a model. Use
  * scene.camera(source.camera()) after adding the scene to a model.
  * @return a new unattached Scene object with the same properties.
  * @throws std::logic_error if this scene is null.
  */
  Scene copy() const;
};

} /* namespace CW */

#endif /* Scene_hpp */
