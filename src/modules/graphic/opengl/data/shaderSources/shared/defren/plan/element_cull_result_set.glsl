
// element cull results are stored as uvec4 with each component storing one element.
// each element is composed like this:
// - bit 0-7: cull flags
// - bit 8: visible
// - bit 12-15: lod index (max 15)


// set element cull result
void setElementCullResult( in uint index, uint cullResult ){
	pElementCullResult[ index / uint( 4 ) ][ index % uint( 4 ) ] = cullResult;
}

// create element cull result from components. includes ecrVisible
uint composeElementCullResult( in uint cullFlags, in uint lodIndex ){
	return ecrVisible | cullFlags | ( lodIndex << uint( 12 ) );
}
