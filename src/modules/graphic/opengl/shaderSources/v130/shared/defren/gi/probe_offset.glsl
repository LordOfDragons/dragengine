
uniform sampler2DArray texGIProbeOffset;

vec3 gipoProbeOffset( in ivec3 probeCoord, in int cascade ){
	ivec3 texCoord = ivec3( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z, cascade );
	return vec3( texelFetch( texGIProbeOffset, texCoord, 0 ) );
}

vec4 gipoProbeOffsetFlags( in ivec3 probeCoord, in int cascade ){
	ivec3 texCoord = ivec3( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z, cascade );
	return texelFetch( texGIProbeOffset, texCoord, 0 );
}

uint gipoProbeFlags( in ivec3 probeCoord, in int cascade ){
	ivec3 texCoord = ivec3( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z, cascade );
	return uint( texelFetch( texGIProbeOffset, texCoord, 0 ).w );
}

vec3 gipoProbeOffset( in int probeIndex, in int cascade ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	
	ivec3 probeCoord = ivec3( probeIndex % pGIGridProbeCount.x,
		probeIndex / stride, ( probeIndex % stride ) / pGIGridProbeCount.x );
	
	return gipoProbeOffset( probeCoord, cascade );
}

vec4 gipoProbeOffsetFlags( in int probeIndex, in int cascade ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	
	ivec3 probeCoord = ivec3( probeIndex % pGIGridProbeCount.x,
		probeIndex / stride, ( probeIndex % stride ) / pGIGridProbeCount.x );
	
	return gipoProbeOffsetFlags( probeCoord, cascade );
}

uint gipoProbeFlags( in int probeIndex, in int cascade ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	
	ivec3 probeCoord = ivec3( probeIndex % pGIGridProbeCount.x,
		probeIndex / stride, ( probeIndex % stride ) / pGIGridProbeCount.x );
	
	return gipoProbeFlags( probeCoord, cascade );
}
