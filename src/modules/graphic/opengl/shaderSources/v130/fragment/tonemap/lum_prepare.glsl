precision highp float;
precision highp int;

uniform highp sampler2DArray texColor;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	in flat int vLayer;
#else
	const int vLayer = 0;
#endif

out float outLuminance;

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	outLuminance = dot( texelFetch( texColor, ivec3( gl_FragCoord.xy, vLayer ), 0 ).rgb, lumiFactors );
}
