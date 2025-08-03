#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(location=0) in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	layout(location=1) in int inLayer;
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
