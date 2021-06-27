//
//  FontLoader.cpp
//  OpenGL
//
//  Created by Apple on 26/06/21.
//

#include "FontRenderer.hpp"

FontRenderer::FontRenderer(){
    // Setup projection matrix
    projection = glm::ortho(
                            0.0f,       // Left
                            800.0f,     // Right
                            0.0f,       // Bottom
                            600.0f      // Top
                            );
    
    // Init freetype
    if(FT_Init_FreeType(&ft)){
        LOGGER("Failed to initialise free type.");
        return;
    }
    
    if(FT_New_Face(ft, "/System/Library/Fonts/SFCompactText.ttf", 0, &face)){
        LOGGER("Failed to create font face.");
        return;
    }
    // Define Pixel Font Size
    FT_Set_Pixel_Sizes(face, 0, 48);
    LOGGER("FreeType Initialised.");
    // Load all the necessary characters
    loadCharacters();
    
}

void FontRenderer::init(){
    // Shaders Setup
    fontShaders = new Shader();
    fontShaders->createFromFiles("Shaders/font.vert", "Shaders/font.frag");
    uniformProjection = fontShaders->getProjectionLocation();
    uniformTextColor = fontShaders->getTextColorLocation();
    
    // Setup VAO and VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // The 2D quad requires 6 vertices of 4 floats each, so we reserve 6 * 4 floats of memory.
    // Because we'll be updating the content of the VBO's memory quite often we'll allocate the memory with GL_DYNAMIC_DRAW.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    LOGGER("Font Renderer Initialised.");
}

void FontRenderer::loadCharacters(){
    // If we use a single byte to represent the colors of a texture we do need to take care of a restriction of OpenGL
    // OpenGL requires that textures all have a 4-byte alignment
    // By setting its unpack alignment to 1 we ensure there are no alignment issues (which could cause segmentation faults)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Disable Byte Allignment Restriction
    
    // Load first 128 ASCII characters
    for(unsigned char c=0; c<128; c++){
        // Load character Glyph
        // Set one of the Glyphs as the active glyph by calling FT_Load_Char
        // By setting FT_LOAD_RENDER as one of the loading flags, we tell FreeType to create
        // an 8-bit grayscale bitmap image for us that we can access via face->glyph->bitmap
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
            LOGGER("Failed to load the Glyph.");
            continue;
        }
        
        // Generate Texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        
        // Set Texture Options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Store Character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<const unsigned int>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Character>(c, character));
    }
    LOGGER("Characters are loaded successfully.\n");
}

FontRenderer::~FontRenderer(){
    if(VBO != 0){
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    
    if(VAO != 0){
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void FontRenderer::drawText(std::string text, float x, float y, float scale, glm::vec3 color){
    // Activate Shader and Pass Uniform Values and Textures
    fontShaders->useShader();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(uniformTextColor, color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    
    // Activate VAO
    glBindVertexArray(VAO);
    
    // Iterate thought all the characters
    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++){
        Character ch = characters[*c];
        
        // Calculate the origin position of the quad
        float xpos = x + ch.bearing.x * scale;
        // Some characters (like 'p' or 'g') are rendered slightly below the baseline, so the quad should also be positioned slightly below RenderText's y value.
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        
        // Calculate Quad Size
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        
        // Upload VBO for each character
        float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };
        
        // Render Glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // Render Quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void FontRenderer::render(){
    // Initialize
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Text Renderer
    this->drawText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
    this->drawText("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
    
    glDisable(GL_BLEND);
}
