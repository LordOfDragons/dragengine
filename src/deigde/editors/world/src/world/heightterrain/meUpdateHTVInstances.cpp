/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meUpdateHTVInstances.h"
#include "meHTVInstance.h"
#include "meHTVVariation.h"
#include "meHTVEvaluationEnvironment.h"
#include "meHTVegetationLayer.h"
#include "../meWorld.h"
#include "../terrain/meHeightTerrain.h"
#include "../terrain/meHeightTerrainSector.h"
#include "../terrain/meHeightTerrainPropField.h"
#include "../../utils/meBitArray.h"

#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#define random rand
#define srandom srand
#endif



// Class meUpdateHTVInstances
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUpdateHTVInstances::meUpdateHTVInstances( meHeightTerrainPropField &propField ) :
pPropField( propField ){
}

meUpdateHTVInstances::~meUpdateHTVInstances(){
}



// Management
///////////////
//#include <dragengine/common/utils/decTimer.h>

void meUpdateHTVInstances::UpdateInstances(){
	meHeightTerrainSector * const htsector = pPropField.GetHTSector();
	dePropField * const engPF = pPropField.GetEnginePropField();
	
	if( ! engPF || ! htsector || ! htsector->GetHeightTerrain() ){
		// this should never fail but to avoid troubles we do nothing if so
		return;
	}
	
	// to be save remove all instances first
	pPropField.RemoveAllVInstances();
	
	// add instances by evaluating the individual vegetation layers in order
	const decDVector sectorPosition( htsector->GetWorldPosition() );
	const decDVector &fieldPosition = engPF->GetPosition();
	decDVector srfpos = fieldPosition - sectorPosition;
	meHTVEvaluationEnvironment evalEnv;
	
	int l, layerCount = htsector->GetHeightTerrain()->GetVLayerCount();
	decVector inormal, sripos;
	decDVector evalPos;
	
	double rfactor = 1.0 / ( double )RAND_MAX;
	decVector ipos, irot;
	int i, instanceCount;
	float iscale;
	bool evaluateInstance;
	
	const int resolution = htsector->GetHeightTerrain()->GetSectorResolution();
	const float invSectorDim = 1.0f / htsector->GetHeightTerrain()->GetSectorSize();
	
	srandom( 1 );
	
	const float rsize = htsector->GetHeightTerrain()->GetSectorSize() / ( float )htsector->GetPropFieldCellCount();
	const float roffset = rsize * 0.5f;
	const float rpos = rfactor * rsize;
	//const float rmaskpos = ( float )imageDim * invSectorDim;
	
	// set up the evaluation environment
	if( htsector->GetHeightTerrain() ){
		evalEnv.SetWorld( &htsector->GetHeightTerrain()->GetWorld() );
		
	}else{
		evalEnv.SetWorld( NULL );
	}
	evalEnv.SetHTSector( htsector );
	evalEnv.SetPropField( &pPropField );
	
	const float occupCellResolution = 0.5f; //0.1f;
	const float invOccupCellResolution = 1.0f / occupCellResolution;
	const int occupCellCountXZ = ( int )floorf( rsize * invOccupCellResolution + 0.5f );
//	const int occupCellCount = occupCellCountXZ * occupCellCountXZ;
	meBitArray occupation( occupCellCountXZ, occupCellCountXZ );
	
	evalEnv.SetOccupation( &occupation );
	
	occupation.ClearTo( false );
	
//	int oldVInstCount = 0;
//	decTimer timer;
	for( l=0; l<layerCount; l++ ){
		meHTVegetationLayer * const vlayer = htsector->GetHeightTerrain()->GetVLayerAt( l );
		
		const int variationCount = vlayer->GetVariationCount();
		if( variationCount == 0 ){
			continue;
		}
		
		evalEnv.SetVLayer( vlayer );
		evalEnv.Prepare();
		
		instanceCount = 50000; //10000; // 10k
		
		for( i=0; i<instanceCount; i++ ){
			evaluateInstance = false;
			
			ipos.x = rpos * ( float )random();
			ipos.z = rpos * ( float )random();
			
			const int ox = decMath::clamp( ( int )( ipos.x * invOccupCellResolution ), 0, occupCellCountXZ - 1 );
			const int oy = decMath::clamp( ( int )( ipos.z * invOccupCellResolution ), 0, occupCellCountXZ - 1 );
			
			if( occupation.GetValueAt( ox, oy ) ){
				continue;
			}
			
			ipos.x -= roffset;
			ipos.z -= roffset;
			
			sripos.x = srfpos.x + ipos.x;
			sripos.z = srfpos.z + ipos.z;
			
			if( htsector->GetHeightNormalAt( sripos.x, sripos.z, ipos.y, inormal ) ){
				evalPos.x = fieldPosition.x + ( double )ipos.x;
				evalPos.y = ( double )ipos.y;
				evalPos.z = fieldPosition.z + ( double )ipos.z;
				evalEnv.SetParameters( evalPos, inormal );
				evalEnv.SetHTCoordinates( ( sripos.x * invSectorDim + 0.5f ) * ( float )resolution, ( 0.5f - sripos.z * invSectorDim ) * ( float )resolution );
				evalEnv.SetHTDominantTexture( htsector->GetTextureUnder( evalEnv.GetHTCoordinates().x, evalEnv.GetHTCoordinates().y ) );
				evaluateInstance = true;
			}
			
			if( ! evaluateInstance ){
				continue;
			}
			
			evalEnv.EvaluateRules();
			if( evalEnv.GetProbability() < ( float )random() * rfactor ){
				continue;
			}
			
			meHTVInstance &vinstance = pPropField.AddVInstance();
			const int variationIndex = decMath::clamp( evalEnv.GetVariation(), 0, variationCount - 1 );
			meHTVVariation &variation = *vlayer->GetVariationAt( variationIndex );
			
			const float minrot = variation.GetMinimumRandomRotation() * DEG2RAD;
			const float maxrot = variation.GetMaximumRandomRotation() * DEG2RAD;
			const float minscale = variation.GetMinimumRandomScaling();
			const float maxscale = variation.GetMaximumRandomScaling();
			
			float rval = ( float )random() * rfactor;
			irot.y = minrot * ( 1.0f - rval ) + maxrot * rval;
			
			if( inormal.y < 0.99f ){
				irot = ( decMatrix::CreateRotationY( irot.y ) * decMatrix::CreateVU(
					decVector( -inormal.x * inormal.y, inormal.x * inormal.x + inormal.z * inormal.z, -inormal.z * inormal.y ),
					inormal ) ).GetEulerAngles();
				
			}else{
				irot.x = 0.0f;
				irot.z = 0.0f;
			}
			
			rval = ( float )random() * rfactor;
			iscale = minscale * ( 1.0f - rval ) + maxscale * rval;
			
			vinstance.SetParameters( l, variationIndex, ipos, irot, iscale );
			
			occupation.SetValueAt( ox, oy, true );
		}
		
//		printf( "PField (%g,%g): VLayer %i: Added %i VInstances.\n", fieldPosition.x, fieldPosition.z, l, pPropField.GetVInstanceCount() - oldVInstCount );
//		oldVInstCount = pPropField.GetVInstanceCount();
	}
	
//	printf( "PField (%g,%g): %i VInstances ( in %gs ).\n", fieldPosition.x, fieldPosition.z, oldVInstCount, timer.GetElapsedTime() );
}
