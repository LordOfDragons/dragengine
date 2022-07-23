#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_instance_parameters.glsl"

in vec3 inPosition;

#ifdef FULLSCREENQUAD
	out vec2 vScreenCoord;
#else
	out vec3 vLightVolumePos;
#endif

void main( void ){
	#ifdef FULLSCREENQUAD
		gl_Position = vec4( inPosition, 1 );
		vScreenCoord = inPosition.xy;
	#else
		gl_Position = pMatrixMVP * vec4( inPosition, 1 );
		vLightVolumePos = pMatrixMV * vec4( inPosition, 1 );
	#endif
}
