precision HIGHP float;
precision HIGHP int;

uniform HIGHP sampler2DArray texOccMap;
#ifdef DUAL_OCCMAP
uniform HIGHP sampler2DArray texOccMap2;
#endif

uniform vec2 pScaleSize;
uniform float pBaseLevel;
#ifdef DUAL_OCCMAP
uniform vec2 pScaleSize2;
uniform float pBaseLevel2;
#endif

in vec3 vMinExtend;
in vec2 vMaxExtend;
#ifdef GS_RENDER_STEREO
	in vec3 vMinExtend2;
	in vec2 vMaxExtend2;
#endif

out float outResult;

const float baselog = log2( 3.0 ); // 1.5849625007211563

bool testPoint( in int layer, in vec3 minExtend, in vec2 maxExtend ){
	vec2 size = ( maxExtend - vec2( minExtend ) ) * pScaleSize;
	
	if( min( size.x, size.y ) < 0.1 ){
		return false;
	}
	
	/*
	float maxSize = max( size.x, size.y );
	float level = ceil( log2( maxSize ) );
	vec4 samples;
	vec3 tc;
	
	tc = vec3( minExtend.xy, vLayer );
	samples.x = textureLod( texOccMap, tc, level ).x;
	tc.x = maxExtend.x;
	samples.y = textureLod( texOccMap, tc, level ).x;
	tc.y = maxExtend.y;
	samples.z = textureLod( texOccMap, tc, level ).x;
	tc.x = minExtend.x;
	samples.w = textureLod( texOccMap, tc, level ).x;
	samples.xy = max( samples.xy, samples.zw );
	outResult = float( minExtend.z <= max( samples.x, samples.y ) );
	*/
	
	// note about the calculation of the lod level to use. we tap 4x4 pixels to obtain the depth value
	// to compare against. hence we need the lod level where the box covers at most 4 pixels in a row.
	// since the box can though be exactly in the middle over these four pixels the maximal size of
	// the box can not exceed 3 pixels. The following graphics illustrates the problem:
	// | x | x | x | x |
	// here a pixel is a |x| block with the x the center of the pixel. if the box starts in the middle
	// of the first pixel 4 taps are done each a pixel away from the last one if the size of the box is
	// 3 pixels. if it is larger up to 5 pixels are crossed and one pixel has to be skipped falsifying
	// the result. hence we choose the lod level where the maximum size of the box is at most 3 to gain
	// as much information from our 4x4 taps without compromising the correctness of the calculation.
	// for this subtracting log2(3) results in an as optimal usage of the 4x4 block as reasonable
	// without obtaining incorrect results even in the worst case.
	float maxSize = max( size.x, size.y );
	float level = max( ceil( log2( maxSize ) - baselog ), pBaseLevel );
	vec4 steps = mix( minExtend.xxyy, maxExtend.xxyy, vec4( 1.0 / 3.0, 2.0 / 3.0, 1.0 / 3.0, 2.0 / 3.0 ) );
	vec4 samples, samplesAll;
	vec3 tc;
	
	// test pattern where we have to change only one texture coordinate component at the time:
	// [ 6 7 10 11 ]
	// [ 5 8  9 12 ]
	// [ 4 3 14 13 ]
	// [ 1 2 15 16 ]
	tc = vec3( minExtend.xy, layer );
	samples.x = textureLod( texOccMap, tc, level ).x;
	tc.x = steps.x;
	samples.y = textureLod( texOccMap, tc, level ).x;
	tc.y = steps.z;
	samples.z = textureLod( texOccMap, tc, level ).x;
	tc.x = vMinExtend.x;
	samples.w = textureLod( texOccMap, tc, level ).x;
	samples.xy = max( samples.xy, samples.zw );
	samplesAll.x = max( samples.x, samples.y );
	
	tc.y = steps.w;
	samples.x = textureLod( texOccMap, tc, level ).x;
	tc.y = vMaxExtend.y;
	samples.y = textureLod( texOccMap, tc, level ).x;
	tc.x = steps.x;
	samples.z = textureLod( texOccMap, tc, level ).x;
	tc.y = steps.w;
	samples.w = textureLod( texOccMap, tc, level ).x;
	samples.xy = max( samples.xy, samples.zw );
	samplesAll.y = max( samples.x, samples.y );
	
	tc.x = steps.y;
	samples.x = textureLod( texOccMap, tc, level ).x;
	tc.y = vMaxExtend.y;
	samples.y = textureLod( texOccMap, tc, level ).x;
	tc.x = vMaxExtend.x;
	samples.z = textureLod( texOccMap, tc, level ).x;
	tc.y = steps.w;
	samples.w = textureLod( texOccMap, tc, level ).x;
	samples.xy = max( samples.xy, samples.zw );
	samplesAll.z = max( samples.x, samples.y );
	
	tc.y = steps.z;
	samples.x = textureLod( texOccMap, tc, level ).x;
	tc.x = steps.y;
	samples.y = textureLod( texOccMap, tc, level ).x;
	tc.y = vMinExtend.y;
	samples.z = textureLod( texOccMap, tc, level ).x;
	tc.x = vMaxExtend.x;
	samples.w = textureLod( texOccMap, tc, level ).x;
	samples.xy = max( samples.xy, samples.zw );
	samplesAll.w = max( samples.x, samples.y );
	
	samplesAll.xy = max( samplesAll.xy, samplesAll.zw );
	
	// using float(bool) can cause segfault on android devices so an explicit version
	// is used. this is anyways more clear to understand.
	//outResult = float( vMinExtend.z <= max( samplesAll.x, samplesAll.y ) );
	return minExtend.z <= max( samplesAll.x, samplesAll.y );
}

void main( void ){
	bool result = testPoint( 0, vMinExtend, vMaxExtend );
	#ifdef GS_RENDER_STEREO
		result = result || testPoint( 1, vMinExtend2, vMaxExtend2 );
	#endif
	
	outResult = result ? 1 : 0;
}
