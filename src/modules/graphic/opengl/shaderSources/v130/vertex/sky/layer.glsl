#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"

in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	in int inLayer;
#else
	const int inLayer = 0;
#endif

#ifdef GS_RENDER_STEREO
	out vec2 vGSTexCoord;
	#define vTexCoord vGSTexCoord
#else
	out vec2 vTexCoord;
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

const float vZFar = -1; // Z value on far clip plane (cleared depth) in NDC

void main( void ){
	gl_Position = vec4( inPosition, vZFar, 1 );
	vTexCoord = inPosition;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
