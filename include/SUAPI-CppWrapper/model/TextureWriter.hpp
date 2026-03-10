//
//  TextureWriter.hpp
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

#ifndef TextureWriter_hpp
#define TextureWriter_hpp

#include <SketchUpAPI/model/texture_writer.h>

namespace CW {

/**
 * @brief Wrapper for SUTextureWriterRef, used to write texture
 *        images associated with a model to disk.
 *
 * TextureWriter provides the capability to extract texture images
 * from faces and materials and save them to files.
 *
 * @see https://extensions.sketchup.com/developer_center/sketchup_c_api/
 */
class TextureWriter {
  private:
  SUTextureWriterRef m_texture_writer;
  
  public:
  /** @brief Default constructor – creates an invalid TextureWriter. */
  TextureWriter();

  /**
   * @brief Constructs a TextureWriter wrapping a raw API ref.
   * @param texture_writer the SUTextureWriterRef to wrap.
   */
  TextureWriter(SUTextureWriterRef texture_writer);
  
  /** @brief Conversion to SUTextureWriterRef. */
  operator SUTextureWriterRef() const;

  /** @brief Conversion to SUTextureWriterRef pointer. */
  operator SUTextureWriterRef*() const;
};

}
#endif /* TextureWriter_hpp */
