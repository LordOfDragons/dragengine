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

layout(location=0) in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	layout(location=1) in int inLayer;
#endif

#ifdef VS_RENDER_STEREO
	flat out int vLayer;
#endif

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
