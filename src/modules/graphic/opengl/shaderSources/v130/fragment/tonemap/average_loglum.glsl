precision highp float;
precision highp int;

uniform vec4 pOffsets; // -offsetU, offsetU, -offsetV, offsetV

uniform mediump sampler2D texValues;

in vec2 vTexCoord;

out float outValue;

const vec4 weights = vec4( 0.25 );

void main( void ){
	vec4 tc = vTexCoord.sstt + pOffsets;
	vec4 values;
	
	values.x = textureLod( texValues, tc.xz, 0.0 ).r; // -u, -v
	values.y = textureLod( texValues, tc.yz, 0.0 ).r; //  u, -v
	values.z = textureLod( texValues, tc.xw, 0.0 ).r; // -u,  v
	values.w = textureLod( texValues, tc.yw, 0.0 ).r; //  u,  v
	
	outValue = dot( values, weights );
}
