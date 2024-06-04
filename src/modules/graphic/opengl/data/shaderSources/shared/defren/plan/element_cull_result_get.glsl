
// element cull results are stored as uvec4 with each component storing one element.
// each element is composed like this:
// - bit 0-7: cull flags
// - bit 8: visible
// - bit 12-15: lod index (max 15)


// get element cull result
uint getElementCullResult( in uint index ){
	return pElementCullResult[ index / uint( 4 ) ][ index % uint( 4 ) ];
}

// split element cull result into components
void splitElementCullResult( in uint cullResult, out uint cullFlags, out bool visible, out uint lodIndex ){
	cullFlags = cullResult & uint( 0xff );
	visible = ( cullResult & uint( 0x100 ) ) == uint( 0x100 );
	lodIndex = cullResult >> 9;
}

// create element cull result from components. includes ecrVisible
uint composeElementCullResult( in uint cullFlags, in uint lodIndex ){
	return uint( 0x100 ) | cullFlags | ( lodIndex << uint( 12 ) );
}

// get lod index from cull result
uint elementCullResultGetLodIndex( in uint cullResult ){
	return cullResult >> uint( 12 );
}
