precision mediump float;
precision mediump int;

uniform mat4x4 pColorTransform; // left most 4x4 part
uniform vec4 pColorTransform2; // right most colon
uniform vec4 pGamma; // red, green, blue, 1
uniform vec4 pClipRect; // left, top, right, bottom
uniform vec4 pTCClamp; // left, top, right, bottom

#ifdef WITH_TEXTURE
uniform lowp sampler2D texColor;
#endif

#ifdef WITH_TEXTURE
in vec2 vTexCoord;
#endif

out vec4 outColor;

void main( void ){
	// clamp to viewport. avoids changing glViewport/glScissor all the time
	if( any( lessThan( gl_FragCoord.xy, pClipRect.xy ) ) || any( greaterThan( gl_FragCoord.xy, pClipRect.zw ) ) ){
		discard;
	}
	
	// fragment color
	#ifdef WITH_TEXTURE
	outColor = pColorTransform * pow( texture( texColor, clamp( vTexCoord, pTCClamp.xy, pTCClamp.zw ) ), pGamma ) + pColorTransform2;
	#else
	outColor = vec4( pColorTransform[0][0], pColorTransform[1][1], pColorTransform[2][2], pColorTransform[3][3] ) + pColorTransform2;
	#endif
}
