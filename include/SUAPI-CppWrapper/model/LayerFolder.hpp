//
//  LayerFolder.hpp
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

#ifndef LayerFolder_hpp
#define LayerFolder_hpp

#include <string>
#include <vector>
#include <SketchUpAPI/model/layer_folder.h>

#include "SUAPI-CppWrapper/model/Entity.hpp"

namespace CW {

// Forward declarations
class Layer;
class String;

/**
 * @brief C++ wrapper for SULayerFolderRef.
 *
 * A LayerFolder represents a "Tag Folder" in the SketchUp UI. It groups
 * layers (tags) together for organizational purposes. LayerFolders can be
 * nested and can contain any number of layers.
 *
 * @note "Layer" is SketchUp's legacy API name for what the UI calls "Tags".
 *
 * @see SULayerFolderRef
 * @since SketchUp 2021.0, API 9.0
 */
class LayerFolder :public Entity {
  private:
  static SULayerFolderRef create_layer_folder(const std::string& name);
  static SULayerFolderRef copy_reference(const LayerFolder& other);

  public:
  /**
  * @brief Constructs a NULL LayerFolder object.
  */
  LayerFolder();

  /**
  * @brief Constructs a new LayerFolder with the given name.
  * @since SketchUp 2021.0, API 9.0
  * @param name - the name of the folder (must be non-empty).
  */
  LayerFolder(const std::string& name);

  /**
  * @brief Constructs a LayerFolder from an existing SULayerFolderRef.
  * @since SketchUp 2021.0, API 9.0
  * @param layer_folder - the SULayerFolderRef to wrap.
  * @param attached - flag indicating whether the folder is owned by a model.
  */
  LayerFolder(SULayerFolderRef layer_folder, bool attached = true);

  /**
  * @brief Copy constructor.
  */
  LayerFolder(const LayerFolder& other);

  /** @brief Destructor. */
  ~LayerFolder();

  /**
  * @brief Copy assignment operator.
  */
  LayerFolder& operator=(const LayerFolder& other);

  /**
  * @brief Returns the SULayerFolderRef object.
  */
  SULayerFolderRef ref() const;

  /**
  * @brief Returns true if this is an invalid/null layer folder.
  */
  bool operator!() const;

  /**
   * @brief Returns a detached copy of the LayerFolder, excluding the contained layers and sub-folders.
   *
   * @return LayerFolder
   */
  LayerFolder copy() const;

  /**
  * @brief Retrieves the name of the layer folder.
  * @since SketchUp 2021.0, API 9.0
  * @throws std::logic_error if the layer folder is null.
  */
  String name() const;

  /**
  * @brief Sets the name of the layer folder.
  * @since SketchUp 2021.0, API 9.0
  * @param name - the new name (must be non-empty).
  * @throws std::logic_error if the layer folder is null.
  * @throws std::invalid_argument if the name is empty.
  */
  void name(const std::string& name);

  /**
  * @brief Retrieves the visibility of the layer folder.
  * @since SketchUp 2021.0, API 9.0
  * @throws std::logic_error if the layer folder is null.
  */
  bool visible() const;

  /**
  * @brief Sets the visibility of the layer folder.
  * @since SketchUp 2021.0, API 9.0
  * @throws std::logic_error if the layer folder is null.
  */
  void visible(bool visible);

  /**
  * @brief Retrieves whether the layer folder is visible on new scenes.
  * @since SketchUp 2021.0, API 9.0
  * @throws std::logic_error if the layer folder is null.
  */
  bool visible_on_new_scenes() const;

  /**
  * @brief Sets whether the layer folder is visible on new scenes.
  * @since SketchUp 2021.0, API 9.0
  * @throws std::logic_error if the layer folder is null.
  */
  void visible_on_new_scenes(bool visible);

  /**
  * @brief Adds a layer to this folder.
  * @since SketchUp 2021.0, API 9.0
  * @param layer - the layer to add.
  * @throws std::logic_error if the layer folder is null.
  */
  void add_layer(Layer& layer);

  /**
  * @brief Removes a layer from this folder.
  * @since SketchUp 2021.0, API 9.0
  * @param layer - the layer to remove.
  * @throws std::logic_error if the layer folder is null.
  */
  void remove_layer(Layer& layer);

  /**
  * @brief Retrieves the layers in this folder.
  * @since SketchUp 2021.0, API 9.0
  * @throws std::logic_error if the layer folder is null.
  */
  std::vector<Layer> layers() const;

  /**
  * @brief Adds a sub-folder to this folder.
  * @since SketchUp 2021.0, API 9.0
  * @param folder - the sub-folder to add.
  * @throws std::logic_error if the layer folder is null.
  */
  void add_layer_folder(LayerFolder& folder);

  /**
  * @brief Retrieves the sub-folders of this folder.
  * @since SketchUp 2021.0, API 9.0
  * @throws std::logic_error if the layer folder is null.
  */
  std::vector<LayerFolder> sub_folders() const;

  /**
  * @brief Retrieves the parent folder, if any.
  * @since SketchUp 2021.0, API 9.0
  * @return LayerFolder object (may be null if no parent).
  * @throws std::logic_error if the layer folder is null.
  */
  LayerFolder parent_folder() const;
};

} /* namespace CW */

#endif /* LayerFolder_hpp */
