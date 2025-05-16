precision HIGHP float;
precision HIGHP int;

uniform vec4 pColor;

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = pColor;
}
