#include "shared/preamble.glsl"

UNIFORM_BIND(0) uniform mat4x3 pMatrixModel;
UNIFORM_BIND(1) uniform mat4 pMatrixVP;
UNIFORM_BIND(2) uniform mat4 pMatrixVP2;

layout(location=0) in vec3 inPosition;
layout(location=4) in vec2 inTexCoord;

// VSRenderLayer
layout(location=5) in int inLayer;

#include "shared/interface/2d/vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	vec3 position = pMatrixModel * vec4( inPosition, 1 );
	int layer = VSRenderLayer ? inLayer : 0;
	
	if(LayeredRendering != LayeredRenderingNone && !VSRenderLayer){
		gl_Position = vec4(position, 1.0);
		
	}else{
		gl_Position = (layer == 0 ? pMatrixVP : pMatrixVP2) * vec4(position, 1.0);
	}
	
	vTexCoord = inTexCoord;
	
	if(VSRenderLayer){
		vLayer = layer;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = layer;
		#endif
	}
}
