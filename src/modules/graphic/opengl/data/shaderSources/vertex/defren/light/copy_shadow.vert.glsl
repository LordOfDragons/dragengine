#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

layout(location=0) in vec2 inPosition;
layout(location=1) in int inLayer;
layout(location=2) in vec3 inTexCoord;

#ifdef GS_LAYER
	out vec3 vGSTexCoord;
	flat out int vGSLayer;
#else
	out vec3 vTexCoord;
#endif

void main( void ){
	gl_Position = vec4(inPosition, 0.0, 1.0);
	
	#ifdef GS_LAYER
		vGSTexCoord = inTexCoord;
		vGSLayer = inLayer;
	#else
		vTexCoord = inTexCoord;
		#ifdef VS_LAYER
			gl_Layer = inLayer;
		#endif
	#endif
}
