#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

// uniform vec4 pOptions; // rows, cols, stepU, stepV
// uniform vec3 pKernel1;
// uniform vec3 pKernel2;
// uniform vec3 pKernel3;
UNIFORM_BIND(3) uniform ivec2 pTCClamp;

uniform mediump sampler2DArray texColor;

#include "shared/interface/2d_fragment.glsl"

layout(location=0) out mediump vec4 outColor;

const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );
const vec4 weights = vec4( 0.25 );

void main( void ){
	ivec4 tc = min( ivec4( gl_FragCoord.xyxy ) * tcScale + tcOffset, pTCClamp.xyxy ); // s*2, t*2, s*2+1, t*2+1
	
	outColor =  texelFetch( texColor, ivec3( tc.xy, vLayer ), 0 ); // (s*2, t*2)
	outColor += texelFetch( texColor, ivec3( tc.zy, vLayer ), 0 ); // (s*2+1, t*2)
	outColor += texelFetch( texColor, ivec3( tc.xw, vLayer ), 0 ); // (s*2, t*2+1)
	outColor += texelFetch( texColor, ivec3( tc.zw, vLayer ), 0 ); // (s*2+1, t*2+1)
	
	outColor *= weights;
}
