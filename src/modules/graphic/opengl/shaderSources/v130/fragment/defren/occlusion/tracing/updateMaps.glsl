precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/occtracing.glsl"
#include "v130/shared/octahedral.glsl"


uniform sampler2D texRayCast;


flat in ivec2 vRayOffset;
flat in ivec2 vOffset;
flat in vec3 vProbePosition;

out vec4 outValue;


#ifdef MAP_OCCLUSION
	#define mapProbeSize pOcclusionMapSize
#else
	#define mapProbeSize pDistanceMapSize
#endif

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
	vec3 texelDirection = octahedralDecode( ( vec2( tc ) + vec2( 0.5 ) ) * ( 2.0 / float( mapProbeSize ) ) - vec2( 1.0 ) );
	
	float weight, sumWeight = 0.0;
	vec4 rayCast;
	int i;
	
	#ifdef MAP_DISTANCE
	float rayProbeDistance;
	#endif
	
	outValue = vec4( 0.0 );
	
	for( i=0; i<pRaysPerProbe; i++ ){
		rayCast = texelFetch( texRayCast, vRayOffset + ivec2( i, 0 ), 0 ); // position, distance
		
		// if ray misses hit distance is set to 10000. in this case max probe distance
		// has to be used. this works with the min code below so no extra code required
		#ifdef MAP_DISTANCE
		rayProbeDistance = min( rayCast.w, pMaxProbeDistance );
		#endif
		
		weight = max( dot( texelDirection, pRayDirection[ i ] ), 0.0 );
		#ifdef MAP_DISTANCE
		weight = pow( weight, pDepthSharpness );
		#endif
		
		if( weight >= epsilon ){
			#ifdef MAP_OCCLUSION
			if( rayCast.w > 9999.0 ){
				outValue.x += weight; // 1 * weight
			}
			#else
			outValue.x += rayProbeDistance * weight;
			outValue.y += rayProbeDistance * rayProbeDistance * weight;
			#endif
			
			sumWeight += weight;
		}
	}
	
	if( sumWeight > epsilon ){
		#ifdef MAP_OCCLUSION
		outValue.x /= sumWeight;
		#else
		outValue.xy /= sumWeight;
		#endif
		
		outValue.a = 1.0 - pHysteresis;
	}
}
