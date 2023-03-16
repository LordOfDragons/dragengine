precision lowp float;
precision lowp int;

uniform lowp vec4 pColor;

uniform lowp sampler2D texColor;

in vec2 vTexCoord;

out lowp vec4 outColor;

void main( void ){
	outColor = texture( texColor, vTexCoord ) * pColor;
}
