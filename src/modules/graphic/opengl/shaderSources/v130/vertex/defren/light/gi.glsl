#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

in vec3 inPosition;

#ifdef GS_RENDER_STEREO
	out vec2 vGSScreenCoord;
#else
	out vec2 vScreenCoord;
#endif

#ifdef VS_RENDER_STEREO
	#define inLayer gl_DrawID
	out int vLayer;
#else
	const int inLayer = 0;
#endif

void main( void ){
	gl_Position = vec4( inPosition, 1 );
	
	#ifdef GS_RENDER_STEREO
		vGSScreenCoord = inPosition.xy;
	#else
		vScreenCoord = inPosition.xy;
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
