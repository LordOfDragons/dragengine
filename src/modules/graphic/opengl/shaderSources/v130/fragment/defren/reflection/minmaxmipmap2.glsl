precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"

uniform ivec2 pTCClamp;
#ifdef DOWNSAMPLE
	uniform int pMipMapLevel;
#endif

uniform HIGHP sampler2DArray texData;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

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

out vec2 outData;

#include "v130/shared/defren/sample_depth.glsl"

const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );

void main( void ){
	ivec4 tc = min( ivec4( gl_FragCoord.xyxy ) * tcScale + tcOffset, pTCClamp.xyxy ); // s*2, t*2, s*2+1, t*2+1
	vec4 data1, data2;
	
	#ifdef INITIAL
		data1.x = sampleDepth( texData, ivec3( tc.xy, vLayer ) ); // (s*2, t*2)
		data1.y = sampleDepth( texData, ivec3( tc.zy, vLayer ) ); // (s*2+1, t*2)
		data1.z = sampleDepth( texData, ivec3( tc.xw, vLayer ) ); // (s*2, t*2+1)
		data1.w = sampleDepth( texData, ivec3( tc.zw, vLayer ) ); // (s*2+1, t*2+1)
		
		data2.xz = min( data1.xy, data1.zw );
		data2.yw = max( data1.xy, data1.zw );
		
	#elif DOWNSAMPLE
		data1.xy = texelFetch( texData, ivec3( tc.xy, vLayer ), pMipMapLevel ).rg; // (s*2, t*2)
		data1.zw = texelFetch( texData, ivec3( tc.zy, vLayer ), pMipMapLevel ).rg; // (s*2+1, t*2)
		data2.xy = texelFetch( texData, ivec3( tc.xw, vLayer ), pMipMapLevel ).rg; // (s*2, t*2+1)
		data2.zw = texelFetch( texData, ivec3( tc.zw, vLayer ), pMipMapLevel ).rg; // (s*2+1, t*2+1)
		
		data2.xz = min( data1.xz, data2.xz );
		data2.yw = max( data1.yw, data2.yw );
	#endif
	
	outData.x = min( data2.x, data2.z );
	outData.y = max( data2.y, data2.w );
}
