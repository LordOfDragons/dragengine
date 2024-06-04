
// Calculate cache first ray tex-coord from probe index
ivec2 giRayCastCacheFirstTCFromProbeIndex( in int probeIndex ){
	return ivec2( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ),
		probeIndex / pGIProbesPerLine );
}

// Calculate cache ray tex-coord from probe index
ivec3 giRayCastCacheTCFromProbeIndex( in int probeIndex, in int rayIndex ){
	return ivec3( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ) + rayIndex,
		probeIndex / pGIProbesPerLine, pGICascade );
}
