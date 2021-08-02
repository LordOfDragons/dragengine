precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/defren/gi/trace_probe.glsl"

#include "v130/shared/octahedral.glsl"

// if GI_RAYCAST_DISTANCE_ONLY is used for occlusion mesh only material casting is not required
#include "v130/shared/defren/gi/raycast/sample_material.glsl"

#include "v130/shared/defren/gi/raycast/trace_ray.glsl"

#ifdef GI_USE_RAY_CACHE
	#include "v130/shared/defren/gi/raycast/ray_cache_distance.glsl"
#endif


#ifdef GI_RAYCAST_DISTANCE_ONLY
	// redirecting name to reuse the same shader definition
	out float outPosition;
	#define outDistance outPosition
	
#else
	out vec4 outPosition;
	out vec3 outNormal;
	out vec3 outDiffuse;
	out vec4 outReflectivity;
	out vec3 outLight;
#endif


const float STEP_BACK_DISTANCE = 0.01;


#ifdef DEBUG_TRACING
const float PI = 3.14159265;
const vec2 TC2PCOORD_MUL = vec2(PI * 2.0 / 256.0, PI / 256.0);
const vec2 TC2PCOORD_ADD = vec2(0, PI * -0.5);
#endif


void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	
	#ifdef DEBUG_TRACING
	{
	vec2 pc = vec2(tc) * TC2PCOORD_MUL + TC2PCOORD_ADD;
	float r = cos(pc.y);
	vec3 dir = vec3(sin(pc.x)*r, sin(pc.y), cos(pc.x)*r);
	
	GIRayCastResult result;
	if( giRayCastTraceInstance( pBVHInstanceRootNode, pGIBVHOffset, dir, giRayCastNoHitDistance, result ) ){
		outPosition = vec4( dir * result.distance, result.distance );
		outNormal = result.normal;
		
	}else{
		#ifdef GI_USE_RAY_CACHE
			discard;
		#endif
		
		outPosition = vec4( 0.0, 0.0, 0.0, 250.0 );
		outNormal vec3( 0.0, 0.0, 1.0 );
	}
	outDiffuse = vec3( 1.0, 1.0, 1.0 );
	outReflectivity = vec4( 0.0, 0.0, 0.0, 1.0 );
	outLight = vec3( 0.0 );
	return;
	}
	#endif
	
	#if GI_RENDERDOC_DEBUG
	{
		const float PI = 3.14159265;
		const vec2 TC2PCOORD_MUL = vec2(PI * 2.0 / 512.0, PI / 256.0);
		const vec2 TC2PCOORD_ADD = vec2(0, PI * -0.5);
		
		vec2 pc = vec2(tc.x % 512, tc.y) * TC2PCOORD_MUL + TC2PCOORD_ADD;
		float r = cos(pc.y);
		vec3 direction = vec3(sin(pc.x)*r, sin(pc.y), cos(pc.x)*r);
		
		//vec3 pp = vec3(0,-0.4*(int(tc.x)/512),0);
		//vec3 pp = vec3(2*(int(tc.x)/512),0,0);
		vec3 pp = vec3(0,0,-1*(tc.x/512));
		
		GIRayCastResult result;
		
		#ifdef GI_RAYCAST_DISTANCE_ONLY
			if( giRayCastTraceInstance( pGIBVHInstanceRootNode, pp + pGIBVHOffset,
			direction, giRayCastNoHitDistance, result ) ){
				outDistance = result.distance;
				
			}else{
				outDistance = 10000.0;
			}
			
		#else
			if( giRayCastTraceInstance( pGIBVHInstanceRootNode,  pp + pGIBVHOffset,
			direction, giRayCastNoHitDistance, result ) ){
				outPosition = vec4( pp, result.distance );
				outPosition.xyz += direction * max( result.distance - STEP_BACK_DISTANCE, 0.0 );
				outNormal = result.normal;
				
				vec3 matDiffuse, matReflectivity, matEmissivity;
				float matRoughness;
				
				giRayCastMaterialAll( result.material, giRayCastFaceTexCoord( result.face, result.barycentric ),
					matDiffuse, matReflectivity, matRoughness, matEmissivity );
				
				outDiffuse = matDiffuse;
				outReflectivity = vec4( matReflectivity, matRoughness );
				outLight = vec3( matEmissivity );
				
			}else{
				#ifdef GI_USE_RAY_CACHE
					discard;
				#endif
				
				outPosition = vec4( pp, 10000.0 );
				outNormal = vec3( 0.0, 0.0, 1.0 );
				outDiffuse = vec3( 1.0, 1.0, 1.0 );
				outReflectivity = vec4( 0.0, 0.0, 0.0, 1.0 );
				outLight = vec3( 0.0 );
			}
		#endif
		return;
	}
	#endif
	
	
	// calculate probe position and index from offset. the quad covers an entire block
	// of probes including not used ones at the end. skip those
	int rowProbeIndex = tc.x / pGIRaysPerProbe;
	int instanceID = pGIProbesPerLine * tc.y + rowProbeIndex;
	if( instanceID >= pGIProbeCount ){
		discard;
	}
	
	int firstRayOffset = pGIRaysPerProbe * rowProbeIndex;
	vec3 position = pGIProbePosition[ instanceID ].xyz;
	
	// trace ray
	int rayIndex = tc.x - firstRayOffset;
	vec3 direction = pGIRayDirection[ rayIndex ];
	GIRayCastResult result;
	
	#ifdef GI_RAYCAST_DISTANCE_ONLY
		if( pGIBVHInstanceRootNode != -1 && giRayCastTraceInstance( pGIBVHInstanceRootNode,
		position + pGIBVHOffset, direction, giRayCastNoHitDistance, result ) ){
			outDistance = result.distance;
			
		}else{
			outDistance = 10000.0;
		}
		
	#else
		#ifdef GI_USE_RAY_CACHE
			float distLimit = giRayCastCacheDistance( instanceID, rayIndex, pGICascade );
		#else
			float distLimit = giRayCastNoHitDistance;
		#endif
		
		if( pGIBVHInstanceRootNode != -1 && giRayCastTraceInstance( pGIBVHInstanceRootNode,
		position + pGIBVHOffset, direction, distLimit, result ) ){
			outPosition.xyz = position + direction * result.distance;
			outPosition.xyz += result.normal * STEP_BACK_DISTANCE;
			
			outPosition.w = length( outPosition.xyz - position );
			outNormal = result.normal;
			
			vec3 matDiffuse, matReflectivity, matEmissivity;
			float matRoughness;
			
			giRayCastMaterialAll( result.material, giRayCastFaceTexCoord( result.face, result.barycentric ),
				matDiffuse, matReflectivity, matRoughness, matEmissivity );
			
			outDiffuse = matDiffuse;
			outReflectivity = vec4( matReflectivity, matRoughness );
			outLight = vec3( matEmissivity );
			
		}else{
			#ifdef GI_USE_RAY_CACHE
				discard;
			#endif
			
			// we can not store simply the position here since later code calculates the
			// ray direction using this hit point. anything can go here in the end
			outPosition = vec4( position + direction, 10000.0 );
			outNormal = vec3( 0.0, 0.0, 1.0 );
			outDiffuse = vec3( 1.0, 1.0, 1.0 );
			outReflectivity = vec4( 0.0, 0.0, 0.0, 1.0 );
			outLight = vec3( 0.0 );
		}
	#endif
	//gl_FragDepth = min( outPosition.w / 1000.0, 1.0 );
}
