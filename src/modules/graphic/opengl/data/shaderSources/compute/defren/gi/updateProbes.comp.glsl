#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/octahedral.glsl"
#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/trace_probe.glsl"
#include "shared/image_buffer.glsl"


layout(binding=0, rgba16f) uniform readonly HIGHP image2D texPosition;
layout(binding=1, rgba8_snorm) uniform readonly HIGHP image2D texNormal;
layout(binding=2, rgba16f) uniform readonly HIGHP image2D texLight;

// MapIrradiance
#ifdef ANDROID
	layout(binding=3, rgba16f) uniform readonly HIGHP image2DArray texProbeIrradiance_load;
	layout(binding=3, rgba16f) uniform writeonly HIGHP image2DArray texProbeIrradiance_store;
#else
	layout(binding=3, rgba16f) uniform HIGHP image2DArray texProbeIrradiance;
	#define texProbeIrradiance_load texProbeIrradiance
	#define texProbeIrradiance_store texProbeIrradiance
#endif

// !MapIrradiance
#ifdef ANDROID
	layout(binding=3, IMG_RG16F_FMT) uniform readonly HIGHP IMG_R16F_2DARR texProbeDistance_load;
	layout(binding=3, IMG_RG16F_FMT) uniform writeonly HIGHP IMG_R16F_2DARR texProbeDistance_store;
#else
	layout(binding=3, IMG_RG16F_FMT) uniform HIGHP IMG_R16F_2DARR texProbeDistance;
	#define texProbeDistance_load texProbeDistance
	#define texProbeDistance_store texProbeDistance
#endif

// RenderDocDebugGI
layout(binding=4, rgba16f) uniform writeonly restrict HIGHP image2D texRenderDocDebug;
void renderDocDebugStore(int index, vec4 value){
	ivec2 tc = ivec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y * uint(12) + uint(index));
	if(all(lessThan(tc, ivec2(2048, 1024)))){
		imageStore(texRenderDocDebug, tc, value);
	}
}


// Irradiance: 8x8, Distance: 16x16
CT_COMPUTE_IN_SIZE_XY


// NOTE findMSB exists since GLSL 4.0
#if __VERSION__ == 330 || __VERSION__ == 150 || __VERSION__ == 140 || __VERSION__ == 130
int findMSB( in int x ){
	int i;
	int mask;
	int res = -1;
	if( x < 0 ){
		x ~= x;
	}
	for( i=0; i<32; i++ ){
		mask = 0x80000000 >> i;
		if( x & mask ){
			res = 31 - i;
			break;
		}
	}
	return res;
}
#endif

ivec3 probeIndexToGridCoord( in int index ){
	return ivec3( index % pGIGridProbeCount.x, index / ( pGIGridProbeCount.x * pGIGridProbeCount.z ),
		( index % ( pGIGridProbeCount.x * pGIGridProbeCount.z ) ) / pGIGridProbeCount.x );
	/*
	return ivec3( index & ( pGIGridProbeCount.x - 1 ),
		index >> findMSB( pGIGridProbeCount.x * pGIGridProbeCount.z ),
		( index & ( ( pGIGridProbeCount.x * pGIGridProbeCount.z ) - 1 ) ) >> findMSB( pGIGridProbeCount.x ) );
	*/
}


//const float epsilon = 1e-9 * float( pGIRaysPerProbe );
const float epsilon = 1e-6;

// for distance map the probe size is 16x16 instead of 8x8 as with irradiance. to allow using
// 8x8 work group size also for distance map 4 work groups are used one operating on each
// quadrant of the full probe. this constant allows for easy tc shifting for each quadrant
//const ivec2 tcQuadrant[4] = ivec2[4]( ivec2( 0 ), ivec2( 8, 0 ), ivec2( 0, 8 ), ivec2( 8 ) );


struct sRayData{
	vec4 position;
	
	vec3 normal;
	bool rayMisses;
	
	vec3 rayDirection;
	bool frontFacing;
	
	vec3 light;
	float rayProbeDistance;
};

const int RayCount = MapIrradiance ? 64 : 256;
shared sRayData vRayData[RayCount]; // 4096 / 16384 bytes


void main( void ){
	// parameters shared across all invocations in the same work group
	int updateIndex = int( gl_WorkGroupID.x );
	
	vec3 probePosition = vec3( pGIProbePosition[ updateIndex ] );
	int probeFlags = int( pGIProbePosition[ updateIndex ].w );
	
	ivec2 rayOffset = ivec2( ( updateIndex % pGIProbesPerLine ) * pGIRaysPerProbe, updateIndex / pGIProbesPerLine );
	
	float blendFactor = ( probeFlags & 1 ) == 1 ? pGIBlendUpdateProbe : 1.0;
	
	int probeIndex = giTraceProbeProbeIndex( updateIndex );
	ivec3 probeGrid = probeIndexToGridCoord( probeIndex );
	
	if(RenderDocDebugGI){
		renderDocDebugStore(0, vec4(updateIndex, rayOffset, blendFactor));
		renderDocDebugStore(1, vec4(probePosition, probeFlags));
		renderDocDebugStore(2, vec4(probeIndex, probeGrid));
	}
	
	// map layout: (probeCount.x * probeCount.y) x pGIGridProbeCount.z
	// 
	// the y work group coordinate is used to select the quadrant. this is used only for
	// distance map which requires 4 work groups to update the entire probe
	// 
	// the actual size is
	// - width = ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2
	// - height = ( pSizeTexDistance + 2 ) * pProbeCount.z + 2
	// 
	// hence each probe is surrounded by a 1-pixel border and the entire grid of probes is
	// surround by another 1-pixel border.
	// 
	// tcProbe is the top-left pixel in the probe excluding the border
	// tcLocal is the coordinate inside the probe excluding the border
	
	int mapProbeSize = MapIrradiance ? pGIIrradianceMapSize : pGIDistanceMapSize;
	
	ivec2 tcProbe = ivec2( pGIGridProbeCount.x * probeGrid.y + probeGrid.x, probeGrid.z ) * ( mapProbeSize + 2 ) + ivec2( 2 );
	ivec2 tcLocal = ivec2( gl_LocalInvocationID ); // + tcQuadrant[ gl_WorkGroupID.y ];
	ivec3 tcSample = ivec3( tcProbe + tcLocal, pGICascade );
	
	
	// the texture coordinates have to be shifted to sample at the center of the
	// pixel. for this reason the 0.5 shift is used together with mapProbeSize.
	vec3 texelDirection = octahedralDecode( ( vec2( tcLocal ) + vec2( 0.5 ) ) * ( 2.0 / float( mapProbeSize ) ) - vec2( 1 ) );
	
	float weight, sumWeight = 0.0;
	int i;
	
	vec3 newProbeState = vec3(0);
	
// 	bool tooCloseToSurface = false;
	float rayBackCount = 0.0;
// 	int rayFrontCount = 0;
// 	int rayMissCount = 0;
	
	// all invocations in the work group have to sample all traced probe rays. to spead this
	// up the reading of the samples is done using cooperative reading. since we use work
	// group size of 8x8 the available invocations running in parallel is 64.
	// 
	// ray counts per probe can be 16, 32, 64, 128 and 256 . Only the last 3 are multiples
	// of 64. this requires limiting the count of threads assisting in the cooperative reading
	
	UFCONST int rayGroupCount = ( pGIRaysPerProbe - 1 ) / RayCount + 1;
	int rg;
	
	for( rg=0; rg<rayGroupCount; rg++ ){
		int rayFirst = RayCount * rg;
		
		// cooperative processing
		int rayIndex = rayFirst + int( gl_LocalInvocationIndex );
		
		if( rayIndex < pGIRaysPerProbe ){
			ivec2 rayTC = rayOffset + ivec2( rayIndex, 0 );
			
			vRayData[ gl_LocalInvocationIndex ].position = imageLoad( texPosition, rayTC );
			vRayData[ gl_LocalInvocationIndex ].normal = vec3( imageLoad( texNormal, rayTC ) );
			vRayData[ gl_LocalInvocationIndex ].rayDirection = pGIRayDirection[ rayIndex ];
			
			vRayData[ gl_LocalInvocationIndex ].rayMisses = vRayData[ gl_LocalInvocationIndex ].position.w > 9999.0;
			
			vRayData[ gl_LocalInvocationIndex ].frontFacing = vRayData[ gl_LocalInvocationIndex ].rayMisses
				|| dot( vRayData[ gl_LocalInvocationIndex ].normal, vRayData[ gl_LocalInvocationIndex ].rayDirection ) < 0.0;
			
			if(MapIrradiance){
				vRayData[gl_LocalInvocationIndex].light = vec3(imageLoad(texLight, rayTC));
			}else{
				vRayData[gl_LocalInvocationIndex].rayProbeDistance = min(
					vRayData[gl_LocalInvocationIndex].position.w, pGIMaxProbeDistance);
			}
		}
		barrier();
		
		
		// per invocation processing
		int rayLimit = min( RayCount, pGIRaysPerProbe - rayFirst );
		
		for( i=0; i<rayLimit; i++ ){
			/*
			bool rayMisses, frontFacing;
			
			if( rayPosition.w > 9999 ){
				// ray misses. rayNormal is not valid in this case
				rayMisses = true;
				frontFacing = true;
// 				rayMissCount++;
				
			}else if( dot( rayNormal, rayDirection ) < 0 ){
				// ray hits front facing geometry
				rayMisses = false;
				frontFacing = true;
// 				rayFrontCount++;
				
			}else{
				// ray hits back facing geometry
				rayMisses = false;
				frontFacing = false;
				rayBackCount += 1.0;
			}
			*/
			
			// optimized version of the above commented out code block. the ray misses check
			// has been moved inside the optimized block to save more time
			if(!MapIrradiance){
				// here we deviate from the paper. ignoring misses to influence the result
				// removes the most glaring light leaks
				if( vRayData[ i ].rayMisses ){
					continue;
				}
			}
			
			rayBackCount += vRayData[ i ].frontFacing ? 0.0 : 1.0;
			// end of optimized block
			
// 			tooCloseToSurface = tooCloseToSurface || rayPosition.w < 0.001;
			
			// for dynamic ray-tracing only the pGIRayDirection[i] (see define) can be used
			//vec3 rayDirection = normalize( rayPosition.xyz - vProbePosition );
			weight = max( dot( texelDirection, vRayData[ i ].rayDirection ), 0.0 );
			if(!MapIrradiance){
				weight = pow( weight, pGIDepthSharpness );
			}
			
			if( weight < epsilon ){
				continue;
			}
			
			/*
			if(!MapIrradiance){
				// here we deviate from the paper. ignoring misses to influence the result
				// removes the most glaring light leaks
				if( vRayData[ i ].rayMisses ){
					continue;
				}
			}
			*/
			
			sumWeight += weight;
			
			if(MapIrradiance){
				if( vRayData[ i ].frontFacing ){
					// ray misses or hits front facing geometry. ray misses are handled
					// the same since sky lighting is applied to missing rays too
					newProbeState += vRayData[ i ].light * weight;
				}
				
			}else{
				// ray misses do not end up here due to the check above
				
				// according to source code distance distance hits and misses should be clamped
				// to a maximum value to not blow out variance. furthermore back face hits
				// should be shortened
				
				if( vRayData[ i ].frontFacing ){
					// if ray misses hit distance is set to 10000. in this case max probe distance
					// has to be used. this works with the min code below so no extra code required
// 					float rayProbeDistance = min( rayPosition.w, pGIMaxProbeDistance ) * ( frontFacing ? 1 : 0.2 );
					
					newProbeState.x += vRayData[ i ].rayProbeDistance * weight;
					newProbeState.y += vRayData[ i ].rayProbeDistance * vRayData[ i ].rayProbeDistance * weight;
				}
			}
		}
	}
	
	
	// determine if the probe is enabled
	bool enableProbe = sumWeight > epsilon;
	
	// this is taken from the paper code update probe offset. there it is used
	// to disable the probe (although I have no clue how this "disable" works).
	// the idea is that if at least 35% of all probe rays hit a backface the
	// probe is likely outside geometry.
	// 
	// in this case here we want to counter the effect of probes outside
	// geometry grazing room corners. in this situation some rays hit back faces
	// of the room while some fly close by the corner and hit nothing. in this
	// situation mean/meanSquared are not 0 and light leaks in.
	// 
	// the 35% rule can though be also applied to only the rays affecting this
	// probe map texel. this is not as optimal as applying the rule to all rays
	// but it helps to remedy out such cases. the rule can not remedy all such
	// cases especially if front faces are visible. but such situations should
	// usually not happen since geometry outside rooms or in walls should not
	// see front faces at all
	//
	// this extra check increases the shader run cost roughly by 50%. for example
	// with 256 rays the cost raises from 80ys to 120ys in average
	// 
	// we count the rays hitting front faces, back faces or miss geometry altogether.
	// rays missing geometry should be counted either to front face hits or back
	// face hits depending where the probe is located. out in the open misses should
	// be counted towards front faces while inside walls they should be counted
	// towards back faces. we do not know though which case is true. we can only
	// guess. so if the count of back faces relative to the total amount of rays
	// is above the 25% threshold then we count it as inside walls
	enableProbe = enableProbe && rayBackCount / float( pGIRaysPerProbe ) < 0.25;
	
	// if probes are too close to the surface various problems happen including view
	// dependent flickering and potential bright flashing on the first update step
	// using blend factor of 1. if at least one ray is too short disable the probe
// 	enableProbe = enableProbe && ! tooCloseToSurface;
	
	
	// finalize the probe
	// blendFactor: 1-hysteresis. modified by update code per-probe
	
	vec3 prevProbeState;
	if(MapIrradiance){
		prevProbeState = vec3(imageLoad(texProbeIrradiance_load, tcSample));
	}else{
		prevProbeState = vec3(IMG_RG16F_LOAD(imageLoad(texProbeDistance_load, tcSample)), 0);
	}
	
	if(RenderDocDebugGI){
		renderDocDebugStore(3, vec4(tcProbe, tcLocal));
		renderDocDebugStore(4, vec4(tcSample, 0));
		renderDocDebugStore(5, vec4(texelDirection, 0));
		renderDocDebugStore(6, vec4(sumWeight, rayBackCount, enableProbe, 0));
		renderDocDebugStore(7, vec4(newProbeState, 0));
		renderDocDebugStore(8, vec4(prevProbeState, 0));
	}
	
	if( enableProbe ){
		sumWeight = 1.0 / max( sumWeight, epsilon );
		
		if(MapIrradiance){
			// according to source code this is required to account for the extra factor
			// of two from summing the cosine weights. I do not see such a factor
// 			sumWeight *= 0.5;
			
			newProbeState *= sumWeight;
			
			// determine blend factor to use. if the difference between the old and new
			// irradiance is large switch to blend factor of 1 otherwise use the regular one
			vec3 lastIrradiance = pow( prevProbeState, vec3( pGIIrradianceGamma ) );
			vec3 diffIrradiance = abs( newProbeState / max( lastIrradiance, vec3( 0.001 ) ) - 1.0 );
			if( max( max( diffIrradiance.x, diffIrradiance.y ), diffIrradiance.z ) > 0.25 ){ // paper probeChangeRatio = 0.25
				blendFactor = min( blendFactor + 0.75, 1.0 ); // blend faster
			}
			
			// in the paper the maximum change is clamped
// 			diffIrradiance = abs( newProbeState - lastIrradiance );
// 			if( length( newProbeState - lastIrradiance ) > 0.1 ){ // probeBrightnessThreshold
// 				newProbeState = mix( lastIrradiance, newProbeState, vec3( 0.25 ) );
// 			}
			
			// not sure what this does to the end result. mentioned in the source code
			newProbeState = pow( newProbeState, vec3( pGIInvIrradianceGamma ) );
			
// 			newProbeState = vec3(0,1,0); // DEBUG
		}else{
			newProbeState *= sumWeight;
		}
		
	}else{
		if(MapIrradiance){
			newProbeState = vec3(0);
// 			newProbeState = vec3(1,0,0); // DEBUG
		}else{
			// by deviating from the paper above we need to handle the case of no hit
			// being scored at all otherwise the probe turns black
			//newProbeState = vec2( pGIMaxProbeDistance, pGIMaxProbeDistance * pGIMaxProbeDistance );
			newProbeState = vec3(0);
		}
		blendFactor = 1.0;
	}
	
// 		blendFactor = 1.0; // DEBUG
// 		blendFactor = pGIBlendUpdateProbe;
	
	// update probe state
	vec4 result = vec4( mix( prevProbeState, newProbeState, blendFactor ), 0 );
	if(MapIrradiance){
		imageStore(texProbeIrradiance_store, tcSample, result);
		
	}else{
		imageStore(texProbeDistance_store, tcSample, IMG_RG16F_STORE(result));
	}
	
	
	// update border pixels by copying probe edge pixels. the original paper does not
	// outline well how this is done. found a schematics somewhere else on the internet
	// and derived an own copy system from it. the basic idea is to flip texture
	// coordinates to get the right ones.
	// 
	// starting with border pixels the matching probe edge pixels are reached like this:
	// 
	// 1) if the pixel U coordinate is on the edge flip the V coordinate.
	// 2) if the pixel V coordinate is on the edge flip the U coordinate.
	// 3) if the pixel U coordinate is on the edge move it 1 pixel away from the edge
	// 4) if the pixel V coordinate is on the edge move it 1 pixel away from the edge
	// 
	// this sequence of actions works for pixels on the edge as well as corner pixels.
	// 
	// to copy probe edge pixels to border pixels this is sort of done in reverse using
	// if conditions running only for suitable pixels.
	// 
	// the copy rules from probe edge pixels to border pixels are then like this:
	// 
	// 1) if U on border => copy to V flipped moving 1 pixel outwards
	// 2) if V on border => copy to U flipped moving 1 pixel outwards
	// 3) if U and V on border => copy to U and V flipped moving 1 pixel outwards
	
	UFCONST ivec2 edgeValues = ivec2( 0, mapProbeSize - 1 );
	bvec4 isOnEdge = equal( tcLocal.xxyy, edgeValues.xyxy );
	bvec2 anyOnEdge = bvec2( any( isOnEdge.xy ), any( isOnEdge.zw ) );
	ivec2 tcShift = ivec2( isOnEdge.x ? -1 : 1, isOnEdge.z ? -1 : 1 );
	ivec2 tcFlipped = tcProbe + edgeValues.yy - tcLocal;
	
	if(RenderDocDebugGI){
		renderDocDebugStore(9, result);
		renderDocDebugStore(10, vec4(isOnEdge));
		renderDocDebugStore(11, vec4(tcShift, tcFlipped));
	}
	
	if(anyOnEdge.x){
		ivec3 tc = ivec3(tcSample.x + tcShift.x, tcFlipped.y, tcSample.z);
		if(MapIrradiance){
			imageStore(texProbeIrradiance_store, tc, result);
			
		}else{
			imageStore(texProbeDistance_store, tc, IMG_RG16F_STORE(result));
		}
	}
	
	if(anyOnEdge.y){
		ivec3 tc = ivec3(tcFlipped.x, tcSample.y + tcShift.y, tcSample.z);
		if(MapIrradiance){
			imageStore(texProbeIrradiance_store, tc, result);
			
		}else{
			imageStore(texProbeDistance_store, tc, IMG_RG16F_STORE(result));
		}
	}
	
	if(all(anyOnEdge)){
		ivec3 tc = ivec3(tcFlipped - tcShift, tcSample.z);
		if(MapIrradiance){
			imageStore(texProbeIrradiance_store, tc, result);
			
		}else{
			imageStore(texProbeDistance_store, tc, IMG_RG16F_STORE(result));
		}
	}
}
