#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_instance_parameters.glsl"

in vec3 inPosition;

#ifndef GS_RENDER_STEREO
	#ifdef FULLSCREENQUAD
		out vec2 vScreenCoord;
	#else
		out vec3 vLightVolumePos;
	#endif
#endif

void main( void ){
	#ifdef GS_RENDER_STEREO
		gl_Position = vec4( inPosition, 1 );
	#else
		#ifdef FULLSCREENQUAD
			gl_Position = vec4( inPosition, 1 );
			vScreenCoord = inPosition.xy;
		#else
			gl_Position = pMatrixMVP * vec4( inPosition, 1 );
			vLightVolumePos = pMatrixMV * vec4( inPosition, 1 );
		#endif
	#endif
}
