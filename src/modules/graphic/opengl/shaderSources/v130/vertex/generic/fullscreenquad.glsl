#ifdef EXT_ARB_SHADER_VIEWPORT_LAYER_ARRAY
	#extension GL_ARB_shader_viewport_layer_array : require
#endif
#ifdef EXT_ARB_SHADER_DRAW_PARAMETERS
	#extension GL_ARB_shader_draw_parameters : require
#endif

precision highp float;
precision highp int;

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //
//                                                        //
//             use screenspace.glsl instead               //
//    using defines: FULLSCREENQUAD, NO_POSTRANSFORM      //
//                                                        //
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //

#ifndef GEOMETRY_SHADER
	#ifdef GS_RENDER_STEREO
		#define GEOMETRY_SHADER 1
	#endif
#endif

#ifndef NO_TEXCOORD
	uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY
#endif

in vec2 inPosition;

#ifndef NO_TEXCOORD
	#ifdef GEOMETRY_SHADER
		out vec2 vGSTexCoord;
		#define vTexCoord vGSTexCoord
	#else
		out vec2 vTexCoord;
	#endif
#endif

#ifdef VS_RENDER_STEREO
	#define inLayer gl_DrawID
	flat out int vLayer;
#else
	const int inLayer = 0;
#endif

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	#ifndef NO_TEXCOORD
		vTexCoord = inPosition.xy * pQuadParams.xy + pQuadParams.zw;
	#endif
	
	#ifdef VS_RENDER_STEREO
		gl_Layer = inLayer;
		vLayer = inLayer;
	#endif
}
