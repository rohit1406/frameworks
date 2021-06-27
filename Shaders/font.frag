#version 330                // OpenGL version 3.3

in vec2 outTexCoords;
out vec4 color;

uniform sampler2D text;     // Mono-Colored bitmap image of a glyph
uniform vec3 textColor;     // Text Color

void main(){
    // By varying the output color's alpha value, the resulting pixel will be transparent for all the glyph's background colors and non-transparent for the actual character pixels.
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, outTexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
