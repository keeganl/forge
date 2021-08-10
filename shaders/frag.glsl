#version 330 core
out vec4 FragColor;
layout (location = 1) out int id;


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform sampler2D texture1;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec4 objectColor;
uniform int objectId;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

struct Light {
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
   // ambient
   float ambientStrength = 0.5;
   vec3 ambient = ambientStrength * lightColor;

   // diffuse
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   // specular
   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;

   // textures
   vec4 texColor = texture(texture1, TexCoord);

//   vec3 result = (ambient + diffuse + specular) * objectColor;
   FragColor = texColor * objectColor;
   id = objectId;
}