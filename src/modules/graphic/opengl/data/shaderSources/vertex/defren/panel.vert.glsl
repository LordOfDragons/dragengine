#include "shared/preamble.glsl"

UNIFORM_BIND(0) uniform mat4x3 pMatrixModel;
UNIFORM_BIND(1) uniform mat4 pMatrixVP;
UNIFORM_BIND(2) uniform mat4 pMatrixVP2;

layout(location=0) in vec3 inPosition;
layout(location=4) in vec2 inTexCoord;

// VSRenderStereo
layout(location=5) in int inLayer;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	vec3 position = pMatrixModel * vec4( inPosition, 1 );
	
	if(GSRenderStereo){
		gl_Position = vec4( position, 1 );
	}else{
		gl_Position = ( inLayer == 0 ? pMatrixVP : pMatrixVP2 ) * vec4( position, 1 );
	}
	
	vTexCoord = inTexCoord;
	
	if(VSRenderStereo){
		vLayer = inLayer;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = vLayer;
		#endif
	}
}
