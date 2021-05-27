
uniform sampler2D texGIProbeOffset;

vec3 gipoProbeOffset( in ivec3 probeCoord ){
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z );
	return vec3( texelFetch( texGIProbeOffset, texCoord, 0 ) );
}

vec4 gipoProbeOffsetFlags( in ivec3 probeCoord ){
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z );
	return texelFetch( texGIProbeOffset, texCoord, 0 );
}

uint gipoProbeFlags( in ivec3 probeCoord ){
	ivec2 texCoord = ivec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z );
	return uint( texelFetch( texGIProbeOffset, texCoord, 0 ).w );
}

vec3 gipoProbeOffset( in int probeIndex ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	
	ivec3 probeCoord = ivec3( probeIndex % pGIGridProbeCount.x,
		probeIndex / stride, ( probeIndex % stride ) / pGIGridProbeCount.x );
	
	return gipoProbeOffset( probeCoord );
}

vec4 gipoProbeOffsetFlags( in int probeIndex ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	
	ivec3 probeCoord = ivec3( probeIndex % pGIGridProbeCount.x,
		probeIndex / stride, ( probeIndex % stride ) / pGIGridProbeCount.x );
	
	return gipoProbeOffsetFlags( probeCoord );
}

uint gipoProbeFlags( in int probeIndex ){
	int stride = pGIGridProbeCount.x * pGIGridProbeCount.z;
	
	ivec3 probeCoord = ivec3( probeIndex % pGIGridProbeCount.x,
		probeIndex / stride, ( probeIndex % stride ) / pGIGridProbeCount.x );
	
	return gipoProbeFlags( probeCoord );
}
