precision highp float;
precision highp int;

uniform vec4 pParam1; // offset1.s, offset1.t, offset2.s, offset2.t
uniform vec4 pParam2; // offset3.s, offset3.t, clamp.s, clamp.t

// uniform mediump sampler2DArray texColor;
uniform mediump sampler2DArray texLuminance;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out float outLogLum;

const vec4 weights = vec4( 0.25 );
const vec4 epsilon = vec4( 0.0001 );
const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	vec4 tc1 = min( vTexCoord.stst + pParam1, pParam2.zwzw );
	vec2 tc2 = min( vTexCoord + pParam2.xy, pParam2.zw );
	vec4 values;
	
// 	values.x = dot( textureLod( texColor, vTexCoord, 0.0 ).rgb, lumiFactors );
// 	values.y = dot( textureLod( texColor, tc1.xy, 0.0 ).rgb, lumiFactors );
// 	values.z = dot( textureLod( texColor, tc1.zw, 0.0 ).rgb, lumiFactors );
// 	values.w = dot( textureLod( texColor, tc2, 0.0 ).rgb, lumiFactors );
	
	values.x = textureLod( texLuminance, vec3( vTexCoord, vLayer ), 0 ).r;
	values.y = textureLod( texLuminance, vec3( tc1.xy, vLayer ), 0 ).r;
	values.z = textureLod( texLuminance, vec3( tc1.zw, vLayer ), 0 ).r;
	values.w = textureLod( texLuminance, vec3( tc2, vLayer ), 0 ).r;
	
	outLogLum = dot( log( values + epsilon ), weights );
}
