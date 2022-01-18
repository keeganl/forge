#version 330 core
uniform mat4 view;
uniform mat4 projection;


// Grid position are in xy clipped space
vec3 gridPlane[6] = vec3[](
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);
// normal vertice projection
void main() {
    gl_Position = projection * view * vec4(gridPlane[gl_VertexID].xyz, 1.0);
}
