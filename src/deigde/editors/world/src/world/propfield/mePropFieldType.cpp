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

#include "mePropField.h"
#include "mePropFieldType.h"
#include "../meWorld.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>



// Class mePropFieldType
//////////////////////////

// Constructor, destructor
////////////////////////////

mePropFieldType::mePropFieldType( deEngine *engine ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pPropField = NULL;
	
	pEngine = engine;
	pEngPFType = NULL;
	
	pModel = NULL;
	pSkin = NULL;
	pRotPerForceX = 5.0f;
	pRotPerForceZ = 5.0f;
	
	pInstances = NULL;
	pInstanceCount = 0;
}

mePropFieldType::~mePropFieldType(){
	if( pInstances ) delete [] pInstances;
	
	if( pSkin ) pSkin->FreeReference();
	if( pModel ) pModel->FreeReference();
}



// Management
///////////////

void mePropFieldType::SetPropField( mePropField *propField ){
	pPropField = propField;
	pEngPFType = NULL;
}

void mePropFieldType::SetEnginePFType( dePropFieldType *engPFType ){
	if( engPFType && ! pPropField ) DETHROW( deeInvalidParam );
	
	pEngPFType = engPFType;
}



dePropFieldType *mePropFieldType::CreateEnginePFType(){
	if( ! pPropField ) DETHROW( deeInvalidParam );
	
	dePropFieldInstance *engPFTInstances = NULL;
	dePropFieldType *engPFType = NULL;
	int i;
	
	if( pInstanceCount > 0 ){
		try{
			engPFType = new dePropFieldType();
			
			engPFType->SetModel( pModel );
			engPFType->SetSkin( pSkin );
			
			decLayerMask collisionCategory;
			collisionCategory.SetBit( meWorld::eclmPropFields );
			
			decLayerMask collisionFilter;
			collisionFilter.SetBit( meWorld::eclmEditing );
			collisionFilter.SetBit( meWorld::eclmForceField );
			collisionFilter.SetBit( meWorld::eclmParticles );
			
			engPFType->SetCollisionFilter( decCollisionFilter( collisionCategory, collisionFilter ) );
			
			engPFType->SetRotationPerForce( pRotPerForceX * DEG2RAD );
			
			engPFType->SetInstanceCount( pInstanceCount );
			engPFTInstances = engPFType->GetInstances();
			for( i=0; i<pInstanceCount; i++ ){
				engPFTInstances[ i ].SetPosition( pInstances[ i ].GetPosition() );
				engPFTInstances[ i ].SetRotation( pInstances[ i ].GetRotation() );
				engPFTInstances[ i ].SetScaling( pInstances[ i ].GetScaling() );
				// others
			}
			
		}catch( const deException & ){
			if( engPFTInstances ) delete engPFTInstances;
			throw;
		}
	}
	
	return engPFType;
}

void mePropFieldType::UpdateEnginePFType(){
	dePropField *engPropField = NULL;
	int engPFTInstanceCount = 0;
	
	if( pPropField ){
		engPropField = pPropField->GetEnginePropField();
	}
	
	if( pEngPFType ){
		engPFTInstanceCount = pEngPFType->GetInstanceCount();
	}
	
	if( engPFTInstanceCount == pInstanceCount ){
		if( pEngPFType ){
			dePropFieldInstance *engPFTInstances = pEngPFType->GetInstances();
			int i;
			
			for( i=0; i<pInstanceCount; i++ ){
				engPFTInstances[ i ].SetPosition( pInstances[ i ].GetPosition() );
				engPFTInstances[ i ].SetRotation( pInstances[ i ].GetRotation() );
				engPFTInstances[ i ].SetScaling( pInstances[ i ].GetScaling() );
				// and others
			}
		}
		
	}else{
		dePropFieldType *engPFType = NULL;
		
		try{
			engPFType = CreateEnginePFType();
			
			if( pEngPFType && engPropField ){
				engPropField->RemoveType( pEngPFType );
				pEngPFType = NULL;
			}
			
			if( engPFType ){
				engPropField->AddType( engPFType );
				pEngPFType = engPFType;
			}
			
		}catch( const deException & ){
			if( engPFType ) delete engPFType;
			throw;
		}
	}
	
	NotifyTypeChanged();
}

void mePropFieldType::NotifyTypeChanged(){
	if( pPropField ){
		pPropField->SetWorldChanged();
	}
}



void mePropFieldType::SetPathModel( const char *path ){
	if( pPathModel.Equals( path ) ){
		return;
	}
	
	pPathModel = path;
	
	if( pModel ){
		pModel->FreeReference();
		pModel = NULL;
	}
	
	if( ! pPathModel.IsEmpty() ){
		pModel = pEngine->GetModelManager()->LoadModel( pPathModel.GetString(), "/" );
	}
	
	UpdateEnginePFType();
	if( pPropField && pPropField->GetWorld() ){
		pPropField->GetWorld()->NotifyPFTypeChanged( pPropField, this );
	}
}

void mePropFieldType::SetPathSkin( const char *path ){
	if( pPathSkin.Equals( path ) ){
		return;
	}
	
	pPathSkin = path;
	
	if( pSkin ){
		pSkin->FreeReference();
		pSkin = NULL;
	}
	
	if( ! pPathSkin.IsEmpty() ){
		try{
			pSkin = pEngine->GetSkinManager()->LoadSkin( pPathSkin.GetString(), "/" );
			
		}catch( const deException & ){
			if( pPropField && pPropField->GetWorld() ){
				pSkin = pPropField->GetWorld()->GetEnvironment()->GetErrorSkin();
				pSkin->AddReference();
			}
		}
	}
	
	UpdateEnginePFType();
	if( pPropField && pPropField->GetWorld() ){
		pPropField->GetWorld()->NotifyPFTypeChanged( pPropField, this );
	}
}

void mePropFieldType::SetRotationPerForceX( float rotPerForce ){
	rotPerForce = decMath::max( rotPerForce, 0.0f );
	if( fabsf( rotPerForce - pRotPerForceX ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRotPerForceX = rotPerForce;
	
	UpdateEnginePFType();
	if( pPropField && pPropField->GetWorld() ){
		pPropField->GetWorld()->NotifyPFTypeChanged( pPropField, this );
	}
}

void mePropFieldType::SetRotationPerForceZ( float rotPerForce ){
	rotPerForce = decMath::max( rotPerForce, 0.0f );
	if( fabsf( rotPerForce - pRotPerForceZ ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRotPerForceZ = rotPerForce;
	
	UpdateEnginePFType();
	if( pPropField && pPropField->GetWorld() ){
		pPropField->GetWorld()->NotifyPFTypeChanged( pPropField, this );
	}
}



void mePropFieldType::RebuildInstances(){
	// TODO build some default instances for testing
}
