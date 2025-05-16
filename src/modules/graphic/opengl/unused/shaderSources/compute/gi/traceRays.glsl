#ifdef DEBUG_TRACING
const float PI = 3.14159265;
const vec2 TC2PCOORD_MUL = vec2(PI * 2.0 / 256.0, PI / 256.0);
const vec2 TC2PCOORD_ADD = vec2(0, PI * -0.5);
#endif

void main(){
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
}
