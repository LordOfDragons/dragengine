precision highp float;
precision highp int;

uniform vec4 pOffsets; // -offsetU, offsetU, -offsetV, offsetV

uniform mediump sampler2DArray texValues;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out float outValue;

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
