precision highp float;
precision highp int;

#ifdef WITH_DEPTH
uniform HIGHP sampler2D texDepth;
#endif

uniform vec4 pColor;
#ifdef WITH_DEPTH
uniform vec2 pSCToDTC; // screen coordinates to depth texture coordinates
#endif

out vec4 outColor;

#ifdef ENCODE_DEPTH
const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif // ENCODE_DEPTH

void main( void ){
	#ifdef WITH_DEPTH
		#ifdef INVERSE_DEPTH
			#ifdef ENCODE_DEPTH
				if( gl_FragCoord.z < dot( texture( texDepth, gl_FragCoord.xy * pSCToDTC ).rgb, unpackDepth ) ) discard;
			#else
				if( gl_FragCoord.z < texture( texDepth, gl_FragCoord.xy * pSCToDTC ).r ) discard;
			#endif
			
		#else
			#ifdef ENCODE_DEPTH
				if( gl_FragCoord.z > dot( texture( texDepth, gl_FragCoord.xy * pSCToDTC ).rgb, unpackDepth ) ) discard;
			#else
				if( gl_FragCoord.z > texture( texDepth, gl_FragCoord.xy * pSCToDTC ).r ) discard;
			#endif
		#endif
	#endif
	
	outColor = pColor;
}
