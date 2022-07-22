precision highp float;
precision highp int;

uniform HIGHP sampler2DArray texDepth;
#ifdef COPY_COLOR
	uniform mediump sampler2DArray texColor;
#endif

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

#ifdef COPY_COLOR
	out vec4 outColor;
#endif
#ifdef ENCODED_DEPTH
	out vec4 outDepth;
#endif

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	#ifdef ENCODED_DEPTH
		outDepth = texelFetch( texDepth, tc, 0 );
		#ifdef DEPTH_TEST
			/* if( outDepth.rgb == vec3( 0.0 ) ){
				outDepth.rgb = vec3( 1.0, 0.0, 0.0 );
			} */
			if( outDepth.rgb == vec3( 0.0 ) ){
				discard;
			}
		#endif
	#else
		gl_FragDepth = texelFetch( texDepth, tc, 0 ).r;
		#ifdef DEPTH_TEST
			/* if( gl_FragDepth == 0.0 ){
				gl_FragDepth = 1.0;
			} */
			#ifdef SHADOW_INVERSE_DEPTH
			if( gl_FragDepth == 1.0 ) discard;
			#else
			if( gl_FragDepth == 0.0 ) discard;
			#endif
		#endif
	#endif
	
	#ifdef COPY_COLOR
		outColor = texelFetch( texColor, tc, 0 );
	#endif
}
