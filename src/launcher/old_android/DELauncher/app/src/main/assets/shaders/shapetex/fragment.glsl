#version 100

precision mediump float;

uniform mat4 pColorTransform; // left most 4x4 part
uniform vec4 pColorTransform2; // right most colon
uniform vec4 pGamma; // red, green, blue, 1
uniform vec4 pClipRect; // left, top, right, bottom
uniform vec4 pTCClamp; // left, top, right, bottom

uniform sampler2D texColor;

varying vec2 vTexCoord;

void main( void ){
	// clamp to viewport. avoids changing glViewport/glScissor all the time
	if( any( lessThan( gl_FragCoord.xy, pClipRect.xy ) ) || any( greaterThan( gl_FragCoord.xy, pClipRect.zw ) ) ){
		discard;
	}
	
	// fragment color
	gl_FragColor = pColorTransform * pow(
		texture2D( texColor, clamp( vTexCoord, pTCClamp.xy, pTCClamp.zw ) ), pGamma )
			+ pColorTransform2;
}
