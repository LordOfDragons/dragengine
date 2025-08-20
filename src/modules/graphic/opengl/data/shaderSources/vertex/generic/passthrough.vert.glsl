#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(location=0) in vec2 inPosition;

// VSRenderLayer
layout(location=1) in int inLayer;

#include "shared/interface/2d/vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	gl_Position = vec4( inPosition, 0, 1 );
	
	if(VSRenderLayer){
		vLayer = inLayer;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = vLayer;
		#endif
	}
}
