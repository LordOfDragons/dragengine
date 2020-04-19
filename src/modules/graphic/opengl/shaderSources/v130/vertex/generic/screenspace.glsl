precision mediump float;
precision mediump int;

#ifndef NO_POSTRANSFORM
	uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
#endif
#ifndef NO_TCTRANSFORM
	uniform vec4 pTCTransform; // scaleX, scaleY, offsetX, offsetY
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
	#ifdef NO_POSTRANSFORM
		gl_Position = vec4( inPosition, 0.0, 1.0 );
	#else
		gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
	#endif
	
	#ifndef NO_TEXCOORD
		#ifdef NO_TCTRANSFORM
			vTexCoord = inPosition;
		#else
			vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
		#endif
	#endif
}
