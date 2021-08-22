#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;



void main()
{
    FragColor = vec4((ambient + diffuse + specular), 1.0);
}