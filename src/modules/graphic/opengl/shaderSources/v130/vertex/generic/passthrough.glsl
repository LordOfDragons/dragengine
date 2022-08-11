#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

precision highp float;
precision highp int;

in vec2 inPosition;

#ifdef VS_RENDER_STEREO
	#define inLayer gl_DrawID
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif

void main( void ){
	gl_Position = vec4( vec3( inPosition, 0 ), 1 );
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
