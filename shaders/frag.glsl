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
   sampler2D diffuse;
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
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

   // diffuse
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

   // specular
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
   id = objectId;
}