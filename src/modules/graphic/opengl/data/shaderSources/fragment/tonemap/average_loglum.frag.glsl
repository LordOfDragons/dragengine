#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform vec4 pOffsets; // -offsetU, offsetU, -offsetV, offsetV

uniform mediump sampler2DArray texValues;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out float outValue;

const vec4 weights = vec4( 0.25 );

void main( void ){
	vec4 tc = vTexCoord.sstt + pOffsets;
	vec4 values;
	
	values.x = textureLod( texValues, vec3( tc.xz, vLayer ), 0.0 ).r; // -u, -v
	values.y = textureLod( texValues, vec3( tc.yz, vLayer ), 0.0 ).r; //  u, -v
	values.z = textureLod( texValues, vec3( tc.xw, vLayer ), 0.0 ).r; // -u,  v
	values.w = textureLod( texValues, vec3( tc.yw, vLayer ), 0.0 ).r; //  u,  v
	
	outValue = dot( values, weights );
}
