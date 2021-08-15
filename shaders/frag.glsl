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

struct DirLight {
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct PointLight {
   vec3 position;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct SpotLight {
   vec3 position;
   vec3 direction;
   float cutOff;
   float outerCutOff;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform Material material;
uniform Light light;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

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

//// calculates the color when using a directional light.
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//   vec3 lightDir = normalize(-light.direction);
//   // diffuse shading
//   float diff = max(dot(normal, lightDir), 0.0);
//   // specular shading
//   vec3 reflectDir = reflect(-lightDir, normal);
//   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//   // combine results
//   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
//   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
//   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
//   return (ambient + diffuse + specular);
//}
//
//// calculates the color when using a point light.
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//   vec3 lightDir = normalize(light.position - fragPos);
//   // diffuse shading
//   float diff = max(dot(normal, lightDir), 0.0);
//   // specular shading
//   vec3 reflectDir = reflect(-lightDir, normal);
//   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//   // attenuation
//   float distance = length(light.position - fragPos);
//   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//   // combine results
//   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
//   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
//   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
//   ambient *= attenuation;
//   diffuse *= attenuation;
//   specular *= attenuation;
//   return (ambient + diffuse + specular);
//}
//
//// calculates the color when using a spot light.
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//   vec3 lightDir = normalize(light.position - fragPos);
//   // diffuse shading
//   float diff = max(dot(normal, lightDir), 0.0);
//   // specular shading
//   vec3 reflectDir = reflect(-lightDir, normal);
//   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//   // attenuation
//   float distance = length(light.position - fragPos);
//   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//   // spotlight intensity
//   float theta = dot(lightDir, normalize(-light.direction));
//   float epsilon = light.cutOff - light.outerCutOff;
//   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//   // combine results
//   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
//   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
//   vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
//   ambient *= attenuation * intensity;
//   diffuse *= attenuation * intensity;
//   specular *= attenuation * intensity;
//   return (ambient + diffuse + specular);
//}