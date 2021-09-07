#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform float time;
uniform sampler2D spriteTexture;

void main() {
    vec4 textureColor = texture(spriteTexture, fragmentUV);
    color = vec4(fragmentColor.x * (cos(fragmentPosition.x + time) + 1.0) * 0.5,
                 fragmentColor.y * (cos(fragmentPosition.x + time) + 1.0) * 0.5,
    	         fragmentColor.z * (cos(fragmentPosition.x + time) + 1.0) * 0.5,
                 fragmentColor.a) * textureColor;
}
