precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"

#include "v130/shared/octahedral.glsl"
#include "v130/shared/raycasting.glsl"


out vec4 outPosition;
out vec4 outNormal;
out vec4 outDiffuse;
out vec4 outReflectivity;
out vec3 outLight;


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
	
	RayCastResult result;
	if( rayCastInstance( pBVHInstanceRootNode, vec3( 0.0 ), dir, result ) ){
		outPosition = vec4( dir * result.distance, result.distance );
		outNormal = vec4( result.normal, 0.0 );
	}else{
		outPosition = vec4( 0.0, 0.0, 0.0, 250.0 );
		outNormal vec4( 0.0, 0.0, 1.0, 0.0 );
	}
	outDiffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
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
		
		RayCastResult result;
		if( rayCastInstance( pGIBVHInstanceRootNode, pp, direction, result ) ){
			outPosition = vec4( pp, result.distance );
			outPosition.xyz += direction * max( result.distance - STEP_BACK_DISTANCE, 0.0 );
			outNormal = vec4( result.normal, 0.0 );
			
			vec3 matDiffuse, matReflectivity, matEmissivity;
			float matRoughness;
			rayCastSampleMaterial(result, matDiffuse, matReflectivity, matRoughness, matEmissivity);
			
			outDiffuse = vec4( matDiffuse, 1.0 );
			outReflectivity = vec4( matReflectivity, matRoughness );
			outLight = vec3( matEmissivity );
			
		}else{
			outPosition = vec4( pp, 10000.0 );
			outNormal = vec4( 0.0, 0.0, 1.0, 0.0 );
			outDiffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
			outReflectivity = vec4( 0.0, 0.0, 0.0, 1.0 );
			outLight = vec3( 0.0 );
		}
		return;
	}
	#endif
	
	
	// calculate probe position and index from offset. the quad covers an entire block
	// of probes including not used ones at the end. skip those
	int rowProbeIndex = tc.x / pGIRaysPerProbe;
	int probeIndex = pGIProbesPerLine * tc.y + rowProbeIndex;
	if( probeIndex >= pGIProbeCount ){
		discard;
	}
	
	int firstRayOffset = pGIRaysPerProbe * rowProbeIndex;
	vec3 position = pGIProbePosition[ probeIndex ].xyz;
	
	// trace ray
	vec3 direction = pGIRayDirection[ tc.x - firstRayOffset ];
	RayCastResult result;
	if( pGIBVHInstanceRootNode != -1
	&& rayCastInstance( pGIBVHInstanceRootNode, position, direction, result ) ){
		outPosition.xyz = position + direction * result.distance;
		outPosition.xyz += result.normal * STEP_BACK_DISTANCE;
		
		outPosition.w = length( outPosition.xyz - position );
		outNormal = vec4( result.normal, 0.0 );
		
		vec3 matDiffuse, matReflectivity, matEmissivity;
		float matRoughness;
		rayCastSampleMaterial( result, matDiffuse, matReflectivity, matRoughness, matEmissivity );
		
		outDiffuse = vec4( matDiffuse, 1.0 );
		outReflectivity = vec4( matReflectivity, matRoughness );
		outLight = vec3( matEmissivity );
		
	}else{
		// we can not store simply the position here since later code calculates the
		// ray direction using this hit point. anything can go here in the end
		outPosition = vec4( position + direction, 10000.0 );
		outNormal = vec4( 0.0, 0.0, 1.0, 0.0 );
		outDiffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
		outReflectivity = vec4( 0.0, 0.0, 0.0, 1.0 );
		outLight = vec3( 0.0 );
	}
	
	gl_FragDepth = min( outPosition.w / 1000.0, 1.0 );
}
