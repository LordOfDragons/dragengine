precision highp float;
precision highp int;

uniform vec4 pParam1; // offset1.s, offset1.t, offset2.s, offset2.t
uniform vec4 pParam2; // offset3.s, offset3.t, clamp.s, clamp.t

uniform mediump sampler2D texColor;

in vec2 vTexCoord;

out float outLogLum;

const vec4 weights = vec4( 0.25 );
const vec4 epsilon = vec4( 0.0001 );
const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	vec4 tc1 = min( vTexCoord.stst + pParam1, pParam2.zwzw );
	vec2 tc2 = min( vTexCoord + pParam2.xy, pParam2.zw );
	vec4 values;
	
	values.x = dot( textureLod( texColor, vTexCoord, 0.0 ).rgb, lumiFactors );
	values.y = dot( textureLod( texColor, tc1.xy, 0.0 ).rgb, lumiFactors );
	values.z = dot( textureLod( texColor, tc1.zw, 0.0 ).rgb, lumiFactors );
	values.w = dot( textureLod( texColor, tc2, 0.0 ).rgb, lumiFactors );
	
	outLogLum = dot( log( values + epsilon ), weights );
}
