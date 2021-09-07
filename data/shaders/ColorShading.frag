#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

uniform float time;

void main() {
    color = vec4(fragmentColor.x * (cos(fragmentPosition.x + time) + 1.0) * 0.5,
                 fragmentColor.y * (cos(fragmentPosition.x + time) + 1.0) * 0.5,
    	         fragmentColor.z * (cos(fragmentPosition.x + time) + 1.0) * 0.5,
                 fragmentColor.a);
}
