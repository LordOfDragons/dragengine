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

// !GSRenderStereo && FullScreenQuad
VARYING_BIND(0) out vec2 vScreenCoord;

// !GSRenderStereo && !FullScreenQuad
VARYING_BIND(1) out vec3 vLightVolumePos;

// VSRenderStereo
VARYING_BIND(2) flat out int vLayer;

void main(void){
	#ifdef GS_RENDER_STEREO
		gl_Position = vec4(vec2(inPosition), 0.0, 1.0);
	#else
		if(FullScreenQuad){
			gl_Position = vec4(vec2(inPosition), 0.0, 1.0);
			vScreenCoord = vec2(inPosition);
			
		}else{
			gl_Position = pMatrixMVP[inLayer] * vec4(inPosition, 1.0);
			vLightVolumePos = pMatrixMV[inLayer] * vec4(inPosition, 1.0);
		}
	#endif
	
	vLayer = 0;
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
