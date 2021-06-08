#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
uniform float scale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 TexCoord;
void main()
{

  gl_Position = projection * model * view * vec4(scale * aPos.x, scale * aPos.y, scale * aPos.z, 1.0);
  TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}