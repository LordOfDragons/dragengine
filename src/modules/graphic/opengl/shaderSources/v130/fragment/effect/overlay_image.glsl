precision lowp float;
precision lowp int;

uniform vec4 pGamma;
uniform vec4 pColor;

uniform lowp sampler2D texColor;

in vec2 vTexCoord;

out lowp vec4 outColor;

void main( void ){
	outColor = pow( texture( texColor, vTexCoord ), pGamma ) * pColor;
}
