#version 330 core

struct Material {
    float shininess;
}; 

struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;   

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

vec3 ModelLightFong();

void main()
{    
    vec3 result = ModelLightFong();
     FragColor =  vec4(result, 1.0f);
}

vec3 ModelLightFong() 
{
    // ambient light
    vec3 ambient = light.ambient * texture(diffuseMap, TexCoords).rgb;
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(diffuseMap, TexCoords).rgb;  
    
    // specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(specularMap, TexCoords).rgb;  
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    return result;
}