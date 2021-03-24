#version 410
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 velocity;

out vec2 fragUV;
out vec3 fragNormal;

uniform mat4 mvp;
uniform mat3 normalMatrix;

void main()
{
    gl_Position = mvp * vec4(position + velocity, 1.0);

    fragUV = vec2(uv.x, 1 - uv.y);

    fragNormal = normalMatrix * normal;
}