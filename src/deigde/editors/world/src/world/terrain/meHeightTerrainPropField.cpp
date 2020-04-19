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

#include "meHeightTerrain.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainPropField.h"
#include "meHeightTerrainPFType.h"
#include "meUpdateHeightTerrainPropField.h"
#include "../meWorld.h"
#include "../heightterrain/meHTVInstance.h"
#include "../heightterrain/meUpdateHTVInstances.h"
#include "../heightterrain/meHTVegetationLayer.h"
#include "../heightterrain/meHTVVariation.h"
#include "../../utils/meByteArray.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>



// Class meHeightTerrainPropField
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainPropField::meHeightTerrainPropField( deEngine *engine ) :
pHTSector( NULL ),
pEngine( engine ),
pEngPF( NULL ),
pVInstances( NULL ),
pVInstanceCount( 0 ),
pVInstanceSize( 0 ),
pDirtyVInstances( false ),
pDirty( false ),
pKeepClean( true ),
pListener( NULL )
{
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pListener = new meUpdateHeightTerrainPropField( this );
		pEngPF = engine->GetPropFieldManager()->CreatePropField();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meHeightTerrainPropField::~meHeightTerrainPropField(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainPropField::SetHTSector( meHeightTerrainSector *htsector ){
	if( htsector == pHTSector ){
		return;
	}
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->GetWorld().GetEngineWorld()->RemovePropField( pEngPF );
	}
	
	pHTSector = htsector;
	
	if( htsector && htsector->GetHeightTerrain() ){
		htsector->GetHeightTerrain()->GetWorld().GetEngineWorld()->AddPropField( pEngPF );
	}
}



void meHeightTerrainPropField::SetExtend( const decVector2 &minimum, const decVector2 &maximum ){
	pMinExtend = minimum;
	pMaxExtend = maximum;
}

void meHeightTerrainPropField::InitDelegates( igdeEnvironment *environment ){
	environment->SetPropFieldDelegee( pEngPF, pListener );
}

void meHeightTerrainPropField::Invalidate(){
	pDirty = true;
}

void meHeightTerrainPropField::Validate(){
	pDirty = false;
}



void meHeightTerrainPropField::Update(){
	pKeepClean = false;
	
	if( pDirty ){
		MarkVInstancesDirty();
		pDirty = false;
	}
	
	RebuildVegetationPropFieldTypes();
}

void meHeightTerrainPropField::Clear(){
	pKeepClean = true;
	
	if( pEngPF ){
		pEngPF->RemoveAllTypes();
	}
}



void meHeightTerrainPropField::RebuildVegetationPropFieldTypes(){
	if( ! pEngPF ){
		return;
	}
	
	pEngPF->RemoveAllTypes();
	
	if( pKeepClean ){
		return;
	}
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	
	decLayerMask cfCategory;
	cfCategory.SetBit( meWorld::eclmPropFields );
	
	decLayerMask cfFilter;
	cfFilter.SetBit( meWorld::eclmEditing );
	cfFilter.SetBit( meWorld::eclmForceField );
	cfFilter.SetBit( meWorld::eclmParticles );
	
	const decCollisionFilter collisionFilter( cfCategory, cfFilter );
	
	const int layerCount = pHTSector->GetHeightTerrain()->GetVLayerCount();
	dePropFieldType *engPFType = NULL;
	int i, j;
	
	try{
		for( i=0; i<layerCount; i++ ){
			meHTVegetationLayer &vlayer = *pHTSector->GetHeightTerrain()->GetVLayerAt( i );
			const int variationCount = vlayer.GetVariationCount();
			
			for( j=0; j<variationCount; j++ ){
				meHTVVariation &variation = *vlayer.GetVariationAt( j );
				
				engPFType = new dePropFieldType;
				engPFType->SetModel( variation.GetModel() );
				engPFType->SetSkin( variation.GetSkin() );
				engPFType->SetRotationPerForce( variation.GetRotationPerForce() * DEG2RAD );
				engPFType->SetRestitution( variation.GetRestitution() );
				engPFType->SetCollisionFilter( collisionFilter );
				
				pEngPF->AddType( engPFType );
				engPFType = NULL;
			}
		}
		
	}catch( const deException & ){
		if( engPFType ){
			delete engPFType;
		}
		throw;
	}
}



meHTVInstance &meHeightTerrainPropField::GetVInstanceAt( int index ) const{
	if( index < 0 || index >= pVInstanceCount ){
		DETHROW( deeInvalidParam );
	}
	return pVInstances[ index ];
}

meHTVInstance &meHeightTerrainPropField::AddVInstance(){
	if( pVInstanceCount == pVInstanceSize ){
		const int newSize = pVInstanceSize + 500;
		meHTVInstance * const newArray = new meHTVInstance[ newSize ];
		if( pVInstances ){
			memcpy( newArray, pVInstances, sizeof( meHTVInstance ) * pVInstanceSize );
			delete [] pVInstances;
		}
		pVInstances = newArray;
		pVInstanceSize = newSize;
	}
	
	pVInstanceCount++;
	return pVInstances[ pVInstanceCount - 1 ];
}

void meHeightTerrainPropField::RemoveAllVInstances(){
	pVInstanceCount = 0;
}

void meHeightTerrainPropField::UpdateVInstances(){
	if( ! pDirtyVInstances || ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	
	meUpdateHTVInstances updater( *this );
	updater.UpdateInstances();
	
	pDirtyVInstances = false;
	pHTSector->SetPFCacheChanged( true );
}

void meHeightTerrainPropField::MarkVInstancesDirty(){
	if( pDirtyVInstances ){
		return;
	}
	
	RemoveAllVInstances();
	pDirtyVInstances = true;
}

void meHeightTerrainPropField::MarkVInstancesValid(){
	pDirtyVInstances = false;
}



// Private Functions
//////////////////////

void meHeightTerrainPropField::pCleanUp(){
	SetHTSector( NULL );
	
	if( pVInstances ){
		delete [] pVInstances;
	}
	
	if( pEngPF ){
		pEngPF->FreeReference();
	}
	
	if( pListener ){
		delete pListener;
	}
}
