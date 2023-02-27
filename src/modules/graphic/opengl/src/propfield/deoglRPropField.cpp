/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglPropFieldCluster.h"
#include "deoglRPropField.h"
#include "deoglRPropFieldType.h"
#include "../model/deoglRModel.h"
#include "../renderthread/deoglRenderThread.h"
#include "../texture/deoglTextureStageManager.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>



// Class deoglRPropField
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPropField::deoglRPropField( deoglRenderThread &renderThread) :
pRenderThread( renderThread ),

pParentWorld( NULL ),

pTypesRequirePrepareForRender( true ),

pWorldMarkedRemove( false ),
pLLPrepareForRenderWorld( this )
{
	LEAK_CHECK_CREATE( renderThread, PropField );
}

deoglRPropField::~deoglRPropField(){
	LEAK_CHECK_FREE( pRenderThread, PropField );
	SetParentWorld( nullptr );
}



// Management
///////////////

void deoglRPropField::SetParentWorld( deoglRWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	if( pParentWorld ){
		for( i=0; i<count; i++ ){
			( ( deoglRPropFieldType* )pTypes.GetAt( i ) )->RemoveFromWorldCompute();
		}
	}
	
	pParentWorld = world;
	
	if( world ){
		deoglWorldCompute &worldCompute = world->GetCompute();
		for( i=0; i<count; i++ ){
			( ( deoglRPropFieldType* )pTypes.GetAt( i ) )->AddToWorldCompute( worldCompute );
		}
	}
}



void deoglRPropField::SetPosition( const decDVector &position ){
	pPosition = position;
}



void deoglRPropField::UpdateExtends( const dePropField &propField ){
	const int typeCount = pTypes.GetCount();
	decVector minExtend;
	decVector maxExtend;
	int t, i;
	
	for( t=0; t<typeCount; t++ ){
		const dePropFieldType &engType = *propField.GetTypeAt( t );
		const deoglRPropFieldType &type = GetTypeAt( t );
		
		if( ! type.GetModel() || ! engType.GetSkin() ){
			continue;
		}
		
		const deoglRModel::sExtends &modelExtends = type.GetModel()->GetExtends();
		
		float modelRadius = modelExtends.minimum.Length();
		float mr2 = modelExtends.maximum.Length();
		if( mr2 > modelRadius ){
			modelRadius = mr2;
		}
		
		const int instanceCount = engType.GetInstanceCount();
		dePropFieldInstance * const engInstances = engType.GetInstances();
		
		for( i=0; i<instanceCount; i++ ){
			const decVector &instancePosition = engInstances[ i ].GetPosition();
			const float instanceRadius = modelRadius * engInstances[ i ].GetScaling();
			if( instanceRadius < 1e-5f ){
				continue;
			}
			
			minExtend.x = decMath::min( minExtend.x, instancePosition.x - instanceRadius );
			minExtend.y = decMath::min( minExtend.y, instancePosition.y - instanceRadius );
			minExtend.z = decMath::min( minExtend.z, instancePosition.z - instanceRadius );
			
			maxExtend.x = decMath::max( maxExtend.x, instancePosition.x + instanceRadius );
			maxExtend.y = decMath::max( maxExtend.y, instancePosition.y + instanceRadius );
			maxExtend.z = decMath::max( maxExtend.z, instancePosition.z + instanceRadius );
		}
	}
	
	pMinExtend.x = pPosition.x + ( double )minExtend.x - 0.01;
	pMinExtend.y = pPosition.y + ( double )minExtend.y - 0.01;
	pMinExtend.z = pPosition.z + ( double )minExtend.z - 0.01;
	
	pMaxExtend.x = pPosition.x + ( double )maxExtend.x + 0.01;
	pMaxExtend.y = pPosition.y + ( double )maxExtend.y + 0.01;
	pMaxExtend.z = pPosition.z + ( double )maxExtend.z + 0.01;
	
	if( pParentWorld ){
		for( i=0; i<typeCount; i++ ){
			( ( deoglRPropFieldType* )pTypes.GetAt( i ) )->UpdateWorldCompute();
		}
	}
	
	//pOgl->LogInfoFormat( "PropField.pUpdateExtends: p=(%.3f,%.3f,%.3f) e=(%.3f,%.3f,%.3f)->(%.3f,%.3f,%.3f)",
	//	fieldPosition.x, fieldPosition.y, fieldPosition.z,
	//	pMinExtend.x, pMinExtend.y, pMinExtend.z, pMaxExtend.x, pMaxExtend.y, pMaxExtend.z );
}



void deoglRPropField::PrepareForRender(){
	if( pTypesRequirePrepareForRender ){
		pTypesRequirePrepareForRender = false;
		
		const int count = pTypes.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			( ( deoglRPropFieldType* )pTypes.GetAt( i ) )->PrepareForRender();
		}
	}
}

void deoglRPropField::PrepareForRenderRender(){
}

void deoglRPropField::PrepareInstances( const decDVector &cameraPosition, const decDMatrix &cameraMatrix ){
	// NOTE this could be optimized since the instances and their bend state are different
	// data objects. if the instances change their base matrix can be calculated and stored
	// aside. then the bending state can be applied in the shader later on. currently though
	// every change triggers the rebuilding of the matrices which is slower as required
	
//pOgl->LogInfoFormat( "pf=(%g,%g,%g) cp=(%g,%g,%g)", pPropField->GetPosition().x, pPropField->GetPosition().y,
//	pPropField->GetPosition().z, cameraPosition.x, cameraPosition.y, cameraPosition.z );
//	int t;
	
//	PrepareForRender();
//pOgl->LogInfoFormat( "tc=%i is=%i", pTypeCount, pInstanceSize );
	
/*	if( pLODLevel > 0 ){
		for( t=0; t<pTypeCount; t++ ){
			pTypes[ t ]->UpdateInstances( cameraPosition, cameraMatrix );
		}
		
	}else{
		for( t=0; t<pTypeCount; t++ ){
			pTypes[ t ]->RemoveAllClusters();
		}
	}*/
}



int deoglRPropField::GetTypeCount() const{
	return pTypes.GetCount();
}

deoglRPropFieldType &deoglRPropField::GetTypeAt( int index ) const{
	return *( ( deoglRPropFieldType* )pTypes.GetAt( index ) );
}

void deoglRPropField::RemoveAllTypes(){
	pTypes.RemoveAll();
}

void deoglRPropField::AddType( deoglRPropFieldType *type ){
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	pTypes.Add( type );
	TypeRequiresPrepareForRender();
}

void deoglRPropField::TypeRequiresPrepareForRender(){
	pTypesRequirePrepareForRender = true;
	pRequiresPrepareForRender();
}



void deoglRPropField::WorldReferencePointChanged(){
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRPropFieldType* )pTypes.GetAt( i ) )->WorldReferencePointChanged();
	}
}

void deoglRPropField::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}



// Private Functions
//////////////////////

void deoglRPropField::pRequiresPrepareForRender(){
	if( ! pLLPrepareForRenderWorld.GetList() && pParentWorld ){
		pParentWorld->AddPrepareForRenderPropField( this );
	}
}
