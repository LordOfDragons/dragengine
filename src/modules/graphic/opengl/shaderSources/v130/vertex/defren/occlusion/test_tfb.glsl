precision highp float;
precision highp int;

uniform HIGHP sampler2D texOccMap;
#ifdef DUAL_OCCMAP
uniform HIGHP sampler2D texOccMap2;
#endif

uniform mat4 pMatrix; // camera-rotation and projection
uniform vec2 pScaleSize;
uniform float pBaseLevel;
uniform float pClipNear;
#ifdef DUAL_OCCMAP
uniform mat4 pMatrix2;
uniform vec2 pScaleSize2;
uniform float pBaseLevel2;
uniform float pClipNear2;
#endif
#ifdef FRUSTUM_TEST
uniform mat4 pMatrix2;
uniform float pClipNear2;
uniform vec3 pFrustumNormal1;
uniform vec3 pFrustumNormal2;
uniform vec3 pFrustumNormal3;
uniform vec3 pFrustumNormal4;
uniform vec4 pFrustumTestAdd;
uniform vec4 pFrustumTestMul;
#endif

in vec3 inMinExtend;
in vec3 inMaxExtend;

out float fbResult;

const float baselog = log2( 3.0 ); // 1.5849625007211563
#ifdef ENSURE_MIN_SIZE
	const vec3 epsilonSize = vec3( 0.005 );
#endif



const bvec3 vBoxCorner[ 8 ] = {
	bvec3( false, false, false ), // point -x, -y, -z
	bvec3( true,  false, false ), // point  x, -y, -z
	bvec3( false, true,  false ), // point -x,  y, -z
	bvec3( true,  true,  false ), // point  x,  y, -z
	bvec3( false, false, true  ), // point -x, -y,  z
	bvec3( true,  false, true  ), // point  x, -y,  z
	bvec3( false, true,  true  ), // point -x,  y,  z
	bvec3( true,  true,  true  )  // point  x,  y,  z
};

// calculate screen space AABB projection. returns true if all points are in front of near plane
bool calcScreenAABB( out vec3 minExtend, out vec3 maxExtend, in mat4 matrix,
in vec3 inputMinExtend, in vec3 inputMaxExtend ){
	vec4 screen, world = vec4( 1.0 );
	int i;
	
	minExtend = vec3( 1.0 );
	maxExtend = vec3( -1.0 );
	
	for( i=0; i<8; i++ ){
		world.xyz = mix( inputMinExtend, inputMaxExtend, vBoxCorner[ i ] );
		screen = matrix * world;
		if( screen.z <= 0.0 ){
			return false;
		}
		screen.xy /= screen.ww;
		
		minExtend = min( minExtend, screen.xyz );
		maxExtend = max( maxExtend, screen.xyz );
	}
	
	minExtend = max( minExtend, vec3( -1.0 ) );
	maxExtend = min( maxExtend, vec3( 1.0 ) );
	return true;
}



const vec2 vOneTwoThird = vec2( 1.0 / 3.0, 2.0 / 3.0 );

// test box against occlusion map
bool testBox( out float largestSample, in vec2 minExtend, in vec2 maxExtend,
in float minDepth, in vec2 scaleSize, in float baseLevel, sampler2D occmap ){
	vec2 size = ( maxExtend - minExtend ) * scaleSize;
	
	//if( min( size.x, size.y ) < 0.01 ){ //0.1 ){
	//	result = 0.0;
	//	largestSample = 1.0;
	//	
	//}else{
		// ensure the minimum size is not below 2 pixels. this avoids a problem with steep geometry causing
		// false invisibility of elements
		vec2 ensureMinSize = vec2( 2.0 ); // 4 for sun and still few problems??
		vec2 adjustSize = max( vec2( 0.0 ), ensureMinSize - size ) * vec2( 0.5 ) / scaleSize;
		minExtend -= adjustSize;
		maxExtend += adjustSize;
		size = max( ensureMinSize, size );
		
		// note about the calculation of the lod level to use. we tap 4x4 pixels to obtain
		// the depth value to compare against. hence we need the lod level where the box
		// covers at most 4 pixels in a row. since the box can though be exactly in the
		// middle over these four pixels the maximal size of the box can not exceed 3
		// pixels. The following graphics illustrates the problem:
		// | x | x | x | x |
		// here a pixel is a |x| block with the x the center of the pixel. if the box
		// starts in the middle of the first pixel 4 taps are done each a pixel away from
		// the last one if the size of the box is 3 pixels. if it is larger up to 5 pixels
		// are crossed and one pixel has to be skipped falsifying the result. hence we
		// choose the lod level where the maximum size of the box is at most 3 to gain
		// as much information from our 4x4 taps without compromising the correctness of
		// the calculation. for this subtracting log2(3) results in an as optimal usage of
		// the 4x4 block as reasonable without obtaining incorrect results even in the
		// worst case.
		float maxSize = max( size.x, size.y );
		//float level = max( ceil( log2( maxSize ) - baselog ), baseLevel );
		float level = baseLevel + max( ceil( log2( maxSize ) - baselog ), 0.0 );
		vec4 steps = mix( minExtend.xxyy, maxExtend.xxyy, vOneTwoThird.xyxy );
		vec4 samples, samplesAll;
		vec2 tc;
		
		// test pattern where we have to change only one texture coordinate component at the time:
		// [ 6 7 10 11 ]
		// [ 5 8  9 12 ]
		// [ 4 3 14 13 ]
		// [ 1 2 15 16 ]
		tc = minExtend;
		samples.x = textureLod( occmap, tc, level ).x;
		tc.x = steps.x;
		samples.y = textureLod( occmap, tc, level ).x;
		tc.y = steps.z;
		samples.z = textureLod( occmap, tc, level ).x;
		tc.x = minExtend.x;
		samples.w = textureLod( occmap, tc, level ).x;
		samples.xy = max( samples.xy, samples.zw );
		samplesAll.x = max( samples.x, samples.y );
		
		tc.y = steps.w;
		samples.x = textureLod( occmap, tc, level ).x;
		tc.y = maxExtend.y;
		samples.y = textureLod( occmap, tc, level ).x;
		tc.x = steps.x;
		samples.z = textureLod( occmap, tc, level ).x;
		tc.y = steps.w;
		samples.w = textureLod( occmap, tc, level ).x;
		samples.xy = max( samples.xy, samples.zw );
		samplesAll.y = max( samples.x, samples.y );
		
		tc.x = steps.y;
		samples.x = textureLod( occmap, tc, level ).x;
		tc.y = maxExtend.y;
		samples.y = textureLod( occmap, tc, level ).x;
		tc.x = maxExtend.x;
		samples.z = textureLod( occmap, tc, level ).x;
		tc.y = steps.w;
		samples.w = textureLod( occmap, tc, level ).x;
		samples.xy = max( samples.xy, samples.zw );
		samplesAll.z = max( samples.x, samples.y );
		
		tc.y = steps.z;
		samples.x = textureLod( occmap, tc, level ).x;
		tc.x = steps.y;
		samples.y = textureLod( occmap, tc, level ).x;
		tc.y = minExtend.y;
		samples.z = textureLod( occmap, tc, level ).x;
		tc.x = maxExtend.x;
		samples.w = textureLod( occmap, tc, level ).x;
		samples.xy = max( samples.xy, samples.zw );
		samplesAll.w = max( samples.x, samples.y );
		
		samplesAll.xy = max( samplesAll.xy, samplesAll.zw );
		largestSample = max( samplesAll.x, samplesAll.y );
		
		return minDepth <= largestSample;
	//}
}



const vec2 vScale = vec2( 0.5 );
const vec2 vOffset = vec2( 0.5 );

void main( void ){
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 ); // keep broken compilers happy
	fbResult = 1.0;
	
	#ifdef ENSURE_MIN_SIZE
		//vec3 adjustExtends = step( ( inMaxExtend - inMinExtend ), epsilonSize ) * epsilonSize;
		//vec3 adjustExtends = mix( vec3( 0.0 ), epsilonSize, lessThan( ( inMaxExtend - inMinExtend ), epsilonSize ) );
		vec3 adjustExtends = max( vec3( 0.0 ), epsilonSize + inMinExtend - inMaxExtend ) * vec3( 0.5 );
		vec3 inputMinExtend = inMinExtend - adjustExtends;
		vec3 inputMaxExtend = inMaxExtend + adjustExtends;
	#else
		vec3 inputMinExtend = inMinExtend;
		vec3 inputMaxExtend = inMaxExtend;
	#endif
	
	vec3 testMinExtend;
	vec3 testMaxExtend;
	if( ! calcScreenAABB( testMinExtend, testMaxExtend, pMatrix, inputMinExtend, inputMaxExtend ) ){
		return;
	}
	
	#ifdef FRUSTUM_TEST
		/*
		// NOTE this optimization incorrectly discards a shadow caster causing sun light sometimes
		//      leaking through. mostly seen with hallway and facade meshes. the box seems to fail
		//      the occlusion map test. can be reproduced by looking along a hallway with a wall
		//      behind the camera. the shortened box seems. in this case the z coordinate of the
		//      failing hallway enclosing box is for example [-0.876 .. -0.706] causing the discard.
		//      the camera occlusion test has [-1.054 .. -0.773] for the same box.
		// 
		// TODO verify the calculation and see if the optimization can be made to work correctly
		
		vec3 testCenter = vec3( ( testMinExtend.xy + testMaxExtend.xy ) * vec2( 0.5 ), testMinExtend.z );
		vec4 lambda;
		lambda.x = dot( testCenter, pFrustumNormal1 );
		lambda.y = dot( testCenter, pFrustumNormal2 );
		lambda.z = dot( testCenter, pFrustumNormal3 );
		lambda.w = dot( testCenter, pFrustumNormal4 );
		lambda = lambda * pFrustumTestMul + pFrustumTestAdd;
		lambda.xy = min( lambda.xy, lambda.zw );
		testMaxExtend.z = min( testMinExtend.z + min( lambda.x, lambda.y ), testMaxExtend.z );
		*/
		
		if( ! calcScreenAABB( testMinExtend, testMaxExtend, pMatrix2, testMinExtend, testMaxExtend ) ){
			return;
		}
		
		float occmapMaxDepth;
		vec2 occmapMinExtend = testMinExtend.xy * vScale + vOffset;
		vec2 occmapMaxExtend = vec2( testMaxExtend ) * vScale + vOffset;
		if( ! testBox( occmapMaxDepth, occmapMinExtend, occmapMaxExtend, testMinExtend.z, pScaleSize, pBaseLevel, texOccMap ) ){
			fbResult = 0.0;
			return;
		}
		
	#else
		float occmapMaxDepth;
		vec2 occmapMinExtend = testMinExtend.xy * vScale + vOffset;
		vec2 occmapMaxExtend = vec2( testMaxExtend ) * vScale + vOffset;
		if( ! testBox( occmapMaxDepth, occmapMinExtend, occmapMaxExtend, testMinExtend.z, pScaleSize, pBaseLevel, texOccMap ) ){
			fbResult = 0.0;
			return;
		}
		
		#ifdef DUAL_OCCMAP
			testMaxExtend.z = max( testMaxExtend.z, occmapMaxDepth );
			if( ! calcScreenAABB( testMinExtend, testMaxExtend, pMatrix2, testMinExtend, testMaxExtend ) ){
				return;
			}
			
			occmapMinExtend = testMinExtend.xy * vScale + vOffset;
			occmapMaxExtend = vec2( testMaxExtend ) * vScale + vOffset;
			if( ! testBox( occmapMaxDepth, occmapMinExtend, occmapMaxExtend, testMinExtend.z, pScaleSize2, pBaseLevel2, texOccMap2 ) ){
				fbResult = 0.0;
				return;
			}
		#endif
	#endif
}
