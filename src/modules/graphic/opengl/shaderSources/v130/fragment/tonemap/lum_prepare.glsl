precision highp float;
precision highp int;

uniform highp sampler2DArray texColor;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out float outLuminance;

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	outLuminance = dot( texture( texColor, vec3( vTexCoord, vLayer ) ).rgb, lumiFactors );
}
