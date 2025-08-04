#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

uniform ivec2 pTCClamp;
uniform int pMipMapLevel;

uniform HIGHP sampler2DArray texDepth;

#include "shared/interface/2d_fragment.glsl"

// DecodeInDepth
layout(location=0) out vec4 outDepth;

// DecodeInDepth
const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
const vec3 packShift = vec3( 1.0, 256.0, 65536.0 );
const vec3 packMask = vec3( 1.0 / 256.0, 1.0 / 256.0, 0.0 );

const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );
#ifndef USE_MIN_FUNCTION
	const vec4 weights = vec4( 0.25 );
#endif

void main( void ){
	ivec4 tc = min( ivec4( gl_FragCoord.xyxy ) * tcScale + tcOffset, pTCClamp.xyxy ); // s*2, t*2, s*2+1, t*2+1
	vec4 depth;
	
	if(DecodeInDepth){
		depth.x = dot( texelFetch( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2, t*2)
		depth.y = dot( texelFetch( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2+1, t*2)
		depth.z = dot( texelFetch( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2, t*2+1)
		depth.w = dot( texelFetch( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2+1, t*2+1)
		
		vec3 encoded;
		#ifdef USE_MIN_FUNCTION
			if(InverseDepth){
				depth.xy = max( depth.xy, depth.zw );
				encoded = fract( packShift * vec3( max( depth.x, depth.y ) ) );
				
			}else{
				depth.xy = min( depth.xy, depth.zw );
				encoded = fract( packShift * vec3( min( depth.x, depth.y ) ) );
			}
		#else
			vec3 encoded = fract( packShift * vec3( dot( depth, weights ) ) );
		#endif
		outDepth = vec4( encoded - ( encoded.yzz * packMask ), 1.0 );
		
	}else{
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
}
