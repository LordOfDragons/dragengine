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
	// this fragement shader is run for the entire map including the 1 pixel border around it.
	// by clamping the texture coordinates the border is updated like the pixels around the
	// map border. this avoids creating complicated copy shaders.
	ivec2 tc = clamp( ivec2( gl_FragCoord.xy ) - vOffset, ivec2( 0 ), ivec2( vOffset ) );
	vec3 texelDirection = octahedralDecode( ( vec2( tc ) + vec2( 0.5 ) )
		* ( 2.0 / float( mapProbeSize ) ) - vec2( 1.0 ) );
	
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
