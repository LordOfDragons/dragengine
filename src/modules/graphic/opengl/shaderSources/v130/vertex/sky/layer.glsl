#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

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

#ifdef VS_RENDER_STEREO
	#define inLayer gl_DrawID
	out int vLayer;
#else
	const int inLayer = 0;
#endif

void main( void ){
	gl_Position = vec4( vec3( inPosition, pClearDepthValue ), 1 );
	vTexCoord = inPosition;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
