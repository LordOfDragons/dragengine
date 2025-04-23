UBOLAYOUT_BIND(2) uniform GIProbeIndices{
	ivec4 pGIProbeIndex[ GI_MAX_COUNT_PROBE_POSITIONS / 4 ]; // 4 indices per element
};

// Probe index from trace instance id
int giTraceProbeProbeIndex( in int instanceID ){
	// 4 IDs per array entry
	return pGIProbeIndex[ instanceID >> 2 ][ instanceID & 3 ];
}
