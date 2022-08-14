#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_instance_parameters.glsl"

#if defined GS_RENDER_STEREO || defined FULLSCREENQUAD
	in vec2 inPosition;
#else
	in vec3 inPosition;
#endif

#ifdef VS_RENDER_STEREO
	#ifdef FULLSCREENQUAD
		in int inLayer;
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

void main( void ){
	#ifdef GS_RENDER_STEREO
		gl_Position = vec4( inPosition, 0, 1 );
	#else
		#ifdef FULLSCREENQUAD
			gl_Position = vec4( inPosition, 0, 1 );
			vScreenCoord = inPosition;
		#else
			gl_Position = pMatrixMVP[ inLayer ] * vec4( inPosition, 1 );
			vLightVolumePos = pMatrixMV[ inLayer ] * vec4( inPosition, 1 );
		#endif
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
