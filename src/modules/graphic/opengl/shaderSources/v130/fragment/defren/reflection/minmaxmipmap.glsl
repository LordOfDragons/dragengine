precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"

uniform ivec2 pTCClamp;
uniform int pMipMapLevel;
#ifdef SPLIT_VERSION
	uniform int pSplitPos;
#endif

uniform HIGHP sampler2DArray texDepth;

// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
//       STEREO SUPPORT NOT ENABLED FOR THIS SHADER !!!
//
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING

// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
// 
//       SHADER IS CURRENTLY DISABLED AND MOST PROBABLY BROKEN
// 
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

#include "v130/shared/defren/sample_depth.glsl"

const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );

#ifdef FUNC_MIN
	#define FUNC min
#elif defined FUNC_MAX
	#define FUNC max
#endif

void main( void ){
	ivec4 tc = ivec4( gl_FragCoord.xyxy );
	
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
	
	vec4 depth;
	depth.x = sampleDepth( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ); // (s*2, t*2)
	depth.y = sampleDepth( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ); // (s*2+1, t*2)
	depth.z = sampleDepth( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ); // (s*2, t*2+1)
	depth.w = sampleDepth( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ); // (s*2+1, t*2+1)
	
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
