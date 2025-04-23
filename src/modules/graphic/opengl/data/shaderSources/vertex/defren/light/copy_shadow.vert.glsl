#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_AMD_VERTEX_SHADER_LAYER
	#extension GL_AMD_vertex_shader_layer : require
#endif

precision mediump float;
precision mediump int;

layout(location=0) in vec2 inPosition;
#ifdef SHADOW_CUBE
	layout(location=1) in int inLayer;
	layout(location=2) in vec3 inTexCoord;
#endif

#ifdef SHADOW_CUBE
	#ifdef GS_LAYER
		out vec3 vGSTexCoord;
		flat out int vGSLayer;
	#else
		out vec3 vTexCoord;
	#endif
#endif

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	
	#ifdef SHADOW_CUBE
		#ifdef GS_LAYER
			vGSTexCoord = inTexCoord;
			vGSLayer = inLayer;
		#else
			vTexCoord = inTexCoord;
			#ifdef VS_LAYER
				gl_Layer = inLayer;
			#endif
		#endif
	#endif
}
