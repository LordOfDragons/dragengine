precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"

in vec2 inPosition;

#ifdef GS_RENDER_STEREO
	out vec2 vGSTexCoord;
	#define vTexCoord vGSTexCoord
#else
	out vec2 vTexCoord;
#endif

void main( void ){
	gl_Position = vec4( vec3( inPosition, pClearDepthValue ), 1 );
	vTexCoord = inPosition;
}
