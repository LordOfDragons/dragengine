
// element texture

struct sElementTexture{
	uint renderFilter; // erf* (world_element_constants)
	                   // NOTE fits into 16-bit uint
	uint skinTexture; // index into pSkinTextures
	                  // NOTE fits into 16-bit uint
	uint pipelineBase; // epl* and epm* (world_element_constants)
	                   // bits 0->8: pipeline list (epl*)
	                   // bits 8->16: pipeline modifier (epm*)
	                   // NOTE fits into 16-bit uint
	uint spbInstance; // shared parameter block instance index
	
	uvec4 tucs; // 8 16-bit tucs
	uvec2 tucsOutline; // 3 16-bit outline tucs
	
// 	uint sharedInstance; // render task shared instance index
	                     // NOTE this is per LOD so it can not be here!
	                     // NOTE value is stored in ModelLOD not ComponentLOD
};


#ifndef ELEMENT_TEXTURE_BINDING_POINT
#define ELEMENT_TEXTURE_BINDING_POINT 3
#endif

UBOLAYOUT_BIND(ELEMENT_TEXTURE_BINDING_POINT) readonly buffer ElementTexture {
	sElementTexture pElementTextures[];
};


// constants
const uint vElementTextureTucMap[ 18 ] = uint[18](
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

const uint vElementTextureOutlineTucMap[ 18 ] = uint[18](
	etotGeometry, // deoglSkinTexturePipelines::etGeometry
	etotGeometry, // deoglSkinTexturePipelines::etGeometryDepthTest
	etotDepth, // deoglSkinTexturePipelines::etDepth
	etotDepth, // deoglSkinTexturePipelines::etDepthClipPlane
	etotDepth, // deoglSkinTexturePipelines::etDepthReversed
	etotDepth, // deoglSkinTexturePipelines::etDepthClipPlaneReversed
	etotDepth, // deoglSkinTexturePipelines::etMask
	etotCounter, // deoglSkinTexturePipelines::etCounter
	etotCounter, // deoglSkinTexturePipelines::etCounterClipPlane
	0, // deoglSkinTexturePipelines::etShadowProjection
	0, // deoglSkinTexturePipelines::etShadowProjectionCube
	0, // deoglSkinTexturePipelines::etShadowOrthogonal
	0, // deoglSkinTexturePipelines::etShadowOrthogonalCascaded
	0, // deoglSkinTexturePipelines::etShadowDistance
	0, // deoglSkinTexturePipelines::etShadowDistanceCube
	0, // deoglSkinTexturePipelines::etEnvMap
	0, // deoglSkinTexturePipelines::etLuminance
	0 // deoglSkinTexturePipelines::etGIMaterial
	);


// element texture tuc
uint elementTextureTUC( in uint element, in uint texture, in uint pipelineType ){
	uint t = pElement[ element ].textureFirst + texture;
	uint i = vElementTextureTucMap[ pipelineType ];
	
	uint c = pElementTextures[ t ].tucs[ i / uint( 2 ) ];
	return ( i % uint( 2 ) ) == uint( 0 ) ? ( c & uint( 0xffff ) ) : ( c >> uint( 16 ) );
}


// element texture outline tuc
uint elementTextureOutlineTUC( in uint element, in uint texture, in uint pipelineType ){
	uint t = pElement[ element ].textureFirst + texture;
	uint i = vElementTextureOutlineTucMap[ pipelineType ];
	
	uint c = pElementTextures[ t ].tucsOutline[ i / uint( 2 ) ];
	return ( i % uint( 2 ) ) == uint( 0 ) ? ( c & uint( 0xffff ) ) : ( c >> uint( 16 ) );
}
