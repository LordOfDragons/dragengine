#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"

// (LayeredRendering == LayeredRenderingStereo && !VSRenderLayer) || FullScreenQuad: vec2
// otherwise: vec3
layout(location=0) in vec3 inPosition;

// VSRenderLayer && FullScreenQuad
layout(location=1) in int inLayer;

#include "shared/interface/light/vertex.glsl"

void main(void){
	lightVertexShaderDefaultOutputs();
	
	int layer = VSRenderLayer && FullScreenQuad ? inLayer : 0;
	
	if(LayeredRendering != LayeredRenderingNone && !VSRenderLayer){
		gl_Position = vec4(vec2(inPosition), 0.0, 1.0);
		
	}else if(FullScreenQuad){
		gl_Position = vec4(vec2(inPosition), 0.0, 1.0);
		vScreenCoord = vec2(inPosition);
		
	}else{
		gl_Position = pMatrixMVP[layer] * vec4(inPosition, 1.0);
		vLightVolumePos = pMatrixMV[layer] * vec4(inPosition, 1.0);
	}
	
	if(VSRenderLayer){
		if(FullScreenQuad){
			vLayer = layer;
			
		}else{
			#ifdef SUPPORTS_VSDRAWPARAM
			vLayer = gl_DrawID;
			#endif
		}
		
		#ifdef SUPPORTS_VSLAYER
		gl_Layer = vLayer;
		#endif
	}
}
