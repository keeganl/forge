#version 330 core
out vec4 FragColor;
layout (location = 1) out int id;


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
uniform sampler2D texture1;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int objectId;

uniform struct Material {
   vec3 ambient;
   sampler2D diffuse;
   vec3 specular;
   float shininess;
} material;

#define MAX_LIGHTS 70
uniform int numLights;

uniform struct Light {
   vec4 position;
   vec3 direction;
   vec4 color;
   float coneAngle;
   float constant;
   float linear;
   float quadratic;
   float innerCutoff;
   float outerCutoff;
   float ambient;
   vec3 specular;
   float attenuation;
} allLights[MAX_LIGHTS];

uniform Light light;

vec3 CalcLight(Light light, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera)
{
   vec3 surfaceToLight;
   float attenuation;
   if(light.position.w == 0.0) {
      //directional light
      surfaceToLight = normalize(light.position.xyz);
      attenuation = 1.0; //no attenuation for directional lights
   } else {
      //point light
      surfaceToLight = normalize(light.position.xyz - surfacePos);
      float distanceToLight = length(light.position.xyz - surfacePos);
      float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * (distanceToLight * distanceToLight));

      //cone restrictions (affects attenuation)
      float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.direction))));
      if(lightToSurfaceAngle > light.coneAngle){
         attenuation = 0.0;
      }
   }

   //ambient
   vec3 ambient = light.ambient * material.ambient * light.color.xyz;

   //diffuse
   float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));

   //specular
   // bling-phong
   vec3 viewDir = normalize(viewPos - FragPos);
   float specularCoefficient = 0.0;
   if(diffuseCoefficient > 0.0)
      specularCoefficient = pow(max(0.0, dot(surfaceToCamera, normalize(surfaceToLight + viewDir))), material.shininess);
//   // phong
//   float specularCoefficient = 0.0;
//   if(diffuseCoefficient > 0.0)
//      specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), material.shininess);

   //linear color (color before gamma correction)
   vec3 diffuse = diffuseCoefficient * vec3(texture(material.diffuse, TexCoord));
   vec3 specular = specularCoefficient * material.specular;
   return light.color.xyz * (ambient + attenuation*(diffuse + specular));

}


void main()
{
   vec3 norm = normalize(Normal);
   vec3 surfaceToCamera = normalize(viewPos - FragPos);

   //combine color from all the lights
   vec3 linearColor = vec3(0);
   for(int i = 0; i < numLights; ++i){
      linearColor += CalcLight(allLights[i], norm, FragPos, surfaceToCamera);
   }

   //final color (after gamma correction)
   vec3 gamma = vec3(1.0/2.2);
   FragColor =  vec4(pow(linearColor, gamma), 1.0);
   id = objectId;
}
