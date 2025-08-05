#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform ivec2 pTCClamp;
uniform int pMipMapLevel;

uniform HIGHP sampler2DArray texDepth;

#include "shared/interface/2d_fragment.glsl"

const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );
#ifndef USE_MIN_FUNCTION
	const vec4 weights = vec4( 0.25 );
#endif

void main( void ){
	ivec4 tc = min( ivec4( gl_FragCoord.xyxy ) * tcScale + tcOffset, pTCClamp.xyxy ); // s*2, t*2, s*2+1, t*2+1
	vec4 depth;
	
	depth.x = texelFetch( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ).r; // (s*2, t*2)
	depth.y = texelFetch( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ).r; // (s*2+1, t*2)
	depth.z = texelFetch( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ).r; // (s*2, t*2+1)
	depth.w = texelFetch( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ).r; // (s*2+1, t*2+1)
	
	#ifdef USE_MIN_FUNCTION
		if(InverseDepth){
			depth.xy = max( depth.xy, depth.zw );
			gl_FragDepth = max( depth.x, depth.y );
			
		}else{
			depth.xy = min( depth.xy, depth.zw );
			gl_FragDepth = min( depth.x, depth.y );
		}
	#else
		gl_FragDepth = dot( depth, weights );
	#endif
}
