#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"

UNIFORM_BIND(3) uniform ivec2 pTCClamp;
UNIFORM_BIND(4) uniform int pMipMapLevel;

// SplitVersion
UNIFORM_BIND(5) uniform int pSplitPos;

layout(binding=0) uniform HIGHP sampler2DArray texDepth;

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

#include "shared/interface/2d/fragment.glsl"

#include "shared/defren/sample_depth.glsl"

const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );

void main( void ){
	ivec4 tc = ivec4( gl_FragCoord.xyxy );
	bool splitUseRight;
	
	if(SplitVersion){
		splitUseRight = tc.x >= pSplitPos;
		if(SplitShiftTC && splitUseRight){
			tc.xz -= ivec2(pSplitPos);
		}
	}
	
	tc = tc * tcScale + tcOffset; // s*2, t*2, s*2+1, t*2+1
	if(ClampTC){
		tc = min(tc, pTCClamp.xyxy);
	}
	
	vec4 depth;
	depth.x = sampleDepth( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ); // (s*2, t*2)
	depth.y = sampleDepth( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ); // (s*2+1, t*2)
	depth.z = sampleDepth( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ); // (s*2, t*2+1)
	depth.w = sampleDepth( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ); // (s*2+1, t*2+1)
	
	if(SplitVersion ? !splitUseRight : UseMinFunction){
		depth.xy = min(depth.xy, depth.zw);
		gl_FragDepth = min(depth.x, depth.y);
		
	}else{
		depth.xy = max(depth.xy, depth.zw);
		gl_FragDepth = max(depth.x, depth.y);
	}
}
