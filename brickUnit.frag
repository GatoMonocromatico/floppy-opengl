#version 330 core
out vec4 FragColor;
in vec2 texCoord;
in vec2 texCoordMask;
in vec3 normal;
in vec3 FragPos;
in float ambientLight;
in float centerY;

uniform sampler2D tex0; 
uniform sampler2D mask0; 

struct Light
{
   vec4 position;
   vec4 color;
   vec4 intensity;
};

layout (std140) uniform ScenarioLights
{
   Light scenarioLights[64];
};

uniform int numScenarioLights;
uniform int numBrickLights;

void main()
{
   vec3 diffuseLight = vec3(0.0);
   vec3 specularLight = vec3(0.0);
   float specularStrenght = 0.5;

   vec3 N1 = normalize(normal);

   //vec3 lightVector = vec3(0.0);
   //
   //vec3 lightPos = vec3(N1.xy*0.04 + FragPos.xy, -0.1405408347 * centerY);
   //lightVector = lightPos - FragPos;
   //vec3 lightDirect = normalize(lightVector);
   //
   //
   //diffuseLight += scenarioLights[0].color.rgb *
   //           0.6 *
   //           max(dot(N1, lightDirect), 0) / dot(lightVector * 16, lightVector);

   float monitorAtenuation = 100;
   float monitorIntesity = 0.08;
   vec3 monitorColor = vec3(0.0, 0.5, 0.5) * monitorIntesity;
   vec3 lightVector = vec3(0.0);

   // left border light
   vec3 lightPos = vec3(0.543548387096774-0.775-0.01, FragPos.y, -0.1405408347 * FragPos.y);
   lightVector = lightPos - FragPos;
   vec3 lightDirect = normalize(lightVector);


   diffuseLight += monitorColor *
              0.6 *
              max(dot(N1, lightDirect), 0) / dot(lightVector * monitorAtenuation, lightVector);

   // upper right border light
   lightPos = lightPos = vec3(0.848387096774193-0.775+0.01, FragPos.y + 0.4, -0.1405408347 * FragPos.y);
   lightVector = lightPos - FragPos;
   lightDirect = normalize(lightVector);


   diffuseLight += monitorColor *
              0.6 *
              max(dot(N1, lightDirect), 0) / dot(lightVector * monitorAtenuation, lightVector);

   // upper left border light
   lightPos = vec3(0.543548387096774-0.775-0.01, FragPos.y + 0.4, -0.1405408347 * FragPos.y);
   lightVector = lightPos - FragPos;
   lightDirect = normalize(lightVector);


   diffuseLight += monitorColor *
              0.6 *
              max(dot(N1, lightDirect), 0) / dot(lightVector * monitorAtenuation, lightVector);

   // right border light
   lightPos = lightPos = vec3(0.848387096774193-0.775+0.01, FragPos.y, -0.1405408347 * FragPos.y);
   lightVector = lightPos - FragPos;
   lightDirect = normalize(lightVector);


   diffuseLight += monitorColor *
              0.6 *
              max(dot(N1, lightDirect), 0) / dot(lightVector * monitorAtenuation, lightVector);

   // bottom border light
   lightPos = vec3(FragPos.x, -0.352078515625-0.01, -0.1405408347 * FragPos.y);
   lightVector = lightPos - FragPos;
   lightDirect = normalize(lightVector);


   diffuseLight += monitorColor *
              0.6 *
              max(dot(N1, lightDirect), 0) / dot(lightVector * monitorAtenuation, lightVector);

   // other lights
   for (int i = 1; i < numScenarioLights; i++)
   {
      lightVector = scenarioLights[i].position.xyz - FragPos;
      lightDirect = normalize(lightVector);


      diffuseLight += scenarioLights[i].color.rgb *
                 scenarioLights[i].intensity.r *
                 max(dot(N1, lightDirect), 0) / dot(lightVector * 0.2, lightVector);
       
      vec3 viewDirect = normalize(scenarioLights[1].position.xyz - FragPos);
      vec3 reflectionDirect = reflect(-lightDirect, N1);
      specularLight += pow(max(dot(viewDirect, reflectionDirect), 0.0), 32);
   };

   float ambient = 0.15;
   vec3 finalLight = vec3(pow(ambient + diffuseLight.r*0.5 + specularLight.r*specularStrenght, ambientLight), pow(ambient + diffuseLight.g*0.5 + specularLight.g*specularStrenght, ambientLight), pow(ambient + diffuseLight.b*0.5 + specularLight.b*specularStrenght, ambientLight));

   vec4 textureColor = texture(tex0, texCoord);
   float maskOpacity = texture(mask0, texCoordMask).a;
   FragColor = vec4(textureColor.rgb * finalLight, textureColor.a * maskOpacity);
}