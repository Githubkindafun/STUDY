#version 330 core
layout (location = 0) in vec3 aPos;          
layout (location = 1) in vec3 aNormal;       
layout (location = 3) in vec3 instanceColor; 
layout (location = 4) in mat4 model;         
layout (location = 8) in int isSpecial; // flaga determinjaca czy babelek jest specjalny

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
flat out int IsSpecial; 

uniform mat4 view;
uniform mat4 projection;


void main() {
    vec4 worldPosition = model * vec4(aPos, 1.0);
    FragPos = vec3(worldPosition);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Color = instanceColor;
    IsSpecial = isSpecial;
    gl_Position = projection * view * worldPosition;
}

