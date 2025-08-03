#include "shared/preamble.glsl"

layout(location=0) in vec3 inPosition;

#ifdef VS_RENDER_STEREO
	#define inLayer gl_DrawID
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif

void main( void ){
	gl_Position = vec4( inPosition.xy * vec2( 2 ) - vec2( 1 ), 1, 1 );
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
