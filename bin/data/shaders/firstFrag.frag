#version 410

uniform sampler2D fireTex;
uniform vec4 color;
uniform float brightness;

in vec2 fragUV;

out vec4 outColor;

void main()
{
	outColor = texture(fireTex, fragUV) * color * brightness;
}