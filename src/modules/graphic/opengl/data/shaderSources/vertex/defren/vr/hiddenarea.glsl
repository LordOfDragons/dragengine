#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

in vec3 inPosition;

#ifdef VS_RENDER_STEREO
	#define inLayer gl_DrawID
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif

void main( void ){
	gl_Position = vec4( inPosition.xy * vec2( 2 ) - vec2( 1 ), 1, 1 );
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
