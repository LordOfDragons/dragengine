#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

VARYING_BIND(0) in vec2 vTexCoord;
flat in float vRimThickness;
flat in vec3 vColor;
flat in vec3 vColorRim;

layout(location=0) out vec3 outColor;

void main( void ){
	float radius = length( vTexCoord );
	if( radius > 1.0 ){
		discard; // outside sphere
	}
	
	outColor = radius > vRimThickness ? vColorRim : vColor;
}
