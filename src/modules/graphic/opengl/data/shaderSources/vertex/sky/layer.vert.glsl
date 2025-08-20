#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

layout(location=0) in vec2 inPosition;
layout(location=1) in int inLayer;

#include "shared/interface/2d/vertex.glsl"

// Z value on far clip plane (cleared depth) in NDC
const float vZFar = InverseDepth ? -1.0 : 1.0;

void main( void ){
	vertexShaderDefaultOutputs();
	
	gl_Position = vec4( inPosition, vZFar, 1 );
	vTexCoord = inPosition;
	
	if(VSRenderLayer){
		vLayer = inLayer;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = vLayer;
		#endif
	}
}
