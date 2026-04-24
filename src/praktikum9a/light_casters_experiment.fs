#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
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
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform int lightType;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    vec3 lightDir;
    float attenuation = 1.0;
    float intensity = 1.0;
    
    if (lightType == 0) { // Directional Light
        lightDir = normalize(light.direction);
    } 
    else if (lightType == 1) { // Point Light
        lightDir = normalize(light.position - FragPos);
        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    } 
    else { // Spot Light
        lightDir = normalize(light.position - FragPos);
        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = (light.cutOff - light.outerCutOff);
        intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    }
    
    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularColor = light.specular * spec * texture(material.specular, TexCoords).rgb;
    
    // Apply spotlight intensity
    if (lightType == 2) {
        diffuseColor *= intensity;
        specularColor *= intensity;
    }
    
    // Apply attenuation
    ambient *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;
    
    vec3 result = ambient + diffuseColor + specularColor;
    FragColor = vec4(result, 1.0);
}