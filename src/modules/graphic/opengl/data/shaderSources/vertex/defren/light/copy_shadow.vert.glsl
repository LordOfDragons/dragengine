#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

layout(location=0) in vec2 inPosition;
layout(location=1) in int inLayer;
layout(location=2) in vec3 inTexCoord;

// special situation: can not use interface/2d_vertex.glsl
// because tex coord had to be vec3 not vec2
#ifdef WITH_GEOMETRY_SHADER
	// opengl only
	VARYING_BIND(0) out vec3 vGSTexCoord;
	VARYING_BIND(1) flat out int vGSLayer;
	
#else
	// spir-v only
	VARYING_BIND(0) out vec3 vTexCoord;
#endif


void main( void ){
	gl_Position = vec4(inPosition, 0.0, 1.0);
	
	#ifdef WITH_GEOMETRY_SHADER
		vGSTexCoord = inTexCoord;
		vGSLayer = inLayer;
		
	#else
		vTexCoord = inTexCoord;
		#if defined SUPPORTS_VSLAYER && ! defined OPENGLES
		if(VSRenderLayer){
			gl_Layer = inLayer;
		}
		#endif
	#endif
}
