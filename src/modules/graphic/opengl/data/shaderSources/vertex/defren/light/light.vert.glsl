#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"

// GSRenderStereo || FullScreenQuad: vec2
// !(GSRenderStereo || FullScreenQuad): vec3
layout(location=0) in vec3 inPosition;

// VSRenderStereo && FullScreenQuad
layout(location=1) in int inLayer;

#include "shared/interface/light_vertex.glsl"

void main(void){
	lightVertexShaderDefaultOutputs();
	
	if(GSRenderStereo){
		gl_Position = vec4(vec2(inPosition), 0.0, 1.0);
	}else{
		if(FullScreenQuad){
			gl_Position = vec4(vec2(inPosition), 0.0, 1.0);
			vScreenCoord = vec2(inPosition);
			
		}else{
			gl_Position = pMatrixMVP[inLayer] * vec4(inPosition, 1.0);
			vLightVolumePos = pMatrixMV[inLayer] * vec4(inPosition, 1.0);
		}
	}
	
	if(VSRenderStereo){
		if(FullScreenQuad){
			vLayer = inLayer;
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
