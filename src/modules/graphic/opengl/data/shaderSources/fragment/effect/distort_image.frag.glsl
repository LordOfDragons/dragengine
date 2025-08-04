#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pDistortTransform;

uniform mediump sampler2DArray texColor;
uniform lowp sampler2D texDistort;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out mediump vec4 outColor;

void main( void ){
	// the image coordinates have to be flipped. this can not be done in the vertex
	// shader since the rendered image requires non-flipped coordinates. flipping
	// the y texture coordinate here is faster than calculating the texture coordinates
	// from the screen coordinates (vScreenCoord)
	vec2 tcDistort = vec2( vTexCoord.x, 1 - vTexCoord.y );
	
	// distortion is stored as red(x) and green(y). this allows normal maps to be used
	vec2 distortion = texture( texDistort, tcDistort ).rg;
	
	// distortion strength
	distortion = distortion * pDistortTransform.xy + pDistortTransform.zw;
	
	outColor = texture( texColor, vec3( vTexCoord + distortion, vLayer ) );
}
