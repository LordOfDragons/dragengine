precision highp float;
precision highp int;

uniform mediump sampler2D texColor;

in vec2 vTexCoord;

out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vTexCoord, 0.0 ).rgb;
}
