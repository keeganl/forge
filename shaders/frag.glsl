#version 330 core
out vec4 FragColor;
uniform vec4 color;
in vec2 TexCoord;
// texture samplers
uniform sampler2D texture1;
uniform float mixVal;
void main()
{
   FragColor = mix(color, texture(texture1, TexCoord), mixVal);
}