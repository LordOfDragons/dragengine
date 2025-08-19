#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

UNIFORM_BIND(0) uniform mat4x3 pMatrixBody;

layout(location=0) in vec2 inPosition;
layout(location=1) in int inLayer;

#include "shared/interface/2d/vertex.glsl"

const vec2 tcScale = vec2(0.5, -0.5);
const vec2 tcOffset = vec2(0.5, 0.5);

void main(void){
	vertexShaderDefaultOutputs();
	
	VARCONST int layer = VSRenderLayer ? inLayer : 0;
	
	// apply layer rotation and position
	gl_Position = vec4(pMatrixBody * vec4(inPosition, 1.0, 1.0), 1.0);
	
	if(LayeredRendering == LayeredRenderingNone || VSRenderLayer){
		gl_Position = pMatrixSkyBody[layer] * gl_Position;
		
		// make sure Z is exactly -1 after transformation for depth test to work correctly
		gl_Position.z = -gl_Position.w;
	}
	
	vTexCoord = inPosition * tcScale + tcOffset;
	
	if(VSRenderLayer){
		vLayer = layer;
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = layer;
		#endif
	}
}
