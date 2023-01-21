precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"

uniform HIGHP sampler2DArray texDepth;
#ifdef USE_DEPTH_MIPMAP
uniform HIGHP sampler2DArray texDepthMinMax;
#endif
uniform lowp sampler2DArray texDiffuse;
uniform lowp sampler2DArray texNormal;
uniform lowp sampler2DArray texRoughness;
uniform lowp sampler2DArray texAOSolidity;

in vec2 vTexCoord;
in vec2 vScreenCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec3 outResult;


// Constants
//////////////

const vec4 clipSpaceBorder = vec4( 1.0, -1.0, 1.0, -1.0 );
const vec3 clipThreshold = vec3( 1e-5 );
const vec3 invClipThreshold = vec3( 1e5 );
const vec4 ignoreDistance = vec4( 5.0 ); // anything larger than length(vec3(2,2,2)) = sqrt(12) ~= 3.46
const vec4 distanceBorder = vec4( 0.0 );

#ifdef ROUGHNESS_TAPPING
const vec4 roughnessToAngleBase = vec4( 3.14159265, 3.14159265, -1.5707963, -1.5707963 ); // scaleX, scaleY, offsetX, offsetY
#endif

#include "v130/shared/normal.glsl"
#include "v130/shared/defren/depth_to_position.glsl"


// Calculate the screen space reflection
//////////////////////////////////////////

#if SSR_VERSION == 1

#ifdef MULTI_STEPPING
bool screenSpaceReflectionBisection( in vec4 tcTo, in vec4 tcReflDir, in float dtFactor, inout vec4 tcResult )
#else
void screenSpaceReflectionBisection( in vec4 tcTo, in vec4 tcReflDir, in float dtFactor, inout vec4 tcResult )
#endif
{
	#if 0
	{ // ground truth
	int rayLength = 60;
	float geomZ, dt;
	int i;
	
	tcTo -= tcReflDir;
	tcReflDir /= vec4( rayLength );
	
	for( i=0; i<rayLength; i++ ){
		geomZ = sampleDepth( texDepth, vec3( tcTo.st, vLayer ) );
		dt = dtFactor * tcTo.w * tcTo.w;
		
		#ifdef INVERSE_DEPTH
		if( tcTo.z <= geomZ && geomZ - tcTo.z <= dt )
		#else
		if( tcTo.z >= geomZ && tcTo.z - geomZ <= dt )
		#endif
		{
			tcResult = tcTo;
			#ifdef MULTI_STEPPING
			return true;
			#else
			return;
			#endif
		}
		
		tcTo += tcReflDir;
	}
	#ifdef MULTI_STEPPING
	return false;
	#endif
	}
	#endif
	
	
	
	/*#ifdef USE_DEPTH_MIPMAP
	vec2 geomZ;
	#else*/
	float geomZ;
	//#endif
	float dt;
	int i;
	
	// narrow-phase pass. once a potential block of pixels has been found check the pixels inside using a binary
	// search approach. this allows to test the block with log2(n) samples instead of n samples. the result
	// depends as in the broadphase on the complexity of the depth gradients. in most cases though the gradients
	// are behaving well enough. the post-pass helps to smooth out the potential problems a bit
	tcReflDir *= vec4( 0.5 );
	//roughnessTest1 *= 0.5;
	tcTo -= tcReflDir;
	//roughnessTestX -= roughnessTest1;
	
	/*#ifdef USE_DEPTH_MIPMAP
	float mipMapLod = 2.5; // 5.0 * 0.5
	#endif*/
	
	for( i=0; i<pSSRSubStepCount; i++ ){
		/*#ifdef USE_DEPTH_MIPMAP
			geomZ = textureLod( texDepthMinMax, vec3( tcTo.st * pSSRMinMaxTCScale, vLayer ), mipMapLod ).rg;
		#else*/
			geomZ = sampleDepth( texDepth, vec3( tcTo.st, vLayer ) );
		//#endif
		
		#ifdef INTEGRATED_THRESHOLD_TEST
			dt = dtFactor * tcTo.w * tcTo.w;
		#endif
		
		tcReflDir *= vec4( 0.5 );
		//roughnessTest1 *= 0.5;
		/*#ifdef USE_DEPTH_MIPMAP
		mipMapLod *= 0.5;
		#endif*/
		
		/*#ifdef USE_DEPTH_MIPMAP
			#ifdef INVERSE_DEPTH
			if( tcTo.z <= geomZ.y )
			#else
			if( tcTo.z >= geomZ.x )
			#endif
		#else*/
			#ifdef INVERSE_DEPTH
			if( tcTo.z <= geomZ )
			#else
			if( tcTo.z >= geomZ )
			#endif
		//#endif
		{
			#ifdef INTEGRATED_THRESHOLD_TEST
				#ifdef INVERSE_DEPTH
				if( geomZ - tcTo.z <= dt )
				#else
				if( tcTo.z - geomZ <= dt )
				#endif
				{ // * ( pow( 2.0, roughnessTestX ) )
					tcResult = tcTo; //vec4( tcTo.xyw, depth );
					//roughnessTestFinal = roughnessTestX;
					#ifdef MULTI_STEPPING
					return true;
					#else
					return;
					#endif
				}
			#endif
			
			tcTo -= tcReflDir;
			//roughnessTestX -= roughnessTest1;
			
		}else{
			tcTo += tcReflDir;
			//roughnessTestX += roughnessTest1;
		}
	}
	
	#ifndef INTEGRATED_THRESHOLD_TEST
		#ifdef USE_DEPTH_MIPMAP
			// test point if it is valid. due to the binary search the point might be
			// slightly in front of the geometry instead of behind. we need a little
			// fudge factor. the amount of fudge factor though is a huge problem. too
			// little and it won't count  as hit and too large causes false positives
			// especially a problem with binary search on mip-map depth. to deal with
			// this the neighbor step along the ray is calculated too and used to
			// find the better depth to test
			geomZ = sampleDepth( texDepth, vec3( tcTo.st, vLayer ) );
			dt = dtFactor * tcTo.w * tcTo.w;
			
			#ifdef INVERSE_DEPTH
			if( tcTo.z <= geomZ && geomZ - tcTo.z <= dt )
			#else
			if( tcTo.z >= geomZ && tcTo.z - geomZ <= dt )
			#endif
			{
				tcResult = tcTo;
				#ifdef MULTI_STEPPING
				return true;
				#else
				return;
				#endif
			}
			
			// try the neighbor pixel along the ray
			tcTo += tcReflDir / vec4( length( tcReflDir.xy / pScreenSpacePixelSize ) );
			
			geomZ = sampleDepth( texDepth, vec3( tcTo.st, vLayer ) );
			dt = dtFactor * tcTo.w * tcTo.w;
			
			#ifdef INVERSE_DEPTH
			if( tcTo.z <= geomZ && geomZ - tcTo.z <= dt )
			#else
			if( tcTo.z >= geomZ && tcTo.z - geomZ <= dt )
			#endif
			{
				tcResult = tcTo;
				#ifdef MULTI_STEPPING
				return true;
				#else
				return;
				#endif
			}
			
		#else
			dt = dtFactor * tcTo.w * tcTo.w;
			
			/*#ifdef USE_DEPTH_MIPMAP
				#ifdef INVERSE_DEPTH
				if( abs( geomZ.y - tcTo.z ) <= dt )
				#else
				if( abs( tcTo.z - geomZ.x ) <= dt )
				#endif
			#else*/
				/*#ifdef INVERSE_DEPTH
				if( tcTo.z <= geomZ + dt * 0.05 && geomZ - tcTo.z <= dt )
				#else
				if( tcTo.z >= geomZ - dt * 0.05 && tcTo.z - geomZ <= dt )
				#endif*/
				if( abs( geomZ - tcTo.z ) <= dt )
			//#endif
			{
				tcResult = tcTo;
				#ifdef MULTI_STEPPING
				return true;
				#endif
			}
		#endif
	#endif
	
	#ifdef MULTI_STEPPING
	return false;
	#endif
}
#endif

void screenSpaceReflection( in vec3 position, in vec3 reflectDir, out vec3 result ){
	// determine the reflection direction in screen space. using a scaling of less than the near distance
	// (pSSRClipReflDirNearDist is nearDistance * 0.9) the terminal position can never reach zero or become
	// negative. this prevents the need to check for division by zero or incorrect projection due to a
	// negative z coordinate. this works since in the clip space the reflection vector is stretched to
	// touch the nearest boundary face and for this the initial vector length is irrelevant
	vec4 tcFrom = pMatrixP[ vLayer ] * vec4( position, 1.0 );
	tcFrom = vec4( tcFrom.xyz, 1.0 ) / vec4( tcFrom.w );
	#ifndef INVERSE_DEPTH
	tcFrom.z = tcFrom.z * 0.5 + 0.5;
	#endif
	
	vec4 tcTo = pMatrixP[ vLayer ] * vec4( position + reflectDir * pSSRClipReflDirNearDist, 1.0 );
	tcTo = vec4( tcTo.xyz, 1.0 ) / vec4( tcTo.w );
	#ifndef INVERSE_DEPTH
	tcTo.z = tcTo.z * 0.5 + 0.5;
	#endif
	
	// stretch the reflection vector to touch the nearest clip space boundary face. this ensures the search
	// touches all potential pixels to find the matching result without leaving the screen boundary
	// 
	// basic equation:
	//   hit = from + ray * lambda
	// 
	// for the top border this would be for example:
	//   hit.x = from.x + ray.x * ( 1 - from.y ) / ray.y
	// 
	// we are looking for lambda. for each border this yields these equations:
	//   lambda.top = ( 1 - from.y ) / ray.y
	//   lambda.right = ( 1 - from.x ) / ray.x
	//   lambda.bottom = ( -1 - from.y ) / ray.y
	//   lambda.left = ( -1 - from.x ) / ray.x
	// 
	// we are looking for the smallest lambda which is not negative. one way to solve this in
	// glsl is to use mix() with lessThan() to set negative lambdas to a value larger than any
	// lambda can get through regular calculation.
	// 
	// another problem to take care of is ray.x or ray.y having values close to 0. this would
	// cause infinity values to appear which break shaders.
	// 
	// the calculation can be simplified by flipping x and y problem space if reflection dir
	// is negative in the respective direction
	// 
	// NOTE with inverse depth z range is not -1..1 but 1..0
	vec4 tcReflDir = tcTo - tcFrom;
	float tcReflDirLen = length( tcReflDir.xy );
	
	if( tcReflDirLen < 1e-5 ){
		result = vec3( 0.0 );
		return; // degenerated line case
	}
	
	vec2 testTcFrom = mix( tcFrom.xy, -tcFrom.xy, lessThan( tcReflDir.xy, vec2( 0.0 ) ) );
	vec2 testTcReflDir = abs( tcReflDir.xy );
	
	// test which direction dominates. due to testTcFrom not being in the center the simple test
	// of testTcReflDir.x > testTcReflDir.y is not possible. instead testTcReflDir is compared
	// against the direction of the upper-right corner (1,1) compared to testTcFrom. the full
	// calculation is:
	//   a = normalize( testTcReflDir )
	//   b = normalize( testCornerDir )
	//   if( a.x > b.x ) "x dominates" else "y dominates"
	// 
	// since only one component is important this can be rewritten as:
	//   a.x = testTcReflDir.x / length( testTcReflDir )
	//   b.x = testCornerDir.x / length( testCornerDir )
	vec2 testCornerDir = vec2( 1.0 ) - testTcFrom.xy;
	
	// the correct calculation is:
	//   testTcReflDir.x / tcReflDirLen > testCornerDir.x / length( testCornerDir )
	// 
	// this can result in potential div-by-zero. since length are always positive this can
	// be rewritten into the form below avoiding the div-by-zero problem
	if( testTcReflDir.x * length( testCornerDir ) > testCornerDir.x * tcReflDirLen ){
		tcReflDir *= vec4( ( 1.0 - testTcFrom.x ) / testTcReflDir.x );
		
	}else{
		tcReflDir *= vec4( ( 1.0 - testTcFrom.y ) / testTcReflDir.y );
	}
	
	// apply some scaling factors staying the same for the remainder of the shader
	tcReflDir.xy *= pFSScreenCoordToTexCoord.xy;
	tcFrom.xy = fsquadScreenCoordToTexCoord( tcFrom.st );
	
	// search for the position to sample from.
	// 
	// notes about the depth comprison. a depth is only scored if the depth of the hit candidate fragment is
	// inside a certain threshold compared to the interpolated depth along the reflected ray. due to the
	// interpolation nature the stepSize.z can be used as base for this threshold. due to the nature of the
	// depths stored in the framebuffer though it is possible to miss hits if the location of the reflected
	// ray hitting the framebuffer pixels is not optimal. to counter this problem the threshold is increased
	// a little bit to catch for this situation. the error is neglectable.
	// 
	// a big problem though arises if the reflected ray runs nearly parallel to the camera view plane or even
	// points towards the camera. in this case stepSize.z reaches 0 or goes even neative and the depth test
	// fails. the best solution here is to use the absolute value (to catch the case of negative stepSize.z)
	// and to prevent the threshold to become smaller than a certain lower threshold value (to catch the near
	// 0 case). for this 2 is divided by the smaller of width and height pixels (this is calculate by using
	// (1*scale+offset)-(-1*scale+offset) = 2*scale which is the width and height of the render screen in
	// texels). this is based on looking at the displacement required for a single pixel if the reflected ray
	// is 45 degrees towards the view direction. in this case the same number of steps along the smaller
	// distance in screen space is also the number of steps along the entire z range which is 1. {MOVE TO UNIFORM}.
	tcTo.xy = ceil( abs( tcReflDir.xy ) / pScreenSpacePixelSize );
	float realRayLength = max( max( tcTo.x, tcTo.y ), 2.0 );
	
	tcFrom += tcReflDir / vec4( realRayLength ); // start 1 pixel away from start pixel to not tap yourself
	
	int rayLength = min( int( realRayLength ) - 1, pSSRMaxRayLength );
	tcReflDir *= vec4( float( rayLength ) / realRayLength );
	
	// determine depth threshold. this is a tricky parameter. in general each pixel requires a different
	// depth threshold so only estimates are possible. basic observation is that with reflected rays
	// changing direction a lot compared to the incident ray smaller depth thresholds still work while
	// for reflected rays similar to the incident ray large depth thresholds are required. for this reason
	// the direction of the reflected ray is used to figure out a dpeth threshold
	//float depthThreshold = mix( 0.1, 100.0, pow( abs( reflectDir.z ), 6.0 ) );
	float absReflDirZ = abs( reflectDir.z );
	float depthThreshold = mix( 0.1, 10.0, absReflDirZ );
	
	// close to the grazing angle the situation turns sour. ramp up the threshold A LOT!
	if( absReflDirZ > 0.999 ){
		depthThreshold = mix( 100.0, depthThreshold, ( 1.0 - absReflDirZ ) / 0.001 );
	}
	
	// the depth threshold is set for linear space but we need it for perspective depth. this requires
	// an adjusted depth threshold per tap distance. the calculation is not perfect but close enough
	// to work. the idea is to look at the difference in depth between two z distances depth threshold
	// apart. in math this would be:
	//   d1 = ( z * projMat[2][2] + projMat[3][2] ) / z
	//   d2 = ( ( z + depthThreshold ) * projMat[2][2] + projMat[3][2] ) / ( z + depthThreshold )
	// 
	// this reduces to:
	//   d1 = projMat[2][2] + projMat[3][2] / z
	//   d2 = projMat[2][2] + projMat[3][2] / ( z + depthThreshold )
	//   
	// the difference is:
	//   ( projMat[2][2] + projMat[3][2] / ( z + depthThreshold ) ) - ( projMat[2][2] + projMat[3][2] / z )
	// 
	// or:
	//   projMat[3][2] * ( 1 / ( z + depthThreshold ) - 1 / z )
	// 
	// refomulating:
	//   projMat[3][2] * ( z / ( z * ( z + depthThreshold ) ) - ( z + depthThreshold ) / ( z * ( z + depthThreshold ) ) )
	//   projMat[3][2] * ( z - ( z + depthThreshold ) ) / ( z * ( z + depthThreshold ) )
	//   projMat[3][2] * -depthThreshold / ( z * ( z + depthThreshold ) )
	// 
	// depthThreshold is usually small especially compared to z. it is fine to drop it under the table
	// in the denominator since then we get this:
	//   projMat[3][2] * -depthThreshold / ( z *  z )
	// 
	// good because during interpolation we have 1/w which is 1/z and thus we get:
	//   projMat[3][2] * -depthThreshold * pz * pz
	// 
	// to make this work properly with both inverse and not inverse matrices change to this:
	//   dt = abs( projMat[3][2] ) * depthThreshold * pz * pz
	// 
	// no divs and only muls. sounds good
	float dtFactor = abs( pMatrixP[ vLayer ][3][2] ) * depthThreshold;
	
	// determine the test parameters. the goal is to obtain an upper limit to the number of taps required.
	// currently 20 taps is the maximum. this value can be changed though to balance the quality versus the
	// speed. the taps are distributed over two passes. the first pass is a broad search along the ray to
	// find the first potential block of pixels the result could be found in. this can potentially miss
	// results due to testing the depth at the first and last pixel in the block. this way a potential hit
	// between could be missed. a finer grained search though is usually too costly for moderate cards. at
	// the first hit the second pass is done using a binary search approach to narrow down the search to
	// a single pixel. a hit is not guaranteed but if the result is located in the pixel block it will be
	// found.
	// 
	// since very local reflections are the key use of this shader the step size for the second pass is varied
	// instead of being fixed. the size of the pixel block starts at 1 pixel and increase with every step until
	// it reaches the desired pixel block size. this way smaller steps are taken close to the starting point
	// of the ray to favor a higher quality result where it matters leaving the higher chance to miss a result
	// further along the ray.
	// 
	// now there is the possibility for a bunch of different distributions between the number of taps in the
	// first pass and in the second pass. the key value is the number of taps in the second pass. this defines
	// the size of the pixel blocks used and thus the maximum length of the ray that can be tested. for this
	// a constant array is stored containing the maximum ray length in pixels supported by each final block
	// size configuration. the smallest pixel block size is chosen where the maximum length covers the entire
	// length of the ray. optionally the maximal length of the ray can be clamped to a percentage value of the
	// entire screen size. this is a valid optimization since test rays run all the way until they hit a clip
	// space boundary plane. this is usually at the far end of the view frustum. barely any ray runs all the
	// way back there to score a hit.
	// 
	// module parameter ssrStepCount:
	//    number module parameter. value has to be 1 or larger. default value is 20 (or 30?).
	//    determines the number of broadphase ray steps to take.
	// 
	// module parameter ssrMaxRayLength:
	//    number module parameter. value has to be between 0 and 1. default value is 1. determines the
	//    maximum length of reflection rays measured as percentage of the screen size. a value of 0
	//    is 0% of screen size and 1 is 100% of screen size. reducing the ray length allows to reduce
	//    the number of ray steps. the number of ray steps is considered to apply for the case of 100%
	//    ray length.
	// 
	// uniform parameter pSSRStepCount:
	//    pSSRStepCount = int( ssrStepCount )
	// 
	// uniform parameter pSSRMaxRayLength:
	//    pSSRMaxRayLength = int( float( max( screenWidth, screenHeight ) ) * ssrMaxRayLength )
	// 
	// uniform parameter pSSRSubStepCount:
	//    pSSRSubStepCount = int( floor( log( max( pSSRMaxRayLength / float( pSSRStepCount ), 1.0 ) ) / log( 2.0 ) ) ) + 1
	
#if SSR_VERSION == 0
	// ground GROUND truth
	/*
	vec3 rd = reflectDir*0.1;
	float geomZ, rayZ;
	vec4 p;
	int i;
	tcFrom.w = 2.0; // nothing found
	
	for(i=1; i<1000; i++){
		p = pMatrixP[ vLayer ] * vec4( position + rd*vec3(i), 1.0 );
		p = vec4( p.xyz, 1.0 ) / vec4( p.w );
 		p.st = fsquadScreenCoordToTexCoord( p.st );
		//geomZ = pPosTransform.x / ( pPosTransform.y - sampleDepth( texDepth, vec3( p.st, vLayer ) ) );
		//rayZ = position.z + rd.z * float(i);
		geomZ = sampleDepth( texDepth, vec3( p.st, vLayer ) );
		#ifdef INVERSE_DEPTH
		rayZ = p.z;
		if( rayZ <= geomZ ){ //&& rayZ - geomZ <= depthThreshold ){
		#else
		rayZ = p.z * 0.5 + 0.5;
		if( rayZ >= geomZ ){ //&& rayZ - geomZ <= depthThreshold ){
		#endif
			result = vec3( p.st, 1.0 ); return;
			tcFrom = p;
			break;
		}
	}
	*/
	
	float geomZ, dt;
	int i;
	
	tcReflDir /= vec4( rayLength );
	tcTo = tcFrom;
	tcFrom.w = 2.0; // nothing found
	
	for( i=0; i<rayLength; i++ ){
		geomZ = sampleDepth( texDepth, vec3( tcTo.st, vLayer ) );
		dt = dtFactor * tcTo.w * tcTo.w;
		
		#ifdef INVERSE_DEPTH
		if( tcTo.z <= geomZ && geomZ - tcTo.z <= dt )
		#else
		if( tcTo.z >= geomZ && tcTo.z - geomZ <= dt )
		#endif
		{
			tcFrom = tcTo; //vec4( tcTo.xyw, depth );
			break;
		}
		
		tcTo += tcReflDir;
	}
	
#elif SSR_VERSION == 1
	int stepCount = min( rayLength, pSSRStepCount ); // avoid heavy undersampling
	#ifdef USE_DEPTH_MIPMAP
	vec2 geomZ;
	#else
	float geomZ;
	#endif
	#if ! defined USE_DEPTH_MIPMAP && defined RESULT_AFTER_FIRST_LOOP
	float dt;
	#endif
	int i;
	
	tcReflDir /= vec4( stepCount );
	//float roughnessTest1 = ( 6.0 / 400.0 ) * float( rayLength ) / float( stepCount );
	//float roughnessTestX = 0.0;
	//float roughnessTestFinal = 0.0;
	
	// broad-phase pass. step across the entire length of the reflected ray in chunks of pixels. this is not a
	// correct test as it can miss hits inside the run of pixels. the idea is that if a hit happens inside the
	// run of pixels and the depth gradient is not too complex the z-component of the test ray at the test
	// point is behind the geometry. in this case the block of pixels can be tested in more detail. this allows
	// to scan a larger area of pixels without a huge amount of taps. the smaller the number of pixels in the
	// blocks the better the results. large pixel groups tend to miss hits resulting in punctured reflections.
	// the post-pass takes care of smoothing out the probem a bit
	// 
	// PERFORMANCE NOTE: a loop in a loop is very slow on radeon 4870. if the inner loop is placed outside
	// the outer loop the speed is up to 160 fps but if the inner loop is inside the outer loop the speed
	// drops to 90 fps although the result is identical. the same is true if the inner loop is placed in a
	// function instead of being written fully inside. the hardware doesn't seem to be able to handle this
	// situation efficiently
	
	tcTo = tcFrom;
	tcFrom.w = 2.0; // nothing found
	
	for( i=0; i<stepCount; i++ ){
		tcTo += tcReflDir;
		
		//roughnessTestX += roughnessTest1;
		#ifdef USE_DEPTH_MIPMAP
			#ifndef MULTI_STEPPING
				if( i == 0 ){
					// WARNING! MESA has a bug with continue used inside if-statement skips
					//          loop increment in some situations causing GPU infinite loop.
					//          should a problem happen here change the continue into an if
					//          wrapping the entire loop body with i>0
					continue;
				}
			#endif
			geomZ = textureLod( texDepthMinMax, vec3( tcTo.st * pSSRMinMaxTCScale, vLayer ), 5.0 ).rg;
		#else
			geomZ = sampleDepth( texDepth, vec3( tcTo.st, vLayer ) );
		#endif
		
		#if ! defined USE_DEPTH_MIPMAP && defined RESULT_AFTER_FIRST_LOOP
			dt = dtFactor * tcTo.w * tcTo.w;
		#endif
		
		#ifdef USE_DEPTH_MIPMAP
			#ifdef INVERSE_DEPTH
			if( tcTo.z <= geomZ.y )
			#else
			if( tcTo.z >= geomZ.x )
			#endif
			//if( rayZ >= geomZ.x && rayZ <= geomZ.y + depthThreshold )
			//if( rayZ >= ( geomZ.x + geomZ.y ) * 0.5 ){//&& rayZ <= geomZ.y + depthThreshold )
		#else
			#ifdef INVERSE_DEPTH
			if( tcTo.z <= geomZ )
			#else
			if( tcTo.z >= geomZ )
			#endif
		#endif
		{
			// this test here is important. it is possible the test point itself is the best hit point. if this
			// test is not done the point potentially falls through the narrow-phase test since the z-component
			// can be slightly larger than the depth value. in this case the narrow-phase approaches the point
			// from the other direction and is slightly below the z-component value. this results in black
			// pixels or lines polluting the reflections
			#if ! defined USE_DEPTH_MIPMAP && defined RESULT_AFTER_FIRST_LOOP
				#ifdef INVERSE_DEPTH
				if( geomZ - tcTo.z <= dt )
				#else
				if( tcTo.z - geomZ <= dt )
				#endif
				{ // * ( pow( 2.0, roughnessTestX ) ) ){
					tcFrom = tcTo; //vec4( tcTo.xyw, depth );
					break;
					//roughnessTestFinal = roughnessTestX;
				}
			#endif
			#ifdef NESTED_LOOP
				#ifdef MULTI_STEPPING
					if( screenSpaceReflectionBisection( tcTo, tcReflDir, dtFactor, tcFrom ) ){
						break;
					}
				#else
					screenSpaceReflectionBisection( tcTo, tcReflDir, dtFactor, tcFrom );
					break;
				#endif
			#else
				break;
			#endif
		}
	}
	
	#ifndef NESTED_LOOP
	screenSpaceReflectionBisection( tcTo, tcReflDir, dtFactor, tcFrom );
	#endif
#endif
	
	// in both the broad-phase and the narrow-phase a result can be found. to avoid duplicating the code below
	// in both cases a simple hack is used. the tcFrom is set to all above 1 since it is not used for anything
	// else once we start the testing. if a hit is found the hit point is copied to tcFrom. since we do anyways
	// a test to discard sky reflections (z=1) this can be used to filter out the case of no reflection found
	// (z=2). if a hit is found the coverage calculated. otherwise 0 is used as coverage. in all cases the
	// texture coordinates are written. this allows for better bluring if the sampled color in the non-hit
	// case is crude guess instead of totally off texture coordinates.
	if( tcFrom.w < 1.5 ){//1.0 ){
		// calculate the coverage. for this a couple of possibilities can be used. some of the though do not work
		// very well. they are listed here too just as reference. the result is the multiplication of all used
		// methds. the result is squared to obtain a smoother result.
		// 
		// z difference to from ray to sampled depth:
		//    1 - ( abs(z-depth) / depthThreshold )^2. basically fades out towards pixels close to the threshold.
		//    nice idea in theory but in practice this results in banding artifacts all over the place. the depth
		//    threshold is thus not suitable for coverage calculation.
		// 
		// fading out towards the end of the reflected ray:
		//    1 - (w / rayLength)^4. w is stored in the z component of the result which is the interpolated
		//    distance / rayLength value. this is done by setting the w component of the step variable to 0 and
		//    using 1 as the tcReflDir.w . to be safe clamping is used on the result just in case the value falls
		//    a tiny bit outside the 0-1 range.
		// 
		// screen edge coverage:
		//    x: min( 0.5 / edgeSize - abs( x / pTCClamp.x - 0.5 ) / edgeSize, 1.0 )
		//    y: min( 0.5 / edgeSize - abs( y / pTCClamp.y - 0.5 ) / edgeSize, 1.0 )
		//    fades out towards the edges of the screen inside a range of roughly 10% of the screen size. this is
		//    required since rays towards the edge can not retrieve well results and thus tends to look ugly.
		#if 0
			tcTo = vec4( tcFrom.xy / pScreenSpaceScale - vec2( 0.5 ), tcFrom.z, 1 );
			tcTo.xy = abs( tcTo.xy );
			tcTo.xy = tcTo.xy * pSSRCoverageFactor.xx + pSSRCoverageFactor.yy;
			tcTo.xyz = clamp( tcTo.xyz, vec3( 0.0 ), vec3( 1.0 ) );
			tcTo.xy = pow( tcTo.xy, vec2( pSSRPowEdge ) );
			tcTo.z = pow( tcTo.z, pSSRPowRayLen );
			tcTo.z = 1.0 - tcTo.z;
			
			tcTo.xy = tcTo.xz * tcTo.yw;
			
		#else
			// version without ray length coverage
			tcTo.xy = abs( tcFrom.xy / pScreenSpaceScale - vec2( 0.5 ) );
			tcTo.xy = clamp( tcTo.xy * pSSRCoverageFactor.xx + pSSRCoverageFactor.yy, vec2( 0 ), vec2( 1 ) );
			tcTo.xy = pow( tcTo.xy, vec2( pSSRPowEdge ) );
		#endif
		
		result.xy = tcFrom.xy;
		result.z = tcTo.x * tcTo.y;
		//result.z = roughnessTestFinal;
		
	}else{
		result.xy = tcTo.xy;
		result.z = 0.0;
		//result.z = roughnessTestX;
	}
	
	//color = vec3( float(stepCount) / 1000.0, float(i.z)/255.0, sqrt(coverage) );
	
	//color = vec3( tcTo.xy / pScreenSpacePixelSize, tcTo.z );
	//color = vec3( depth, depthThreshold, 0.0 );
	
	//color = vec3( float(stepCount)/1000.0, float(i.z-1)/255.0, sqrt(coverage) );
	//color = vec3( float( stepCount ) / 255.0, float( subStepCount ) / 255.0, float( rayLength ) / float( stepCount ) / 255.0 );
}



#ifdef ROUGHNESS_TAPPING
float rand( vec2 seed ){
	return fract( sin( dot( seed, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}
#endif

// Main Function
//////////////////

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	// discard not inizalized fragments
	if( texelFetch( texDiffuse, tc, 0 ).a == 0.0 ){
		discard;
	}
	
	outResult = vec3( 0 );
	
	// local reflection is a huge problem for rough surfaces right now. if the depth discontinuity is small using
	// a down-sampled version of the rendered image with the roughness scaled by the distance to the hit point
	// works splendid. as soon though the depth discontinuity is large as with pixels on the edge of objects the
	// entire system breaks down. in this case the scaling used is too large and especially applied on the texture
	// coordinates where it does not help anything. this results in sharp edges around reflections although it
	// should be smooth. looks especially silly since the reflections on the inside of the object are smooth but
	// the objects cut off sharply. so for nothing seems to help the problem. so for the time being local reflections
	// are faded out totally for anything but sharp reflections leaving the result for the global reflections.
	float roughness = texelFetch( texRoughness, tc, 0 ).r;
	#ifdef ROUGHNESS_TAPPING
		if( roughness > 0.5 ){
			return;
		}
	#else
		if( roughness > 0.05 ){
			return;
		}
	#endif
	
	// determine position of fragment
	vec3 position = depthToPosition( texDepth, tc, vScreenCoord, vLayer );
	
	// calculate the reflection parameters
	vec3 normal = normalize( normalLoadMaterial( texNormal, tc ) );
	vec3 reflectDir = reflect( normalize( position ), normal );
	
	// calculate the screen space reflection
	#ifdef ROUGHNESS_TAPPING
		// jitter reflection direction due to roughness. do this a couple of times
		vec4 roughnessToAngle = roughnessToAngleBase * vec4( roughness );
		vec3 tempResult = vec3( 0.0 );
		vec3 temp1, temp2;
		int stepCount;
		vec3 sincos;
		vec2 rval;
		int i;
		
		stepCount = clamp( int( pSSRRoughnessTapCountScale * roughness ), 1, pSSRRoughnessMapTaps );
		
		for( i=0; i<stepCount; i++ ){
			rval.x = rand( gl_FragCoord.xy + vec2( 0.001*float(i), -0.002*float(i) ) );
			rval.y = rand( gl_FragCoord.xy + vec2( 0.002*float(i), -0.001*float(i) ) );
			
			vec2 roughnessAngle = roughnessToAngle.xy * rval + roughnessToAngle.zw;
			
			sincos.x = sin( roughnessAngle.x );
			sincos.y = cos( roughnessAngle.x );
			sincos.z = -sincos.x;
			
			temp1.x = dot( reflectDir.xz, sincos.yz );
			temp1.y = reflectDir.y;
			temp1.z = dot( reflectDir.xz, sincos.xy );
			
			sincos.x = sin( roughnessAngle.y );
			sincos.y = cos( roughnessAngle.y );
			sincos.z = -sin( roughnessAngle.y );
			
			temp2.x = temp1.x;
			temp2.y = dot( temp1.yz, sincos.yx );
			temp2.z = dot( temp1.yz, sincos.zy );
			
			screenSpaceReflection( position, temp2, tempResult );
			outResult += tempResult;
		}
		
		outResult /= vec3( stepCount );
		
	#else
		screenSpaceReflection( position, reflectDir, outResult );
	#endif
	
	// fade out if the roughness gets higher as we can not handle this well for the time being
	#ifdef ROUGHNESS_TAPPING
		outResult.z *= max( 1.0 - ( roughness - 0.3 ) / 0.2, 0.0 ); // (r-0.3) / (0.5-0.3) => 0.3=0 .. 0.5=1
	#else
		outResult.z *= max( 1.0 - roughness / 0.05, 0.0 );
	#endif
}
