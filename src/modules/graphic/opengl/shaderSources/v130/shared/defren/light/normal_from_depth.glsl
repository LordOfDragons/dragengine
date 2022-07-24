// reconstruct normal from depth buffer

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

#ifdef GI_RAY
vec3 normalFromDepth( in ivec2 texcoord, in float centerDepth, in vec3 centerPosition )
#else
vec3 normalFromDepth( in ivec3 texcoord, in float centerDepth, in vec3 centerPosition )
#endif
{
	/*
	the simple solution uses derivatives. this produces wrong results
	along corners and is also not usable in compute shaders
	
	return normalize( cross( dFdy( centerPosition ), dFdx( centerPosition ) ) );
	*/
	
	/*
	an improve version uses 4 additional taps around the center to find a good triangle:
	    4
	 f /|\
	  / | \
	1 --+-- 2
	  \ | /
	   \|/ f(lip)
	    3
	
	each tap is one texel to the side. for each axis the sample with the smaller difference
	to the center value is used. for the chosen triangle the world position is calculated
	and from this the normal. depending on the triangle used the normal has to be flipped
	to obtain the correct one
	*/
	
	#ifdef DECODE_IN_DEPTH
		vec4 samples = vec4( dot( texelFetchOffset( texDepth, texcoord, 0, ivec2( -1, 0 ) ).rgb, unpackDepth ),
			dot( texelFetchOffset( texDepth, texcoord, 0, ivec2( 1, 0 ) ).rgb, unpackDepth ),
			dot( texelFetchOffset( texDepth, texcoord, 0, ivec2( 0, -1 ) ).rgb, unpackDepth ),
			dot( texelFetchOffset( texDepth, texcoord, 0, ivec2( 0, 1 ) ).rgb, unpackDepth ) );
	#else
		vec4 samples = vec4( texelFetchOffset( texDepth, texcoord, 0, ivec2( -1, 0 ) ).r,
			texelFetchOffset( texDepth, texcoord, 0, ivec2( 1, 0 ) ).r,
			texelFetchOffset( texDepth, texcoord, 0, ivec2( 0, -1 ) ).r,
			texelFetchOffset( texDepth, texcoord, 0, ivec2( 0, 1 ) ).r );
	#endif
	
	vec4 difference = abs( samples - vec4( centerDepth ) );
	
	vec2 offsetX, offsetY;
	vec3 positionX, positionY;
	bool flip = false;
	
	if( difference.x < difference.y ){
		offsetX = vec2( -pDepthSampleOffset.x, 0.0 );
		positionX = vec3( samples.x );
		flip = true;
		
	}else{
		offsetX = vec2( pDepthSampleOffset.x, 0.0 );
		positionX = vec3( samples.y );
	}
	
	if( difference.z < difference.w ){
		offsetY = vec2( 0.0, -pDepthSampleOffset.y );
		positionY = vec3( samples.z );
		flip = ! flip;
		
	}else{
		offsetY = vec2( 0.0, pDepthSampleOffset.y );
		positionY = vec3( samples.w );
	}
	
	#ifdef GS_RENDER_STEREO
		positionX.z = pDepthToPosition[ vLayer ].x / ( pDepthToPosition[ vLayer ].y - positionX.z );
		positionY.z = pDepthToPosition[ vLayer ].x / ( pDepthToPosition[ vLayer ].y - positionY.z );
		positionX.xy = ( vScreenCoord + offsetX + pDepthToPosition2[ vLayer ] ) * pDepthToPosition[ vLayer ].zw * positionX.zz;
		positionY.xy = ( vScreenCoord + offsetY + pDepthToPosition2[ vLayer ] ) * pDepthToPosition[ vLayer ].zw * positionY.zz;
	#else
		positionX.z = pDepthToPosition.x / ( pDepthToPosition.y - positionX.z );
		positionY.z = pDepthToPosition.x / ( pDepthToPosition.y - positionY.z );
		positionX.xy = ( vScreenCoord + offsetX + pDepthToPosition2 ) * pDepthToPosition.zw * positionX.zz;
		positionY.xy = ( vScreenCoord + offsetY + pDepthToPosition2 ) * pDepthToPosition.zw * positionY.zz;
	#endif
	
	vec3 normal = normalize( cross( positionY - centerPosition, positionX - centerPosition ) );
	
	return flip ? -normal : normal;
}
