precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/occtracing.glsl"
#include "v130/shared/raycasting.glsl"


#ifdef WITH_GI_FIELD
#include "v130/shared/octahedral.glsl"
#include "v130/shared/defren/gi/ubo_field.glsl"

uniform highp sampler2D texGIFieldRays;

uniform mat4x3 pGIFieldMatrixT2F; // tracing space to field space
uniform mat3 pGIFieldMatrixT2FNor; // normal tracing space to field space
uniform mat4x3 pGIFieldMatrixF2T; // field space to tracing space
#endif


out vec4 outPosition;

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
	}else{
		outPosition = vec4( 0.0, 0.0, 0.0, 250.0 );
	}
	return;
	}
	#endif
	
	
	// calculate probe position and index from offset. the quad covers an entire block
	// of probes including not used ones at the end. skip those
	int rowProbeIndex = tc.x / pRaysPerProbe;
	int probeIndex = pProbesPerLine * tc.y + rowProbeIndex;
	if( probeIndex >= pProbeCount ){
		discard;
	}
	
	int firstRayOffset = pRaysPerProbe * rowProbeIndex;
	vec3 position = pProbePosition[ probeIndex ].xyz;
	
	// trace ray
	vec3 direction = pRayDirection[ tc.x - firstRayOffset ];
	RayCastResult result;
	
	#ifdef WITH_GI_FIELD
		vec3 fieldPosition = vec3( pGIFieldMatrixT2F * vec4( position, 1.0 ) );
		vec3 fieldDirection = normalize( direction * pGIFieldMatrixT2FNor ); // reverse order does transpose()
		
		// if position is not inside field intersect ray with field to find closest field position along ray
		if( any( lessThan( fieldPosition, vec3( 0.0 ) ) ) || any( greaterThan( fieldPosition, pGIFieldSize ) ) ){
			float distToField = bvhNodeHit( vec3( 0.0 ), pGIFieldSize, fieldPosition, vec3( 1.0 ) / fieldDirection );
			if( distToField < 0.0 ){
				discard; // ray does not hit field
			}
			fieldPosition += fieldDirection * distToField;
		}
		
		// determine the closest visible probe
		ivec3 baseProbeCoord = clamp( ivec3( fieldPosition / pGIFieldSpacing ), ivec3( 0 ), pGIFieldClamp );
		float bestProbeDistance = 1000.0;
		vec3 bestProbePosition;
		ivec2 bestProbeTC;
		int i;
		
		for( i=0; i<8; i++ ){
			ivec3 offset = ivec3( i, i >> 1, i >> 2 ) & ivec3( 1 );
			ivec3 probeCoord = min( baseProbeCoord + offset, pGIFieldClamp );
			probeIndex = pGIFieldProbeStride * probeCoord.y + pGIFieldResolution.x * probeCoord.z + probeCoord.x;
			ivec2 tcProbe = ivec2( pGIFieldProbeSize ) * ivec2( probeIndex % pGIFieldProbesPerLine, probeIndex / pGIFieldProbesPerLine );
			
			vec3 probePosition = pGIFieldSpacing * vec3( probeCoord );
			vec3 dirVisibility = fieldPosition - probePosition;
			vec2 tcVisibility = ( octahedralEncode( normalize( dirVisibility ) ) + vec2( 1.0 ) ) * vec2( 0.5 * pGIFieldProbeSize );
			float probeDistance = length( dirVisibility );
			
			if( texelFetch( texGIFieldRays, tcProbe + ivec2( tcVisibility ), 0 ).r < probeDistance ){
				continue; // probe is most probably not visible
			}
			
			if( probeDistance >= bestProbeDistance ){
				continue; // another probe is closer
			}
			
			bestProbeTC = tcProbe;
			bestProbePosition = probePosition;
			bestProbeDistance = probeDistance;
		}
		
		if( bestProbeDistance >= 1000.0 ){
			discard; // no probe is potentially visible. this is bad and can only be solved by higher resolution
		}
		
		// sample hit point. this can be done after the loop since we know now which probe won
		ivec2 tcRay = ivec2( ( octahedralEncode( fieldDirection ) + vec2( 1.0 ) ) * vec2( 0.5 * pGIFieldProbeSize ) );
		tcRay = clamp( tcRay, ivec2( 0 ), ivec2( pGIFieldProbeSize - 1 ) );
		
		float fieldHitDistance = texelFetch( texGIFieldRays, bestProbeTC + tcRay, 0 ).r;
		if( fieldHitDistance > 1000.0 ){
			discard;  // no hit
		}
		
		vec3 rayDirection = octahedralDecode( ( vec2( tcRay ) + vec2( 0.5 ) ) * ( 2.0 / float( pGIFieldProbeSize ) ) - vec2( 1.0 ) );
		vec3 hitPoint = bestProbePosition + rayDirection * fieldHitDistance;
		
		// transform back to tracing space
		hitPoint = vec3( pGIFieldMatrixF2T * vec4( hitPoint, 1.0 ) );
		outPosition = vec4( hitPoint, length( hitPoint - position ) );
		
		/*
		// find probe closest to ray. this probe is the best fit we have
		ivec3 probeCoord = ivec3( ( fieldPosition / pGIFieldSpacing ) + 0.5 );
		
		// calculate probe index. this is required to find the look-up position
		probeIndex = pGIFieldProbeStride * probeCoord.y + pGIFieldResolution.x * probeCoord.z + probeCoord.x;
		
		// calculate the offset of the probe in the texture. probes are packed by index
		ivec2 tcProbe = ivec2( pGIFieldProbeSize ) * ivec2( probeIndex % pGIFieldProbesPerLine, probeIndex / pGIFieldProbesPerLine );
		
		// look up the closest texel in the probe map. no blending is used here since we
		// want the closest ray sample. blending them is not really adding more accuracy
		// and only costs sampling time
		ivec2 tcRay = ivec2( ( octahedralEncode( fieldDirection ) + vec2( 1.0 ) ) * vec2( 0.5 * pGIFieldProbeSize ) );
		
		float fieldHitDistance = texelFetch( texGIFieldRays, tcProbe + tcRay, 0 ).r;
		if( fieldHitDistance > 1000.0 ){
			discard;  // no hit
		}
		
		// calculate the hit point. this requires first calculating the probe position and
		// the ray direction used to sample the probe texel
		vec3 rayDirection = octahedralDecode( ( vec2( tcRay ) + vec2( 0.5 ) ) * ( 2.0 / float( pGIFieldProbeSize ) ) - vec2( 1.0 ) );
		vec3 probePosition = pGIFieldSpacing * vec3( probeCoord );
		
		vec3 fieldHitPoint = probePosition + rayDirection * fieldHitDistance;
		
		// transform back to tracing space
		vec3 hitPoint = vec3( pGIFieldMatrixF2T * vec4( fieldHitPoint, 1.0 ) );
		float hitDistance = length( hitPoint - position );
		
		outPosition = vec4( hitPoint, hitDistance );
		//outPosition.xyz = mix( position, hitPoint, max( hitDistance - STEP_BACK_DISTANCE, 0.0 ) / max( hitDistance, 0.001 ) );
		*/
		
	#else
		if( rayCastInstance( pBVHInstanceRootNode, position, direction, result ) ){
			outPosition = vec4( position, result.distance );
			outPosition.xyz += direction * max( result.distance - STEP_BACK_DISTANCE, 0.0 );
			// original DDGI code steps back using "hitNormal * 0.01"
			// // outPosition.xyz += direction * result.distance;
			// // outPosition.xyz += hitNormal * STEP_BACK_DISTANCE;
		}else{
			// we can not store simply the position here since later code calculates the
			// ray direction using this hit point. anything can go here in the end
			outPosition = vec4( position + direction, 10000.0 );
		}
	#endif
	
	gl_FragDepth = min( outPosition.w / 1000.0, 1.0 );
	
	
	#if RENDERDOC_DEBUG
	const float PI = 3.14159265;
	const vec2 TC2PCOORD_MUL = vec2(PI * 2.0 / 64.0, PI / 64.0);
	const vec2 TC2PCOORD_ADD = vec2(0, PI * -0.5);

	vec2 pc = vec2(tc.x % 64, tc.y) * TC2PCOORD_MUL + TC2PCOORD_ADD;
	float r = cos(pc.y);
	direction = vec3(sin(pc.x)*r, sin(pc.y), cos(pc.x)*r);

	//vec3 pp = vec3(0,-0.4*(int(tc.x)/64),0);
	//vec3 pp = vec3(2*(int(tc.x)/64),0,0);
	vec3 pp = vec3(0,0,-1*(tc.x/64));

	if( rayCastInstance( pBVHInstanceRootNode, pp, direction, result ) ){
		outPosition = vec4( pp, result.distance );
		outPosition.xyz += direction * max( result.distance - STEP_BACK_DISTANCE, 0.0 );
	}else{
		outPosition = vec4( pp, 10000.0 );
	}
	
	//if(tc.x/64 != 3) outPosition = vec4(0);
	#endif
}
