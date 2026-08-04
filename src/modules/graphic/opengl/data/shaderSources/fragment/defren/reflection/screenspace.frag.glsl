#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

layout(binding=0) uniform HIGHP sampler2DArray texDepth;

// UseDepthMipMap
layout(binding=1) uniform HIGHP sampler2DArray texDepthMinMax;

layout(binding=2) uniform lowp sampler2DArray texDiffuse;
layout(binding=3) uniform lowp sampler2DArray texNormal;
layout(binding=4) uniform lowp sampler2DArray texRoughness;
layout(binding=5) uniform lowp sampler2DArray texAOSolidity;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec3 outResult;


// Constants
//////////////

// RoughnessTapping
const vec4 roughnessToAngleBase = vec4( 3.14159265, 3.14159265, -1.5707963, -1.5707963 ); // scaleX, scaleY, offsetX, offsetY

#include "shared/normal/texture.glsl"
#include "shared/defren/depth_to_position.glsl"


// Calculate the screen space reflection
//////////////////////////////////////////

/** Screen space reflection using linear search for reference only. */
vec4 screenSpaceReflectionLinear(in vec4 rayOrigin, in vec4 rayDir, const in float dtFactor){
	vec2 sizeTexDepth = vec2(textureSize(texDepth, 0));
	int stepCount = int(length(rayDir.xy * sizeTexDepth));
	ivec3 tc = ivec3(0, 0, vLayer);
	for(int i=1; i<stepCount; i++){
		float t = float(i) / float(stepCount);
		vec4 position = rayOrigin + rayDir * t;
		tc.xy = ivec2(position.xy * sizeTexDepth);
		float depth = texelFetch(texDepth, tc, 0).r;
		float diffDepth = InverseDepth ? depth - position.z : position.z - depth;
		float dt = dtFactor * position.w * position.w;
		if(diffDepth >= 0.0 && diffDepth <= dt){
			return vec4(position.xyz, t);
		}
	}
	return vec4(rayOrigin.xyz + rayDir.xyz, 2.1);
}

// SSRVersion == 1

/** Screen space reflection using hierarchical Z buffer. */
vec4 screenSpaceReflectionHiZ(in vec4 rayOrigin, in vec4 rayDir, const in float dtFactor){
	vec2 rayDirDiv = vec2(1.0) / mix(vec2(1e-7) * sign(rayDir.xy), rayDir.xy, greaterThan(abs(rayDir.xy), vec2(1e-7)));
	bvec2 rayDirGT0 = greaterThan(rayDir.xy, vec2(0.0));
	ivec2 sizeTexDepthInt = textureSize(texDepth, 0).xy;
	vec2 sizeTexDepth = vec2(sizeTexDepthInt);
	ivec2 tcOrg = ivec2(rayOrigin.xy * sizeTexDepth);
	vec2 pixelSizeDepth = vec2(1.0) / sizeTexDepth;
	vec2 borderStep = vec2(0.5) * pixelSizeDepth;
	vec2 safeTcOffset = normalize(rayDir.xy) * 0.5;
	ivec3 tc = ivec3(0, 0, vLayer);
	vec4 position = rayOrigin;
	int mipLevel = 0;
	float t = 0.0;
	
	for(int i=0; i<pSSRStepCount; i++){
		if(t >= 1.0){
			return vec4(position.xyz, 2.0);
		}
		
		tc.xy = ivec2(position.xy * sizeTexDepth + safeTcOffset) >> mipLevel;
		
		float minDepth = texelFetch(texDepth, tc, mipLevel).r;
		
		vec2 pixelSize = vec2(1 << mipLevel) * pixelSizeDepth;
		vec2 tcBoundary = vec2(tc) * pixelSize + mix(vec2(0.0), pixelSize, rayDirGT0);
		vec2 tToBoundary = (tcBoundary - rayOrigin.xy) * rayDirDiv;
		float tNext = min(min(tToBoundary.x, tToBoundary.y), 1.0);
		
		vec4 positionNext = rayOrigin + rayDir * tNext;
		
		float diffDepth = InverseDepth ? minDepth - positionNext.z : positionNext.z - minDepth;
		
		if(any(greaterThanEqual(tc.xy, sizeTexDepthInt >> mipLevel))){
			if(mipLevel == 0){
				return vec4(position.xyz, 2.2);
			}
			diffDepth = 0.0;
		}
		
		if(diffDepth >= 0.0){
			if(mipLevel == 0){
				float dt = dtFactor * positionNext.w * positionNext.w;
				if(diffDepth <= dt){
					return vec4(position.xyz, t);
				}
			}else{
				mipLevel--;
				continue;
			}
		}
		
		t = tNext;
		position = positionNext;
		mipLevel = min(mipLevel + 1, int(pMipMapMaxLevel));
	}
	return vec4(rayOrigin.xyz + rayDir.xyz, 2.1);
}


void screenSpaceReflection(const in vec3 position, const in vec3 reflectDir, out vec3 result){
	vec4 tcFrom = pMatrixP[vLayer] * vec4(position, 1.0);
	tcFrom = vec4(tcFrom.xyz, 1.0) / vec4(tcFrom.w);
	if(!InverseDepth){
		tcFrom.z = tcFrom.z * 0.5 + 0.5;
	}
	vec4 tcTo = pMatrixP[vLayer] * vec4(position + reflectDir * pSSRClipReflDirNearDist, 1.0);
	tcTo = vec4(tcTo.xyz, 1.0) / vec4(tcTo.w);
	if(!InverseDepth){
		tcTo.z = tcTo.z * 0.5 + 0.5;
	}
	
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
	
	tcReflDir *= vec4(float(int(realRayLength) - 1) / realRayLength);
	
	// determine depth threshold. this is a tricky parameter. in general each pixel requires a different
	// depth threshold so only estimates are possible. basic observation is that with reflected rays
	// changing direction a lot compared to the incident ray smaller depth thresholds still work while
	// for reflected rays similar to the incident ray large depth thresholds are required. for this reason
	// the direction of the reflected ray is used to figure out a depth threshold
	//float depthThreshold = mix( 0.1, 100.0, pow( abs( reflectDir.z ), 6.0 ) );
	float absReflDirZ = abs(reflectDir.z);
	
	// improved depth threshold calculation using view-dependent thickness.
	// base threshold scales with view angle: steeper angles need larger thresholds
	//float baseThreshold = mix(0.05, 5.0, absReflDirZ);
	float baseThreshold = mix(0.01, 1000.0, pow(absReflDirZ, 2.0));
	
	// for grazing angles use a more gradual ramp to avoid artifacts
	/*if(absReflDirZ > 0.99){
		baseThreshold = mix(50.0, baseThreshold, (1.0 - absReflDirZ) / 0.01);
	}*/
	/*
	// add depth thickness awareness using depth texture mip maps.
	// sample depth at ray origin and at next mip level to estimate local surface thickness
	float depthBase = textureLod(texDepth, vec3(tcFrom.st, vLayer), 0.0).r;
	float depthMip1 = textureLod(texDepth, vec3(tcFrom.st, vLayer), 1.0).r;
	float localThickness = abs(depthMip1 - depthBase);
	
	// scale threshold by local thickness to handle thin geometry better
	float thicknessScale = 1.0 + localThickness * 10.0;
	float depthThreshold = baseThreshold * thicknessScale;
	*/
	float depthThreshold = baseThreshold * 0.25;
	
	// clamp to reasonable range
	depthThreshold = clamp(depthThreshold, 0.01, 100.0);
	
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
	bool condition;
	
	if(SSRVersion == 0){
		vec4 hitResult = screenSpaceReflectionLinear(tcFrom, tcReflDir, dtFactor);
		if(hitResult.w < 1.5){
			tcFrom = hitResult;
		}
		
	}else if(SSRVersion == 1){
		vec4 hitResult = screenSpaceReflectionHiZ(tcFrom, tcReflDir, dtFactor);
		if(hitResult.w < 1.5){
			tcFrom = hitResult;
		}
	}
	
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



// RoughnessTapping
float rand( vec2 seed ){
	return fract( sin( dot( seed, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}


// Main Function
//////////////////

void main( void ){
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	// discard not initialized fragments
	if( texelFetch( texDiffuse, tc, 0 ).a == 0.0 ){
		discard;
	}
	
	outResult = vec3( 0 );
	
	// determine position of fragment
	vec3 position = depthToPosition( texDepth, tc, vScreenCoord, vLayer );
	
	// calculate the reflection parameters
	vec3 normal = sanitizeNormal( normalLoadMaterial( texNormal, tc ) );
	vec3 reflectDir = reflect(normalize(position), normal);
	
	// skip SSR for rough surfaces
	float roughness = texelFetch(texRoughness, tc, 0).r;
	if(roughness > (RoughnessTapping ? 0.5 : 0.2)){
		return;
	}
	
	// calculate screen-space reflection
	if(RoughnessTapping){
		// jitter reflection direction due to roughness for stochastic sampling
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
		
		outResult.z *= clamp(1.0 - (roughness - 0.3) / 0.2, 0.0, 1.0); // (r-0.3) / (0.5-0.3) => 0.3=0 .. 0.5=1
		
	}else{
		screenSpaceReflection( position, reflectDir, outResult );
		outResult.z *= clamp(1.0 - (roughness - 0.1) / 0.1, 0.0, 1.0);
	}
}
