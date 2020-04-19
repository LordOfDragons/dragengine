// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif



// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/light/ubo_instance_parameters.glsl"



// Inputs
///////////

in vec3 inPosition;



// Outputs
////////////

#ifdef FULLSCREENQUAD
	out vec2 vScreenCoord;
#else
	out vec3 vLightVolumePos;
#endif



// Main Function
//////////////////

void main( void ){
	#ifdef FULLSCREENQUAD
		gl_Position = vec4( inPosition, 1.0 );
		vScreenCoord = inPosition.xy;
	#else
		gl_Position = pMatrixMVP * vec4( inPosition, 1.0 );
		vLightVolumePos = pMatrixMV * vec4( inPosition, 1.0 );
	#endif
}
