#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

VARYING_BIND(0) in vec2 vTexCoord;
VARYING_BIND(1) flat in vec3 vColor;
VARYING_BIND(2) flat in vec3 vColorRim;
VARYING_BIND(3) flat in float vRimThickness;

layout(location=0) out vec3 outColor;

void main( void ){
	float radius = length( vTexCoord );
	if( radius > 1.0 ){
		discard; // outside sphere
	}
	
	outColor = radius > vRimThickness ? vColorRim : vColor;
}
