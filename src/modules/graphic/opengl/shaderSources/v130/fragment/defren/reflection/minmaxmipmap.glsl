precision highp float;
precision highp int;

uniform ivec2 pTCClamp;
uniform int pMipMapLevel;
#ifdef SPLIT_VERSION
	uniform int pSplitPos;
#endif

uniform HIGHP sampler2DArray texDepth;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif
const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );

#ifdef FUNC_MIN
	#define FUNC min
#elif defined FUNC_MAX
	#define FUNC max
#endif

void main( void ){
	ivec4 tc = ivec4( gl_FragCoord.xyxy );
	vec4 depth;
	
	#ifdef SPLIT_VERSION
		bool splitUseRight = ( tc.x >= pSplitPos );
		#ifdef SPLIT_SHIFT_TC
			if( splitUseRight ){
				tc.xz -= ivec2( pSplitPos );
			}
		#endif
	#endif
	
	tc = tc * tcScale + tcOffset; // s*2, t*2, s*2+1, t*2+1
	#ifdef CLAMP_TC
		tc = min( tc, pTCClamp.xyxy );
	#endif
	
	#ifdef DECODE_IN_DEPTH
		depth.x = dot( texelFetch( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ).rgb, unpackDepth );
		depth.y = dot( texelFetch( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ).rgb, unpackDepth );
		depth.z = dot( texelFetch( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ).rgb, unpackDepth );
		depth.w = dot( texelFetch( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ).rgb, unpackDepth );
	#else
		depth.x = texelFetch( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ).r; // (s*2, t*2)
		depth.y = texelFetch( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ).r; // (s*2+1, t*2)
		depth.z = texelFetch( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ).r; // (s*2, t*2+1)
		depth.w = texelFetch( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ).r; // (s*2+1, t*2+1)
	#endif
	
	#ifdef SPLIT_VERSION
		if( splitUseRight ){
			depth.xy = max( depth.xy, depth.zw );
			gl_FragDepth = max( depth.x, depth.y );
			
		}else{
			depth.xy = min( depth.xy, depth.zw );
			gl_FragDepth = min( depth.x, depth.y );
		}
	#else
		depth.xy = FUNC( depth.xy, depth.zw );
		gl_FragDepth = FUNC( depth.x, depth.y );
	#endif
}
