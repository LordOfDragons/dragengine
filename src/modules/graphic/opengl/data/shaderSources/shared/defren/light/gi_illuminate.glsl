// dir is required to be normalized
vec2 giTCFromDirection( in vec3 dir, in ivec3 probeCoord, in vec2 mapScale, in int mapSize ){
	vec2 tc = octahedralEncode( dir ); // range [-1..1]
	tc = ( tc + vec2( 1.0 ) ) * 0.5; // range [0..1]
	tc *= vec2( mapSize ); // range [0..mapSize] (left border of left pixel to right border of right pixel)
	tc += vec2( 2 ); // offset by full map border and probe map border
	tc += vec2( pGIParams[0].probeCount.x * probeCoord.y + probeCoord.x, probeCoord.z ) * vec2( mapSize + 2 );
	return tc * mapScale;
}

// shifted grid coordinates to local grid coordinates. the pGIGridCoordShift value
// contains "probeCount - shift" to reduce the calculation
ivec3 giGridShiftToLocal( in ivec3 shifted, in int cascade ){
	return ( shifted + pGIParams[cascade].gridCoordShift ) % pGIParams[0].probeCount;
}

// calculate illumination to apply to fragment. returns summed irradiance as RGB and summed weight as A
vec4 giIlluminate( in vec3 position, in vec3 normal, in vec3 bendNormal, in int cascade ){
	// in the paper this calculation looks like this:
	//   BiasVector = (n * 0.2 + wo * 0.8 ) * ( 0.75 * minDistanceBetweenProbes ) * TurnableShadowBias
	// whereas n is the normal and wo the direction from fragment to camera.
	// 
	// we need the offset in GI space so the pGIMatrixNormal is used to transform it. since
	// position is the vector from camera to fragment w0 is the negation of the normalized
	// position. furthermore the GI state sets pGISelfShadowBias is set to 0.75 * selfShadowBias.
	vec3 offsetPosition = mix( bendNormal, -normalize( position ),
		vec3( pGIParams[cascade].normalBias ) ) * pGIParams[cascade].selfShadowBias;
	offsetPosition = offsetPosition * pGIParams[cascade].matrixNormal; // reverse order does transpose()
	
	normal = normalize( normal * pGIParams[cascade].matrixNormal ); // reverse order does transpose()
	bendNormal = normalize( bendNormal * pGIParams[cascade].matrixNormal ); // reverse order does transpose()
	position = vec3( pGIParams[cascade].matrix * vec4( position, 1.0 ) );
	
	// outside the grid of probes the irradiance is not known. the clamping above extends the
	// outer most probe result. the blend factor below fades out into non-irradiated over a
	// short distance. if 0 avoid calculating the cascade at all
// 	float cascadeBlend = 1.0 - min( length( position - clamp( position, vec3( 0.0 ),
// 		pGIParams[cascade].positionClamp ) ) / 2.0, 1.0 );
	vec3 gridCoord = ( position + offsetPosition ) * pGIParams[cascade].probeSpacingInv;
	
	float cascadeBlend = 1.0;
	
	if( cascade < pGIHighestCascade ){
		vec3 borderBlend = clamp( gridCoord, vec3( 0.0 ), vec3( 1.0 ) )
			* clamp( vec3( pGIParams[cascade].probeClamp ) - gridCoord, vec3( 0.0 ), vec3( 1.0 ) );
		cascadeBlend = borderBlend.x * borderBlend.y * borderBlend.z;
		
		if( cascadeBlend <= 0.0 ){
			return vec4( 0.0 );
		}
	}
	
	// cascade does contribute
	ivec3 baseCoord = clamp( ivec3( gridCoord ), ivec3( 0 ), pGIParams[cascade].probeClamp );
	vec3 basePosition = pGIParams[cascade].probeSpacing * vec3( baseCoord );
	
	vec3 sumIrradiance = vec3( 0.0 );
	float sumWeight = 0.0;
	
	vec3 alpha = clamp( pGIParams[cascade].probeSpacingInv
		* ( position + offsetPosition - basePosition ), vec3( 0.0 ), vec3( 1.0 ) ); // paper
	
	// iterate over adjacent probe cage
	int i;
	for( i=0; i<8; i++ ){
		// offset = 0 or 1 along each axis
		ivec3 offset = ivec3( i, i >> 1, i >> 2 ) & ivec3( 1 );
		ivec3 probeCoord = clamp( baseCoord + offset, ivec3( 0 ), pGIParams[cascade].probeClamp );
		vec3 probePosition = pGIParams[cascade].probeSpacing * vec3( probeCoord );
		
		probeCoord = giGridShiftToLocal( probeCoord, cascade );
		
		// ignore disabled probes. probes are disabled if >25% of cached rays hit backfaces
		vec4 offsetFlags = gipoProbeOffsetFlags( probeCoord, cascade );
		if( ( uint( offsetFlags.w ) & gipfDisabled ) == gipfDisabled ){
			continue;
		}
		
		probePosition += offsetFlags.xyz;
		float weight = 1.0;
		
		// get visibility
		vec3 probeToPoint = position + offsetPosition - probePosition;
		float distToProbe = length( probeToPoint );
		
		vec3 texCoord = vec3( giTCFromDirection( distToProbe > 0.001 ? probeToPoint / distToProbe : vec3( 0, 0, 1 ),
			probeCoord, pGIParams[cascade].distanceMapScale, pGIParams[cascade].distanceMapSize ), cascade );
		
		vec2 visibility = texture( texGIDistance, texCoord ).ra; // RG16 in opengl has RRRG as swizzle
		
		// ignore disabled probes. this is different than the disabled flag above.
		// the disabled flag is set due to more than 25% of cached rays hitting backfaces.
		// zero visibility is set if more than 25% of all rays hit backfaces. this covers
		// the situation of probes being dynamically disabled because dynamic geometry
		// swallows them. once the dynamic geometry moves away the probe is enabled again
		// 
		// NOTE by using compute shaders this could be handled by using another disabled
		//      flag and both can be checked above without needing to tap the visibility.
		//      without using compute shaders though this is a bit complicated so the
		//      detour over the visibility is used
		if( visibility.x == 0.0 ){
			continue;
		}
		
		// smooth backface test
		vec3 viewDir = normalize( probePosition - position );
		
		float backfaceValue = dot( viewDir, normal ) * 0.5 + 0.5;
		weight *= backfaceValue * backfaceValue + 0.2;
		
		// visibility test
		float meanDist = visibility.x;
		float variance = abs( meanDist * meanDist - visibility.y );
		
		float chebyshevWeight = 1.0;
		if( distToProbe > meanDist ){
			// in shadow case according to paper
			chebyshevWeight = distToProbe - meanDist;
			chebyshevWeight = variance / ( variance + chebyshevWeight * chebyshevWeight );
			chebyshevWeight = max( chebyshevWeight * chebyshevWeight * chebyshevWeight, 0.05 );
				// ^== the 0.05 threshold avoids all probes going to 0 having no influence
		}
		
		weight *= chebyshevWeight;
		
		// avoid zero weight
		weight = max( 0.000001, weight );
		
		// crush small intensities
		const float crushThreshold = 0.2;
		if( weight < crushThreshold ){
			weight *= weight * weight / ( crushThreshold * crushThreshold );
		}
		
		// trilinear weights
		vec3 trilinear = max( mix( vec3( 1.0 ) - alpha, alpha, offset ), 0.001 );
		weight *= trilinear.x * trilinear.y * trilinear.z;
		
		// sample irradiance
		texCoord.xy = giTCFromDirection( bendNormal, probeCoord,
			pGIParams[cascade].irradianceMapScale, pGIParams[cascade].irradianceMapSize );
		vec3 probeIrradiance = texture( texGIIrradiance, texCoord ).rgb;
		
		// from source code. using some kind of gamma=2 curve (basically an sqrt) to
		// approximate sRGB blending. should help somehow with trilinear?
		probeIrradiance = pow( probeIrradiance, vec3( pGIParams[cascade].irradianceGamma * 0.5 ) );
		
		sumIrradiance += probeIrradiance * weight;
		sumWeight += weight;
	}
	
// 	return vec4( sumIrradiance, sumWeight ) * vec4( cascadeBlend );
	
	// normalize
	if( sumWeight == 0.0 ){
		return vec4( 0.0 );
	}
	
	sumIrradiance /= sumWeight;
	
	// final result
// 	cascadeBlend *= min( sumWeight / 0.1, 1.0 );
		// ^== the idea here had been to allow the next higher cascade to fill in the blanks
		//     if this cascade has no good probe because they are covered up or in geometry.
		//     using sumWeight itself produces visible artifacts without helping much.
		//     the linear step version does not show the artifacts but does not produce
		//     any measurable improvements. this does not really help
	
	return vec4( sumIrradiance, cascadeBlend );
}

vec3 giIlluminate( in vec3 position, in vec3 normal, in vec3 bendNormal ){
	vec4 sumIrradiance = vec4( 0.0 );
	int i;
	
	for( i=0; i<=pGIHighestCascade; i++ ){
		vec4 cascadeIlluminance = giIlluminate( position, normal, bendNormal, i );
		sumIrradiance += cascadeIlluminance * vec4( vec3( cascadeIlluminance.a ), 1.0 );
		
		// if enough coverage has been accumulated we can stop
		if( sumIrradiance.a >= 1.0 ){
			break;
		}
	}
	
	// normalize
	if( sumIrradiance.a == 0.0 ){
		return vec3( 0.0 );
	}
	
	sumIrradiance.rgb /= sumIrradiance.a;
	
	// from source code. convert back to linear irradiance (aka square root it)
	sumIrradiance.rgb *= sumIrradiance.rgb;
	
	// from source code: "was factored out of probes". no idea what this means.
	// if I leave this uncommented the irradiance in the scene keeps on increading
	// until all blows out
// 	const float pi = 3.1415926538;
// 	sumIrradiance.rgb *= 2.0 * pi;
	
	// final result
	return sumIrradiance.rgb;
}
