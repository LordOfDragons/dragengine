precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"


uniform sampler2D texPosition;
uniform sampler2D texNormal;


flat in ivec2 vRayOffset;
flat in vec3 vProbePosition;

out vec3 outOffset;


const float epsilon = 1e-6;


void main( void ){
	float backFaceMinDistToSurface = pGIMoveMinDistToSurface + 0.05;
	int frontFaceCount = 0;
	int backfaceCount = 0;
	int countOffsets = 0;
	int i;
	
	outOffset = vec3( 0.0 );
	
	for( i=0; i<pGIRaysPerProbe; i++ ){
		ivec2 rayTC = vRayOffset + ivec2( i, 0 );
		vec4 rayPosition = texelFetch( texPosition, rayTC, 0 ); // position, distance
		
		if( rayPosition.w >= 9999.0 ){
			continue; // ray misses. do not move
		}
		
		vec3 rayDirection = rayPosition.xyz - vProbePosition;
		vec3 hitNormal = texelFetch( texNormal, rayTC, 0 ).xyz;
		float distToSurface = dot( hitNormal, rayDirection );
		
		if( distToSurface < 0.0 ){
			frontFaceCount++;
			
			if( rayPosition.w >= pGIMoveMinDistToSurface ){
				continue; // far enough to have no influence
				
			}else if( rayPosition.w < 0.001 ){
				// at surface. move along normal
				outOffset += hitNormal * pGIMoveMinDistToSurface;
				
			}else{
				// move back on ray until pGIMoveMinDistToSurface distance
				// offset = rayDirection - normalize(rayDirection) * pGIMoveMinDistToSurface
				// offset = normalize(rayDirection) * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection / rayLength * (rayLength - pGIMoveMinDistToSurface)
				// offset = rayDirection * ((rayLength - pGIMoveMinDistToSurface) / rayLength)
				outOffset += rayDirection * ( 1.0 - pGIMoveMinDistToSurface / rayPosition.w );
			}
			
		}else{
			backfaceCount++;
			
			if( rayPosition.w >= backFaceMinDistToSurface ){
				continue; // too far away to reach other side. do not move
				
			}else{
				outOffset += rayDirection + hitNormal * pGIMoveMinDistToSurface;
			}
			/*
			vec3 offset = rayDirection + hitNormal * pGIMoveMinDistToSurface;
			offset = clamp( offset, -pGIMoveMaxOffset, pGIMoveMaxOffset );
			if( dot( hitNormal, offset ) - distToSurface < 0.05 ){
				continue; // can not reach the other side. do not move
			}
			*/
		}
		
		countOffsets++;
	}
	
	// TODO using back/front face count to disable probes
	
	if( countOffsets > 0 ){
		outOffset /= float( countOffsets );
		
		// averaging the offsets has the tendency to shorten the offset if multiple
		// rays contribute. reduce this effect by enlaring the offset with larger counts
		outOffset *= 1.0 + 0.05 * ( countOffsets - 1 );
	}
	
	outOffset = clamp( outOffset, -pGIMoveMaxOffset, pGIMoveMaxOffset );
}
