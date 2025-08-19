#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

UNIFORM_BIND(3) uniform mat4x4 pColorTransform; // left most 4x4 part
UNIFORM_BIND(4) uniform vec4 pColorTransform2; // right most colon
UNIFORM_BIND(5) uniform vec4 pGamma; // red, green, blue, 1
UNIFORM_BIND(6) uniform vec4 pClipRect; // left, top, right, bottom
UNIFORM_BIND(7) uniform vec4 pTCClamp; // left, top, right, bottom

// WithTexture
layout(binding=0) uniform lowp sampler2D texColor;

// WithRenderWorld
layout(binding=0) uniform lowp sampler2DArray texColorArray;

// WithMask
layout(binding=1) uniform lowp sampler2D texMask;

#include "shared/interface/2d/fragment.glsl"

// vTexCoord: WithTexture or WithRenderWorld

// WithMask
VARYING_BIND(3) in vec2 vTexCoordMask;

layout(location=0) out vec4 outColor;

void main( void ){
	// clamp to viewport. avoids changing glViewport/glScissor all the time
	if( any( lessThan( gl_FragCoord.xy, pClipRect.xy ) ) || any( greaterThan( gl_FragCoord.xy, pClipRect.zw ) ) ){
		discard;
	}
	
	// fragment color
	if(WithTexture){
		outColor = pow( texture( texColor, clamp( vTexCoord, pTCClamp.xy, pTCClamp.zw ) ), pGamma );
		outColor = pColorTransform * outColor;
	}else if(WithRenderWorld){
		outColor = pow( texture( texColorArray, vec3( clamp( vTexCoord, pTCClamp.xy, pTCClamp.zw ), 0 ) ), pGamma );
		outColor = pColorTransform * outColor;
	}else{
		outColor = vec4( pColorTransform[0][0], pColorTransform[1][1], pColorTransform[2][2], pColorTransform[3][3] );
	}
	outColor += pColorTransform2;
	
	// mask
	if(WithMask){
		outColor.a *= texture( texMask, vTexCoordMask ).r;
	}
}
