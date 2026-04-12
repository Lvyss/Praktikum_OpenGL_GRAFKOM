#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 WorldPos;

uniform float time;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float continent(vec3 pos) {
    vec3 p = normalize(pos);
    float land = 0.0;
    
    land += sin(p.x * 5.0 + 1.0) * cos(p.z * 4.0) * sin(p.y * 3.0);
    land += sin(p.x * 15.0 + 2.3) * cos(p.z * 12.0) * 0.3;
    land += sin(p.x * 8.0 + p.z * 8.0) * 0.5;
    
    land = land * 0.5 + 0.5;
    return land;
}

float clouds(vec3 pos, float time) {
    vec3 p = normalize(pos);
    float cloud1 = sin(p.x * 20.0 + time) * cos(p.z * 18.0 - time) * sin(p.y * 15.0);
    float cloud2 = sin(p.x * 30.0 + time * 1.5) * cos(p.z * 25.0) * 0.3;
    
    float cloud = (cloud1 + cloud2) * 0.5 + 0.5;
    cloud *= 1.0 - abs(p.y);
    
    return cloud;
}

void main() {
    vec3 pos = normalize(WorldPos);
    float lat = abs(pos.y);
    float pole = 1.0 - smoothstep(0.7, 0.9, lat);
    
    float landAmount = continent(pos);
    
    vec3 color;
    
    if (pole > 0.3) {
        color = vec3(1.0, 1.0, 1.0);
    }
    else {
        vec3 ocean = vec3(0.2, 0.4, 0.8);
        vec3 shallow = vec3(0.3, 0.6, 0.9);
        vec3 sand = vec3(0.9, 0.8, 0.5);
        vec3 grass = vec3(0.2, 0.7, 0.2);
        vec3 mountain = vec3(0.6, 0.5, 0.4);
        
        if (landAmount > 0.55) {
            float t = (landAmount - 0.55) / 0.45;
            color = mix(grass, mountain, t);
        }
        else if (landAmount > 0.5) {
            float t = (landAmount - 0.5) / 0.05;
            color = mix(sand, grass, t);
        }
        else if (landAmount > 0.45) {
            float t = (landAmount - 0.45) / 0.05;
            color = mix(shallow, sand, t);
        }
        else {
            float depth = 1.0 - landAmount * 2.0;
            color = mix(ocean, shallow, depth);
        }
    }
    
    float cloudAmount = clouds(pos, time);
    if (cloudAmount > 0.6 && pole < 0.3) {
        float cloudIntensity = (cloudAmount - 0.6) / 0.4;
        color = mix(color, vec3(1.0, 1.0, 1.0), cloudIntensity * 0.5);
    }
    
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(Normal, lightDir), 0.2);
    
    FragColor = vec4(color * diff, 1.0);
}