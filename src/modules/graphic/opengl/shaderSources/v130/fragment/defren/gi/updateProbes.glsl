precision highp float;
precision highp int;

#include "v130/shared/octahedral.glsl"
#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"


uniform sampler2D texPosition;
uniform sampler2D texNormal;
uniform sampler2D texLight;
uniform sampler2D texCopyProbe;


flat in ivec2 vRayOffset;
flat in ivec2 vOffset;
flat in float vBlendFactor;
flat in vec3 vProbePosition;

out vec4 outValue;


#ifdef MAP_IRRADIANCE
	#define mapProbeSize pGIIrradianceMapSize
#else
	#define mapProbeSize pGIDistanceMapSize
#endif


//const float epsilon = 1e-9 * float( pGIRaysPerProbe );
const float epsilon = 1e-6;

void main( void ){
	// this fragement shader is run for the entire map including the 1 pixel border around it
	// to avoid running extra shader passes to copy border pixels. what is done here instead
	// is moving the texture coordinate to sample if in the border area. this is not as fast
	// as a copy but better than running extra shader passes.
	// 
	// the original paper does not outline how this is done. found a schematics somewhere
	// else on the internet. so I had to come up with my own way to do it. basic idea is
	// to flip texture coordinates to get the right ones.
	// 
	// 1) if the pixel U coordinate is on the edge flip the V coordinate.
	// 2) if the pixel V coordinate is on the edge flip the U coordinate.
	// 3) if the pixel U coordinate is on the edge move it 1 pixel away from the edge
	// 4) if the pixel V coordinate is on the edge move it 1 pixel away from the edge
	// 
	// this sequence of actions works for pixels on the edge as well as corner pixels.
	// 
	// to use only 2 branches instead of 4 the isOnEdge result for the second block
	// is flipped in the first
	ivec2 tc = ivec2( gl_FragCoord.xy ) - vOffset;
	ivec2 edgeValues = ivec2( 0, mapProbeSize + 1 ); // mapProbeSize + 2 - 1
	bvec4 isOnEdge = equal( tc.xxyy, edgeValues.xyxy );
	
	if( any( isOnEdge.xy ) ){
		tc.y = edgeValues.y - tc.y;
		tc.x += isOnEdge.x ? 1 : -1;
		isOnEdge.zw = isOnEdge.wz;
	}
	if( any( isOnEdge.zw ) ){
		tc.x = edgeValues.y - tc.x;
		tc.y += isOnEdge.z ? 1 : -1;
	}
	
	// from here on updating works as if borders do not exist. keep in mind though
	// the texture coordinate starts at the left-top border. for this reason 1 has
	// to be subtracted to end up at the first map pixel
	tc -= ivec2( 1 ); // move to first map pixel
	
	// the texture coordinates have to be shifted to sample at the center of the
	// pixel. for this reason the 0.5 shift is used together with mapProbeSize.
	vec3 texelDirection = octahedralDecode(
		( vec2( tc ) + vec2( 0.5 ) ) * ( 2.0 / float( mapProbeSize ) ) - vec2( 1.0 ) );
	
	float weight, sumWeight = 0.0;
	int i;
	
	outValue = vec4( 0.0 );
	
// 	bool tooCloseToSurface = false;
	float rayBackCount = 0.0;
// 	int rayFrontCount = 0;
// 	int rayMissCount = 0;
	
	#define rayDirection pGIRayDirection[ i ]
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		ivec2 rayTC = vRayOffset + ivec2( i, 0 );
		vec4 rayPosition = texelFetch( texPosition, rayTC, 0 ); // position, distance
		vec3 rayNormal = texelFetch( texNormal, rayTC, 0 ).xyz;
		
		/*
		bool rayMisses, frontFacing;
		
		if( rayPosition.w > 9999.0 ){
			// ray misses. rayNormal is not valid in this case
			rayMisses = true;
			frontFacing = true;
// 			rayMissCount++;
			
		}else if( dot( rayNormal, rayDirection ) < 0.0 ){
			// ray hits front facing geometry
			rayMisses = false;
			frontFacing = true;
// 			rayFrontCount++;
			
		}else{
			// ray hits back facing geometry
			rayMisses = false;
			frontFacing = false;
			rayBackCount += 1.0;
		}
		*/
		
		// optimized version of the above commented out code block. the ray misses check
		// has been moved inside the optimized block to save more time
		bool rayMisses = rayPosition.w > 9999.0;
		
		#ifdef MAP_DISTANCE
			// here we deviate from the paper. ignoring misses to influence the result
			// removes the most glaring light leaks
			if( rayMisses ){
				continue;
			}
		#endif
		
		bool frontFacing = rayMisses || dot( rayNormal, rayDirection ) < 0.0;
		rayBackCount += frontFacing ? 0.0 : 1.0;
		// end of optimized block
		
// 		tooCloseToSurface = tooCloseToSurface || rayPosition.w < 0.001;
		
		// for dynamic ray-tracing only the pGIRayDirection[i] (see define) can be used
		//vec3 rayDirection = normalize( rayPosition.xyz - vProbePosition );
		weight = max( dot( texelDirection, rayDirection ), 0.0 );
		#ifdef MAP_DISTANCE
			weight = pow( weight, pGIDepthSharpness );
		#endif
		
		if( weight < epsilon ){
			continue;
		}
		
		/*
		#ifdef MAP_DISTANCE
			// here we deviate from the paper. ignoring misses to influence the result
			// removes the most glaring light leaks
			if( rayMisses ){
				continue;
			}
		#endif
		*/
		
		sumWeight += weight;
		
		#ifdef MAP_IRRADIANCE
			if( frontFacing ){
				// ray misses or hits front facing geometry. ray misses are handled
				// the same since sky lighting is applied to missing rays too
				outValue.rgb += texelFetch( texLight, rayTC, 0 ).rgb * weight;
			}
			
		#else
			// ray misses do not end up here due to the check above
			
			// according to source code distance distance hits and misses should be clamped
			// to a maximum value to not blow out variance. furthermore back face hits
			// should be shortened
			
			if( frontFacing ){
				// if ray misses hit distance is set to 10000. in this case max probe distance
				// has to be used. this works with the min code below so no extra code required
// 				float rayProbeDistance = min( rayPosition.w, pGIMaxProbeDistance ) * ( frontFacing ? 1.0 : 0.2 );
				float rayProbeDistance = min( rayPosition.w, pGIMaxProbeDistance );
				
				outValue.x += rayProbeDistance * weight;
				outValue.y += rayProbeDistance * rayProbeDistance * weight;
			}
		#endif
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
	outValue.a = vBlendFactor; // 1-hysteresis. modified by update code per-probe
	
	if( enableProbe ){
		sumWeight = 1.0 / max( sumWeight, epsilon );
		
		#ifdef MAP_IRRADIANCE
			// according to source code this is required to account for the extra factor
			// of two from summing the cosine weights. I do not see such a factor
// 			sumWeight *= 0.5;
			
			outValue.rgb *= sumWeight;
			
			// determine blend factor to use. if the difference between the old and new
			// irradiance is large switch to blend factor of 1 otherwise use the regular one
			vec3 lastIrradiance = pow( texelFetch( texCopyProbe, tc, 0 ).xyz, vec3( pGIIrradianceGamma ) );
			vec3 diffIrradiance = abs( outValue.rgb / max( lastIrradiance, vec3( 0.001 ) ) - 1.0 );
			if( max( max( diffIrradiance.x, diffIrradiance.y ), diffIrradiance.z ) > 0.25 ){ // paper probeChangeRatio = 0.25
				outValue.a = min( outValue.a + 0.75, 1.0 ); // blend faster
			}
			
			// in the paper the maximum change is clamped
// 			diffIrradiance = abs( outValue.rgb - lastIrradiance );
// 			if( length( outValue.rgb - lastIrradiance ) > 0.1 ){ // probeBrightnessThreshold
// 				outValue.rgb = mix( lastIrradiance, outValue.rgb, vec3( 0.25 ) );
// 			}
			
			// not sure what this does to the end result. mentioned in the source code
			outValue.rgb = pow( outValue.rgb, vec3( pGIInvIrradianceGamma ) );
			
// 				outValue.rgb = vec3(0,1,0); // DEBUG
		#else
			outValue.xy *= sumWeight;
		#endif
		
	}else{
		#ifdef MAP_IRRADIANCE
			outValue = vec4( 0.0, 0.0, 0.0, 1.0 );
// 				outValue = vec3(1,0,0,1); // DEBUG
		#else
			// by deviating from the paper above we need to handle the case of no hit
			// being scored at all otherwise the probe turns black
			//outValue.xy = vec2( pGIMaxProbeDistance, pGIMaxProbeDistance * pGIMaxProbeDistance );
			outValue = vec4( 0.0, 0.0, 0.0, 1.0 );
		#endif
	}
	
// 		outValue.a = 1; // DEBUG
// 		outValue.a = pGIBlendUpdateProbe;
}
