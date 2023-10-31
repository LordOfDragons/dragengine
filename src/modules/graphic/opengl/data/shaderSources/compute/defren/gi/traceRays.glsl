precision highp float;
precision highp int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/gi/ubo_gi.glsl"
#include "shared/defren/gi/trace_probe.glsl"

#include "shared/octahedral.glsl"

// if GI_RAYCAST_DISTANCE_ONLY is used for occlusion mesh only material casting is not required
#include "shared/defren/gi/raycast/sample_material.glsl"

#include "shared/defren/gi/raycast/trace_ray.glsl"

#ifdef GI_USE_RAY_CACHE
	#include "shared/defren/gi/raycast/ray_cache_distance.glsl"
	#include "shared/defren/sanitize_light.glsl"
#endif


#ifdef GI_RAYCAST_DISTANCE_ONLY
	layout(binding=0, r16f) uniform writeonly restrict image2D texDistance;
	
#else
	layout(binding=0, rgba16f) uniform writeonly restrict image2D texPosition;
	layout(binding=1, rgba8_snorm) uniform writeonly restrict image2D texNormal;
	layout(binding=2, rgba8) uniform writeonly restrict image2D texDiffuse;
	layout(binding=3, rgba8) uniform writeonly restrict image2D texReflectivity;
	layout(binding=4, rgba16f) uniform writeonly restrict image2D texLight;
#endif


layout( local_size_x=64 ) in;


const float STEP_BACK_DISTANCE = 0.01;


void main( void ){
	ivec2 tc = ivec2( gl_GlobalInvocationID );
	
	int updateIndex = pGIProbesPerLine * tc.y + tc.x / pGIRaysPerProbe;
	if( updateIndex >= pGIProbeCount ){
		return;
	}
	
	int rayIndex = tc.x % pGIRaysPerProbe;
	GIRayCastResult result;
	
	vec3 position = vec3( pGIProbePosition[ updateIndex ] );
	vec3 direction = pGIRayDirection[ rayIndex ];
	
	#ifdef GI_RAYCAST_DISTANCE_ONLY
		if( pGIBVHInstanceRootNode != -1 && giRayCastTraceInstance( pGIBVHInstanceRootNode,
		position + pGIBVHOffset, direction, giRayCastNoHitDistance, result ) ){
			imageStore( texDistance, tc, vec4( result.distance, 0, 0, 0 ) );
			
		}else{
			imageStore( texDistance, tc, vec4( 10000, 0, 0, 0 ) );
		}
		
	#else
		#ifdef GI_USE_RAY_CACHE
			float distLimit = giRayCastCacheDistance( updateIndex, rayIndex, pGICascade );
		#else
			float distLimit = giRayCastNoHitDistance;
		#endif
		
		vec3 resultPosition;
		float resultDistance;
		vec3 resultNormal;
		vec3 resultDiffuse;
		vec3 resultReflectivity;
		float resultRoughness;
		vec3 resultLight;
		
		if( pGIBVHInstanceRootNode != -1 && giRayCastTraceInstance( pGIBVHInstanceRootNode,
		position + pGIBVHOffset, direction, distLimit, result ) ){
			resultPosition = position + direction * result.distance;
			resultPosition += result.normal * STEP_BACK_DISTANCE;
			
			resultDistance = length( resultPosition - position );
			resultNormal = result.normal;
			
			giRayCastMaterialAll( result.material,
				giRayCastFaceTexCoord( result.face, result.barycentric ),
				resultDiffuse, resultReflectivity, resultRoughness, resultLight );
			
		}else{
			#ifdef GI_USE_RAY_CACHE
				return;
			#endif
			
			// we can not store simply the position here since later code calculates the
			// ray direction using this hit point. anything can go here in the end
			resultPosition = position + direction;
			resultDistance = 10000;
			resultNormal = vec3( 0, 0, 1 );
			resultDiffuse = vec3( 1 );
			resultReflectivity = vec3( 0 );
			resultRoughness = 1;
			resultLight = vec3( 0 );
		}
		
		#ifdef GI_USE_RAY_CACHE
		// this one here is REALLY strange. without this sanitize GI lighting randomly
		// obtains NaN values causing total black-out. it makes no sense at all why this
		// sanitize is required here to prevent the blow-up but as long as nobody can
		// explain to me the reason why this sanitize is kept here
		resultLight = sanitizeLight( resultLight );
		#endif
		
		imageStore( texPosition, tc, vec4( resultPosition, resultDistance ) );
		imageStore( texNormal, tc, vec4( resultNormal, 0 ) );
		imageStore( texDiffuse, tc, vec4( resultDiffuse, 0 ) );
		imageStore( texReflectivity, tc, vec4( resultReflectivity, resultRoughness ) );
		imageStore( texLight, tc, vec4( resultLight, 0 ) );
	#endif
}
