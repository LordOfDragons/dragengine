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

#if defined GS_RENDER_STEREO
	#define NO_TRANSFORMATION 1
#endif

uniform mat4x3 pMatrixModel; // matrix model-view-projection
#ifdef WITH_SELECTOR
	uniform mat4x3 pMatrixModel2;
#endif

in vec3 inPosition;
#ifdef WITH_SELECTOR
	in float inSelector;
#endif

#ifdef VS_RENDER_STEREO
	uniform int pDrawIDOffset;
	#define inLayer (gl_DrawID + pDrawIDOffset)
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif

void main( void ){
	vec3 position;
	#ifdef WITH_SELECTOR
		if( bool( inSelector ) ){
			position = pMatrixModel2 * vec4( inPosition, 1 );
		}else{
			position = pMatrixModel * vec4( inPosition, 1 );
		}
	#else
		position = pMatrixModel * vec4( inPosition, 1 );
	#endif
	
	#ifdef NO_TRANSFORMATION
		gl_Position = vec4( position, 1 );
		
	#else
		gl_Position = pMatrixVP[ inLayer ] * vec4( position, 1 );
		gl_Position.z = gl_Position.z * pDebugDepthScale + pDebugDepthOffset;
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
