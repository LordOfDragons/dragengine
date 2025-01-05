precision highp float;
precision highp int;

uniform HIGHP sampler2DArray texOccMap;
#ifdef DUAL_OCCMAP
uniform HIGHP sampler2DArray texOccMap2;
#endif

uniform uint pInputDataCount;
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
#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO || defined DUAL_OCCMAP_STEREO || defined FRUSTUM_TEST_STEREO
uniform mat4 pMatrixStereo;
uniform mat4 pMatrix2Stereo;
#endif
uniform uint pCullFilter;


#include "shared/ubo_defines.glsl"

#ifdef WITH_COMPUTE_RENDER_TASK
	#include "shared/defren/plan/world_element.glsl"
	#include "shared/defren/plan/counter.glsl"
	
	UBOLAYOUT_BIND(0) readonly buffer Element {
		sElement pElement[];
	};
	
	UBOLAYOUT_BIND(1) readonly buffer VisibleElement {
		uvec4 pVisibleElement[];
	};
	
	UBOLAYOUT_BIND(2) readonly buffer Counters {
		sCounter pRenderComputeCounter[ pRenderComputeCounterCount ];
	};
	
	UBOLAYOUT_BIND(3) writeonly restrict buffer VisibleElementOut {
		uvec4 pVisibleElementOut[];
	};
	
	// buffer stores in the first uvec4 the output of the node search pass. the output
	// of the element search pass is stored in the next uvec4. for this reason the
	// offset 16 is added so we can reuse the same binding (binding point 3) for input
	// and output through different variables. we store the work group size too so
	// we can dispatch over the results in later passes
	layout( binding=0, offset=0 ) uniform atomic_uint pDispatchWorkGroupCount;
	layout( binding=0, offset=12 ) uniform atomic_uint pNextVisibleIndex;
	
	const uint dispatchWorkGroupSize = uint( 64 );
	
#else
	struct sInputData{
		vec3 minExtend;
		vec3 maxExtend;
	};
	
	UBOLAYOUT_BIND(0) readonly buffer InputData {
		sInputData pInputData[];
	};
	
	UBOLAYOUT_BIND(1) writeonly restrict buffer ResultData {
		bvec4 pResultData[];
	};
#endif

layout( local_size_x=64 ) in;

const float baselog = log2( 3.0 ); // 1.5849625007211563
#ifdef ENSURE_MIN_SIZE
	const vec3 epsilonSize = vec3( 0.005 );
#endif



const bvec3 vBoxCorner[ 8 ] = bvec3[ 8 ](
	bvec3( false, false, false ), // point -x, -y, -z
	bvec3( true,  false, false ), // point  x, -y, -z
	bvec3( false, true,  false ), // point -x,  y, -z
	bvec3( true,  true,  false ), // point  x,  y, -z
	bvec3( false, false, true  ), // point -x, -y,  z
	bvec3( true,  false, true  ), // point  x, -y,  z
	bvec3( false, true,  true  ), // point -x,  y,  z
	bvec3( true,  true,  true  )  // point  x,  y,  z
);

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
in float minDepth, in vec2 scaleSize, in float baseLevel, HIGHP sampler2DArray occmap, in int layer ){
	vec2 size = ( maxExtend - minExtend ) * scaleSize;
	
	//if( min( size.x, size.y ) < 0.01 ){ //0.1 ){
	//	result = 0.0;
	//	largestSample = 1.0;
	//	
	//}else{
		// ensure the minimum size is not below 2 pixels. this avoids a problem with steep
		// geometry causing false invisibility of elements
		vec2 ensureMinSize = vec2( 2.0 ); // 4 for sun and still few problems??
		vec2 adjustSize = max( vec2( 0.0 ), ensureMinSize - size ) * vec2( 0.5 ) / scaleSize;
		minExtend -= adjustSize;
		maxExtend += adjustSize;
		size = max( ensureMinSize, size );
		
		// note about the calculation of the lod level to use. we tap 4x4 pixels to obtain
		// the depth value to compare against. hence we need the lod level where the box
		// covers at most 4 pixels in a row. since the box can though be exactly in the
		// middle over these four pixels the maximal size of the box can not exceed is 3
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
		vec3 tc = vec3( 0, 0, layer );
		
		// test pattern where we have to change only one texture coordinate component at the time:
		// [ 6 7 10 11 ]
		// [ 5 8  9 12 ]
		// [ 4 3 14 13 ]
		// [ 1 2 15 16 ]
		tc.xy = minExtend;
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

bool occlusionTest( in uint index ){
	vec3 inMinExtend, inMaxExtend;
	
	#ifdef WITH_COMPUTE_RENDER_TASK
		if( ( pElement[ index ].flags & pCullFilter ) == uint( 0 ) ){
			return true;
		}
		
		inMinExtend = pElement[ index ].minExtend;
		inMaxExtend = pElement[ index ].maxExtend;
	#else
		inMinExtend = pInputData[ index ].minExtend;
		inMaxExtend = pInputData[ index ].maxExtend;
	#endif
	
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
	#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
		vec3 testMinExtendStereo;
		vec3 testMaxExtendStereo;
	#endif
	
	bool result = calcScreenAABB( testMinExtend, testMaxExtend, pMatrix, inputMinExtend, inputMaxExtend );
	#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
		bool resultStereo = calcScreenAABB( testMinExtendStereo, testMaxExtendStereo, pMatrixStereo, inputMinExtend, inputMaxExtend );
		result = result | resultStereo;
	#endif
	if( ! result ){
		return true;
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
		
		result = calcScreenAABB( testMinExtend, testMaxExtend, pMatrix2, testMinExtend, testMaxExtend );
		if( ! result ){
			return true;
		}
		
		float occmapMaxDepth;
		vec2 occmapMinExtend = testMinExtend.xy * vScale + vOffset;
		vec2 occmapMaxExtend = vec2( testMaxExtend ) * vScale + vOffset;
		
		result = testBox( occmapMaxDepth, occmapMinExtend, occmapMaxExtend, testMinExtend.z, pScaleSize, pBaseLevel, texOccMap, 0 );
		
		#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
			float occmapMaxDepthStereo;
			vec2 occmapMinExtendStereo = testMinExtendStereo.xy * vScale + vOffset;
			vec2 occmapMaxExtendStereo = vec2( testMaxExtendStereo ) * vScale + vOffset;
			result = result | testBox( occmapMaxDepthStereo, occmapMinExtendStereo,
				occmapMaxExtendStereo, testMinExtendStereo.z, pScaleSize, pBaseLevel, texOccMap, 1 );
		#endif
		
		if( ! result ){
			return false;
		}
		
	#else
		float occmapMaxDepth;
		vec2 occmapMinExtend = testMinExtend.xy * vScale + vOffset;
		vec2 occmapMaxExtend = vec2( testMaxExtend ) * vScale + vOffset;
		
		if( result ){
			result = testBox( occmapMaxDepth, occmapMinExtend, occmapMaxExtend,
				testMinExtend.z, pScaleSize, pBaseLevel, texOccMap, 0 );
		}
		
		#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
			float occmapMaxDepthStereo;
			vec2 occmapMinExtendStereo = testMinExtendStereo.xy * vScale + vOffset;
			vec2 occmapMaxExtendStereo = vec2( testMaxExtendStereo ) * vScale + vOffset;
			
			if( resultStereo ){
				resultStereo = testBox( occmapMaxDepthStereo, occmapMinExtendStereo,
					occmapMaxExtendStereo, testMinExtendStereo.z, pScaleSize, pBaseLevel, texOccMap, 1 );
			}
			result = result | resultStereo;
		#endif
		
		if( ! result ){
			return false;
		}
		
		#ifdef DUAL_OCCMAP
			testMaxExtend.z = max( testMaxExtend.z, occmapMaxDepth );
			result = calcScreenAABB( testMinExtend, testMaxExtend, pMatrix2, testMinExtend, testMaxExtend );
			if( ! result ){
				return true;
			}
			
			occmapMinExtend = testMinExtend.xy * vScale + vOffset;
			occmapMaxExtend = vec2( testMaxExtend ) * vScale + vOffset;
			result = testBox( occmapMaxDepth, occmapMinExtend, occmapMaxExtend,
				testMinExtend.z, pScaleSize2, pBaseLevel2, texOccMap2, 0 );
			if( ! result ){
				return false;
			}
		#endif
	#endif
	
	return true;
}

void main( void ){
	uint index = gl_GlobalInvocationID.x;
	
	#ifdef WITH_COMPUTE_RENDER_TASK
		if( index >= pRenderComputeCounter[ erccTempCounter ].counter ){
			return;
		}
		
		uint visibleElement = pVisibleElement[ index / uint( 4 ) ][ index % uint( 4 ) ];
		
		if( ! occlusionTest( visibleElement & uint( 0xffffff ) ) ){
			return;
		}
		
		// add element to found visible elements list
		uint outIndex = atomicCounterIncrement( pNextVisibleIndex );
		
		pVisibleElementOut[ outIndex / uint( 4 ) ][ outIndex % uint( 4 ) ] = visibleElement;
		
		// if the count of visible elements increases by the dispatch workgroup size
		// increment also the work group count. this way the upcoming dispatch
		// indirect calls know the count of workgroups to run
		if( outIndex % dispatchWorkGroupSize == uint( 0 ) ){
			atomicCounterIncrement( pDispatchWorkGroupCount );
		}
		
	#else
		if( index >= pInputDataCount ){
			return;
		}
		
		bool result = occlusionTest( index );
		pResultData[ index / uint(4) ][ index % uint(4) ] = result;
	#endif
}
