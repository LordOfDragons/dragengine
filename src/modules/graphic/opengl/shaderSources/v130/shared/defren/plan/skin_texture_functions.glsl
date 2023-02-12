
// get skin texture pipeline
uint skinTexturePipeline( in uint skinTexture, in uint list, in uint type, int uint modifier ){
	// absolute index
	uint i = vSkinTexturePipelinesPerTexture * skinTexture + vSkinTexturePipelinesPerList * list
		+ vSkinTexturePipelinesPerType * type + modifier;
	
	// each uvec4 stores 8 pipelines as packed 16-bit uint
	uint arrInd1 = i / uint( 8 );
	
	i = i % uint( 8 );
	uint arrInd2 = i / uint( 2 );
	
	uint c = pSkinTexturePipelines[ arrInd1 ][ arrInd2 ];
	
	return ( i % uint( 2 ) ) == uint( 0 ) ? ( c & uint( 0xffff ) ) : ( c >> uint( 16 ) );
}
