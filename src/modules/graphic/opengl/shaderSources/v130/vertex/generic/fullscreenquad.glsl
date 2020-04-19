precision highp float;
precision highp int;

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //
//                                                        //
//             use screenspace.glsl instead               //
//                                                        //
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //

#ifndef NO_TEXCOORD
	uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY
#endif

in vec2 inPosition;

#ifndef NO_TEXCOORD
	#ifdef GEOMETRY_SHADER
		out vec2 vGSTexCoord;
	#else
		out vec2 vTexCoord;
	#endif
#endif

void main( void ){
	gl_Position = vec4( vec3( inPosition, 0.0 ), 1.0 );
	#ifndef NO_TEXCOORD
		#ifdef GEOMETRY_SHADER
			vGSTexCoord = inPosition.xy * pQuadParams.xy + pQuadParams.zw;
		#else
			vTexCoord = inPosition.xy * pQuadParams.xy + pQuadParams.zw;
		#endif
	#endif
}
