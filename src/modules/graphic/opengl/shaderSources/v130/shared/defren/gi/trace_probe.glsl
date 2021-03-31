
// Probe index from trace instance id
int giTraceProbeProbeIndex( in int instanceID ){
	// 4 IDs per array entry
	return pGIProbeIndex[ instanceID >> 2 ][ instanceID & 3 ];
}
