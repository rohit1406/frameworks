//
//  FontLoader.hpp
//  OpenGL
//
//  Created by Apple on 26/06/21.
//

#ifndef FontLoader_hpp
#define FontLoader_hpp

#include <stdio.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.hpp"
#include "logger.h"

struct Character{
    unsigned int textureId;     // ID handle of the Glyph texture
    glm::ivec2 size;            // Size of the glyph
    glm::ivec2 bearing;         // Offset from baseline to left/top of the glyph
    unsigned int advance;        // Offset to advance to next glyph
};

class FontRenderer{
public:
    FontRenderer();
    void init();
    void render();
    void drawText(std::string text, float x, float y, float scale, glm::vec3 color);
    ~FontRenderer();
    
    std::map<char, Character> characters;
    
private:
    unsigned int VAO, VBO;
    Shader *fontShaders;
    
    GLuint uniformProjection;
    GLuint uniformTextColor;
    
    // Fonts
    FT_Library ft;
    FT_Face face;
    
    glm::mat4 projection;
    
    // Functions
    void loadCharacters();
};

#endif /* FontLoader_hpp */
