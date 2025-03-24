#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;

void main()
{
    vec4 worldPosition = model * vec4(aPos, 1.0);
    FragPos = worldPosition.xyz;

    gl_Position = projection * view * worldPosition;
}
