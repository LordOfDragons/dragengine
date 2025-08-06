// reconstruct normal from depth buffer

/*
	<!-- shared/defren/light/normal_from_depth.glsl -->
	<define>GI_RAY</define>
*/

vec3 normalFromDepth(in ARG_CONST ivec3 texcoord, in ARG_CONST float centerDepth,
in ARG_CONST vec3 centerPosition){
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
	
	vec4 samples;
	samples.x = texelFetchOffset( texDepth, texcoord, 0, ivec2( -1, 0 ) ).r;
	samples.y = texelFetchOffset( texDepth, texcoord, 0, ivec2( 1, 0 ) ).r;
	samples.z = texelFetchOffset( texDepth, texcoord, 0, ivec2( 0, -1 ) ).r;
	samples.w = texelFetchOffset( texDepth, texcoord, 0, ivec2( 0, 1 ) ).r;
	
	vec4 difference = abs( samples - vec4( centerDepth ) );
	
	vec2 offsetX, offsetY;
	vec3 positionX, positionY;
	bool flip = false;
	
	if( difference.x < difference.y ){
		offsetX = vec2( -pDepthSampleOffset.x, 0 );
		positionX = vec3( samples.x );
		flip = true;
		
	}else{
		offsetX = vec2( pDepthSampleOffset.x, 0 );
		positionX = vec3( samples.y );
	}
	
	if( difference.z < difference.w ){
		offsetY = vec2( 0, -pDepthSampleOffset.y );
		positionY = vec3( samples.z );
		flip = ! flip;
		
	}else{
		offsetY = vec2( 0, pDepthSampleOffset.y );
		positionY = vec3( samples.w );
	}
	
	positionX.z = pDepthToPosition[ vLayer ].x / ( pDepthToPosition[ vLayer ].y - positionX.z );
	positionY.z = pDepthToPosition[ vLayer ].x / ( pDepthToPosition[ vLayer ].y - positionY.z );
	positionX.xy = ( vScreenCoord + offsetX + pDepthToPosition2[ vLayer ] ) * pDepthToPosition[ vLayer ].zw * positionX.zz;
	positionY.xy = ( vScreenCoord + offsetY + pDepthToPosition2[ vLayer ] ) * pDepthToPosition[ vLayer ].zw * positionY.zz;
	
	vec3 normal = normalize( cross( positionY - centerPosition, positionX - centerPosition ) );
	
	return flip ? -normal : normal;
}
