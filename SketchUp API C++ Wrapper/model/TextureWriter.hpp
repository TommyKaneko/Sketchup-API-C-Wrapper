//
//  TextureWriter.hpp
//  SUEX_SkinUp
//
//  Created by Tom Kaneko on 05/10/2016.
//  Copyright © 2016 Tom Kaneko. All rights reserved.
//

#ifndef TextureWriter_hpp
#define TextureWriter_hpp

#include <stdio.h>

#include <SketchUpAPI/model/texture_writer.h>

namespace CW {

/*
* TextureWriter wrapper
*/
class TextureWriter {
	private:
  SUTextureWriterRef m_texture_writer;
  
  public:
  TextureWriter();
  TextureWriter(SUTextureWriterRef texture_writer);
  
  operator SUTextureWriterRef() const;
  operator SUTextureWriterRef*() const;
};

}
#endif /* TextureWriter_hpp */
