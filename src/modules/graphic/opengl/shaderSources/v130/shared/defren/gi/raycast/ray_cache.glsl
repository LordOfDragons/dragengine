
// Calculate cache first ray tex-coord from probe index
ivec2 giRayCastCacheFirstTCFromProbeIndex( in int probeIndex ){
	return ivec2( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ), probeIndex / pGIProbesPerLine );
}

// Calculate cache ray tex-coord from probe index
ivec2 giRayCastCacheTCFromProbeIndex( in int probeIndex, in int rayIndex ){
	ivec2 tc = ivec2( pGIRaysPerProbe * ( probeIndex % pGIProbesPerLine ), probeIndex / pGIProbesPerLine );
	tc.x += rayIndex;
	return tc;
}
