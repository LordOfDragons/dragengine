/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <memory.h>

#include "deoglSkinTexturePipelineMap.h"


#define STPL(x) deoglSkinTexturePipelinesList::x
#define STP(x) deoglSkinTexturePipelines::x


// Class deoglSkinTexturePipelineMap
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinTexturePipelineMap::deoglSkinTexturePipelineMap(){
	pInitMap();
}



// Management
///////////////


int deoglSkinTexturePipelineMap::Map( STPL(ePipelineTypes) list, STP(eTypes) type, STP(eModifiers) modifier ){
	return pMap[ list ][ type ][ modifier ];
}



// Private Functions
//////////////////////

void deoglSkinTexturePipelineMap::pInitMap(){
	int i = 0;
	
	memset( pMap, 0, sizeof( pMap ) );
	memset( pMapInv, 0, sizeof( pMapInv ) );
	
	// component: 77
	pInitAllGeometryDepthCounterShadow( i, STPL(eptComponent) ); // 68
	pInitMask( i, STPL(eptComponent) ); // 8
	pInitGIMaterial( i, STPL(eptComponent) ); // 1
	
	// billboard: 64
	pInitGeometry( i, STPL(eptBillboard) ); // 8
	pInitAllDepth( i, STPL(eptBillboard) ); // 32
	pInitAllCounter( i, STPL(eptBillboard) ); // 16
	pInitMask( i, STPL(eptBillboard) ); // 8
	
	// decal: 68
	pInitAllGeometryDepthCounterShadow( i, STPL(eptDecal) ); // 68
	
	// prop field: 68
	pInitAllGeometryDepthCounterShadow( i, STPL(eptPropField) ); // 68
	
	// prop field imposter: 68
	pInitAllGeometryDepthCounterShadow( i, STPL(eptPropFieldImposter) ); // 68
	
	// height map 1: 68
	pInitAllGeometryDepthCounterShadow( i, STPL(eptHeightMap1) ); // 68
	
	// height map 2: 68
	pInitAllGeometryDepthCounterShadow( i, STPL(eptHeightMap2) ); // 68
	
	// particle: 84
	pInitAllGeometryDepthCounterShadow( i, STPL(eptParticle) ); // 68
	pInitGeometryDepthTest( i, STPL(eptParticle) ); // 8
	pInitMask( i, STPL(eptParticle) ); // 8
	
	// particle ribbon: 64
	pInitGeometry( i, STPL(eptParticleRibbon) ); // 8
	pInitGeometryDepthTest( i, STPL(eptParticleRibbon) ); // 8
	pInitAllDepth( i, STPL(eptParticleRibbon) ); // 32
	pInitAllCounter( i, STPL(eptParticleRibbon) ); // 16
	
	// particle beam: 64
	pInitGeometry( i, STPL(eptParticleBeam) ); // 8
	pInitGeometryDepthTest( i, STPL(eptParticleBeam) ); // 8
	pInitAllDepth( i, STPL(eptParticleBeam) ); // 32
	pInitAllCounter( i, STPL(eptParticleBeam) ); // 16
	
	// outline: 56
	pInitGeometry( i, STPL(eptOutline) ); // 8
	pInitAllDepth( i, STPL(eptOutline) ); // 32
	pInitAllCounter( i, STPL(eptOutline) ); // 16
	
	// total: 749 of possible 1584 (~47%)
	DEASSERT_TRUE( i == 749 )
}

void deoglSkinTexturePipelineMap::pInitGeometry( int &index, STPL(ePipelineTypes) list ){
	pSetModified( index, list, STP(etGeometry), STP(emStereo) | STP(emFlipCullFace) | STP(emDoubleSided) ); // 8
}

void deoglSkinTexturePipelineMap::pInitGeometryDepthTest( int &index, STPL(ePipelineTypes) list ){
	pSetModified( index, list, STP(etGeometryDepthTest), STP(emStereo) | STP(emFlipCullFace) | STP(emDoubleSided) ); // 8
}

void deoglSkinTexturePipelineMap::pInitMask( int &index, STPL(ePipelineTypes) list ){
	pSetModified( index, list, STP(etMask), STP(emStereo) | STP(emFlipCullFace) | STP(emDoubleSided) ); // 8
}

void deoglSkinTexturePipelineMap::pInitGIMaterial( int &index, STPL(ePipelineTypes) list ){
	pSetModified( index, list, STP(etGIMaterial), 0 ); // 1
}

void deoglSkinTexturePipelineMap::pInitAllDepth( int &index, STPL(ePipelineTypes) list ){
	const int modifiers = STP(emStereo) | STP(emFlipCullFace) | STP(emDoubleSided);
	pSetModified( index, list, STP(etDepth), modifiers ); // 8
	pSetModified( index, list, STP(etDepthClipPlane), modifiers ); // 8
	pSetModified( index, list, STP(etDepthReversed), modifiers ); // 8
	pSetModified( index, list, STP(etDepthClipPlaneReversed), modifiers ); // 8
}

void deoglSkinTexturePipelineMap::pInitAllCounter( int &index, STPL(ePipelineTypes) list ){
	const int modifiers = STP(emStereo) | STP(emFlipCullFace) | STP(emDoubleSided);
	pSetModified( index, list, STP(etCounter), modifiers ); // 8
	pSetModified( index, list, STP(etCounterClipPlane), modifiers ); // 8
}

void deoglSkinTexturePipelineMap::pInitAllShadow( int &index, STPL(ePipelineTypes) list ){
	const int modifiers = STP(emDoubleSided);
	pSetModified( index, list, STP(etShadowProjection), modifiers ); // 2
	pSetModified( index, list, STP(etShadowProjectionCube), modifiers ); // 2
	pSetModified( index, list, STP(etShadowOrthogonal), modifiers ); // 2
	pSetModified( index, list, STP(etShadowOrthogonalCascaded), modifiers ); // 2
	pSetModified( index, list, STP(etShadowDistance), modifiers ); // 2
	pSetModified( index, list, STP(etShadowDistanceCube), modifiers ); // 2
}

void deoglSkinTexturePipelineMap::pInitAllGeometryDepthCounterShadow( int &index, STPL(ePipelineTypes) list ){
	pInitGeometry( index, list ); // 8
	pInitAllDepth( index, list ); // 32
	pInitAllCounter( index, list ); // 16
	pInitAllShadow( index, list ); // 12
}

void deoglSkinTexturePipelineMap::pSetModified( int &index,
STPL(ePipelineTypes) list, STP(eTypes) type, int modifierMask ){
	int modifier;
	
	for( modifier=0; modifier<STP(ModifiersPerType); modifier++ ){
		if( ( modifier & modifierMask ) == modifier ){
			pMapInv[ index ].list = list;
			pMapInv[ index ].type = type;
			pMapInv[ index ].modifiers = modifier;
			
			pMap[ list ][ type ][ modifier ] = index++;
		}
	}
}
