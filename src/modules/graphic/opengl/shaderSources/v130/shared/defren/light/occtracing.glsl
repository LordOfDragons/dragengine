#include "v130/shared/octahedral.glsl"

uniform mediump sampler2D texOTOcclusion;
uniform mediump sampler2D texOTDistance;


// dir is required to be normalized
vec2 occtraceTCFromDirection( in vec3 dir, in ivec3 probeCoord, in vec2 mapScale, in int mapSize ){
	vec2 tc = octahedralEncode( dir ); // range [-1..1]
	tc = ( tc + vec2( 1.0 ) ) * 0.5; // range [0..1]
	tc *= vec2( mapSize ); // range [0..mapSize] (left border of left pixel to right border of right pixel)
	tc += vec2( 2 ); // offset by full map border and probe map border
	tc += vec2( pOTProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z ) * vec2( mapSize + 2 );
	return tc * mapScale;
}

// shifted grid coordinates to local grid coordinates. the pOTGridCoordShift value
// contains "probeCount - shift" to reduce the calculation
ivec3 occtraceGridShiftToLocal( in ivec3 shifted ){
	return ( shifted + pOTGridCoordShift ) % pOTProbeCount;
}

// calculate occlusion to apply to fragment
float occtraceOcclusion( in vec3 position, in vec3 normal ){
	vec3 unclampedPosition = vec3( pOTMatrix * vec4( position, 1.0 ) );
	position = clamp( unclampedPosition, vec3( 0.0 ), pOTPositionClamp );
	
	normal = pOTMatrixNormal * normal;
	
	ivec3 baseCoord = clamp( ivec3( pOTProbeSpacingInv * position ), ivec3( 0 ), pOTProbeClamp );
	
	vec3 basePosition = pOTProbeSpacing * vec3( baseCoord );
	float sumOcclusion = 0.0;
	float sumWeight = 0.0;
	
	// distance from floor(currentVertex) position
	vec3 alpha = clamp( pOTProbeSpacingInv * ( position - basePosition ), vec3( 0.0 ), vec3( 1.0 ) );
	
	// iterate over adjacent probe cage
	int i;
	for( i=0; i<8; i++ ){
		// offset = 0 or 1 along each axis
		ivec3 offset = ivec3( i, i >> 1, i >> 2 ) & ivec3( 1 );
		ivec3 probeCoord = clamp( baseCoord + offset, ivec3( 0 ), pOTProbeClamp );
		vec3 probePosition = pOTProbeSpacing * vec3( probeCoord );
		
		probeCoord = occtraceGridShiftToLocal( probeCoord );
		
		vec3 viewVector = probePosition - position;
		vec3 viewDir = normalize( viewVector );
		
		// ws_o = rayOrigin. where is this coming from? we are shading here scene fragments.
		// rayOrigin belongs to probe shading
		vec3 probeToPoint = -viewVector + ( normal /*+ 3.0 * ws_o*/ ) * pOTNormalBias;
		
		vec3 trilinear = mix( vec3( 1.0 ) - alpha, alpha, offset );
		float weight = 1.0;
		
		// smooth backface test
		{
		float value = max( 0.0001, ( dot( viewDir, normal ) + 1.0 ) * 0.5 );
		//weight *= value * value + 0.2;
		
		// deviating from the paper here. the 0.2 addition term creates additional
		// light leaks. removing this term reduces the light leaks in dark rooms
		weight *= value * value;
		}
		
		// moment visibility test
		{
		vec2 texCoord = occtraceTCFromDirection( normalize( probeToPoint ), probeCoord, pOTDistanceMapScale, pOTDistanceMapSize );
		float distToProbe = length( probeToPoint );
		
		vec2 temp = texture( texOTDistance, texCoord ).ra; // RG16 in opengl has RRRG as swizzle
		float mean = temp.x;
		float variance = abs( mean * mean - temp.y );
		
		float chebyshevWeight = max( distToProbe - mean, 0.0 );
		chebyshevWeight = variance / ( variance + chebyshevWeight * chebyshevWeight );
		chebyshevWeight = max( chebyshevWeight * chebyshevWeight * chebyshevWeight, 0.0 );
		
		weight *= distToProbe <= mean ? 1.0 : chebyshevWeight;
		}
		
		// avoid zero weight
		weight = max( 0.000001, weight );
		
		vec2 texCoord = occtraceTCFromDirection( normal, probeCoord, pOTOcclusionMapScale, pOTOcclusionMapSize );
		
		float probeOcclusion = texture( texOTOcclusion, texCoord ).r;
		
		const float crushThreshold = 0.2;
		if( weight < crushThreshold ){
			weight *= weight * weight / ( crushThreshold * crushThreshold );
		}
		
		// trilinear weights
		weight *= trilinear.x * trilinear.y * trilinear.z;
		sumOcclusion += weight * probeOcclusion;
		sumWeight += weight;
	}
	
	// outside the grid of probes the occlusion is not known. the clamping above extends the
	// outer most probe result. the blend factor below fades out into non-occlusion over a
	// short distance.
	// 
	// this can be later on improved by using probe cascades
	float blendBorder = min( length( unclampedPosition - position ) / 2.0, 1.0 );
	
	return mix( ( sumOcclusion / sumWeight ), 1.0, blendBorder ) * pOTEnergyPreservation;
}
