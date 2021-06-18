#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
uniform float scale;
uniform vec3 scaleAxes;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 TexCoord;
void main()
{

  gl_Position = projection * model * view * vec4((scaleAxes.x * aPos.x) * scale, (scaleAxes.y * aPos.y) * scale, (scaleAxes.z * aPos.z) * scale, 1.0);
  TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}