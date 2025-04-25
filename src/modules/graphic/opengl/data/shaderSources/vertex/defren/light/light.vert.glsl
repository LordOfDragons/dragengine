#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/light/ubo_instance_parameters.glsl"

#if defined GS_RENDER_STEREO || defined FULLSCREENQUAD
	layout(location=0) in vec2 inPosition;
#else
	layout(location=0) in vec3 inPosition;
#endif

#ifdef VS_RENDER_STEREO
	#ifdef FULLSCREENQUAD
		layout(location=1) in int inLayer;
	#else
		#define inLayer gl_DrawID
	#endif
#else
	const int inLayer = 0;
#endif

#ifndef GS_RENDER_STEREO
	#ifdef FULLSCREENQUAD
		out vec2 vScreenCoord;
	#else
		out vec3 vLightVolumePos;
	#endif
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

void main(void){
	#ifdef GS_RENDER_STEREO
		gl_Position = vec4(inPosition, 0.0, 1.0);
	#else
		#ifdef FULLSCREENQUAD
			gl_Position = vec4(inPosition, 0.0, 1.0);
			vScreenCoord = inPosition;
		#else
			gl_Position = pMatrixMVP[inLayer] * vec4(inPosition, 1.0);
			vLightVolumePos = pMatrixMV[inLayer] * vec4(inPosition, 1.0);
		#endif
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
