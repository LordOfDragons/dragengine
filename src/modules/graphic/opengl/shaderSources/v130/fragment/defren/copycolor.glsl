precision mediump float;
precision mediump int;

#ifdef MIPMAP
	uniform float pMipMapLevel;
#endif
#ifdef TCCLAMP
	#ifdef MIPMAP
		uniform vec2 pTCClamp;
	#else
		uniform ivec2 pTCClamp;
	#endif
#endif
uniform mediump sampler2D texColor;

in vec2 vTexCoord;

out mediump vec4 outColor;

void main( void ){
	#ifdef MIPMAP
		#ifdef TCCLAMP
			outColor = textureLod( texColor, min( vTexCoord, pTCClamp ), pMipMapLevel );
		#else
			outColor = textureLod( texColor, vTexCoord, pMipMapLevel );
		#endif
	#else
		#ifdef TCCLAMP
			outColor = texelFetch( texColor, min( ivec2( gl_FragCoord.xy ), pTCClamp ), 0 );
		#else
			outColor = texelFetch( texColor, ivec2( gl_FragCoord.xy ), 0 );
		#endif
	#endif
}
