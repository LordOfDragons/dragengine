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
#include "shared/defren/ubo_render_parameters.glsl"

layout(location=0) in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	layout(location=1) in int inLayer;
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

// Z value on far clip plane (cleared depth) in NDC
#ifdef INVERSE_DEPTH
	const float vZFar = -1.0;
#else
	const float vZFar = 1.0;
#endif

void main( void ){
	gl_Position = vec4( inPosition, vZFar, 1 );
	vTexCoord = inPosition;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
