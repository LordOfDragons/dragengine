#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

uniform mat4x3 pMatrixModel;
uniform mat4 pMatrixVP;
uniform mat4 pMatrixVP2;

in vec3 inPosition;
in vec2 inTexCoord;

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


void main( void ){
	vec3 position = pMatrixModel * vec4( inPosition, 1 );
	
	#ifdef GS_RENDER_STEREO
		gl_Position = vec4( position, 1 );
	#else
		gl_Position = ( inLayer == 0 ? pMatrixVP : pMatrixVP2 ) * vec4( position, 1 );
	#endif
	
	vTexCoord = inTexCoord;
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
	#endif
}
