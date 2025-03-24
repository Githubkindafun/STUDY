#version 330 core
in vec3 FragPos; 
//https://stackoverflow.com/questions/4694608/glsl-checkerboard-pattern

out vec4 FragColor;

void main()
{
    float scale = 15.0;

    float checker = mod(floor(FragPos.x * scale) + floor(FragPos.y * scale) + floor(FragPos.z * scale), 2.0); // 0 : kolor 1, 1 : kolor 2 

    
    vec3 white = vec3(1.0f, 1.0f, 1.0f); 
    vec3 black = vec3(0.0f, 0.0f, 0.0f); 

    vec3 finalColor;
    if (checker != 1.0) {
        finalColor = white;
    }
    else {
        finalColor = black;
    }

    FragColor = vec4(finalColor, 1.0);
}
