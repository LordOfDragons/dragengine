
// constants
const uint vElementGeometryTucMap[ 18 ] = uint[18](
	ettGeometry, // deoglSkinTexturePipelines::etGeometry
	0, // deoglSkinTexturePipelines::etGeometryDepthTest
	ettDepth, // deoglSkinTexturePipelines::etDepth
	ettDepth, // deoglSkinTexturePipelines::etDepthClipPlane
	ettDepth, // deoglSkinTexturePipelines::etDepthReversed
	ettDepth, // deoglSkinTexturePipelines::etDepthClipPlaneReversed
	ettDepth, // deoglSkinTexturePipelines::etMask
	ettCounter, // deoglSkinTexturePipelines::etCounter
	ettCounter, // deoglSkinTexturePipelines::etCounterClipPlane
	ettShadow, // deoglSkinTexturePipelines::etShadowProjection
	ettShadow, // deoglSkinTexturePipelines::etShadowProjectionCube
	ettShadow, // deoglSkinTexturePipelines::etShadowOrthogonal
	ettShadow, // deoglSkinTexturePipelines::etShadowOrthogonalCascaded
	ettShadow, // deoglSkinTexturePipelines::etShadowDistance
	ettShadowCube, // deoglSkinTexturePipelines::etShadowDistanceCube
	ettEnvMap, // deoglSkinTexturePipelines::etEnvMap
	ettLuminance, // deoglSkinTexturePipelines::etLuminance
	ettGIMaterial // deoglSkinTexturePipelines::etGIMaterial
	);


// element geometry tuc
uint elementGeometryTUC( in uint geometry, in uint pipelineType ){
	uint i = vElementGeometryTucMap[ pipelineType ];
	uint c = pElementGeometries[ geometry ].tucs[ i / uint( 2 ) ];
	return ( i % uint( 2 ) ) == uint( 0 ) ? ( c & uint( 0xffff ) ) : ( c >> uint( 16 ) );
}
