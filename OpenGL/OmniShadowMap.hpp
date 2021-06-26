//
//  OmniShadowMap.hpp
//  OpenGL
//
//  Created by Apple on 05/06/21.
//

#ifndef OmniShadowMap_hpp
#define OmniShadowMap_hpp

#include <stdio.h>
#include "ShadowMap.hpp"

class OmniShadowMap: public ShadowMap{
public:
    OmniShadowMap();
    bool init(GLuint width, GLuint height);
    void write();
    void read(GLenum textureUnit);
    ~OmniShadowMap();
};
#endif /* OmniShadowMap_hpp */
