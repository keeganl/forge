#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2DMS screenTexture;
uniform bool hdr;
uniform float exposure;

void main()
{
   float gamma = 2.2f;
   vec3 hdrColor = texelFetch(screenTexture, ivec2(TexCoords.xy), 3).rgb;
   if(hdr)
   {
      // exposure
      vec3 result = vec3(hdrColor) - exp(-hdrColor * exposure);
      // also gamma correct while we're at it
      result = pow(result, vec3(1.0 / gamma));
      FragColor = vec4(result, 1.0);
   }
   else
   {
      vec3 result = pow(hdrColor, vec3(1.0 / gamma));
      FragColor = vec4(result, 1.0);
   }
}