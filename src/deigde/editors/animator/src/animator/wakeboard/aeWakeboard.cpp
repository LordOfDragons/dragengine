/* 
 * Drag[en]gine IGDE Animator Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeWakeboard.h"
#include "../aeAnimator.h"
#include "../../gui/aeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/module/igdeEditorModule.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>



// Class aeWakeboard
//////////////////////

// Constructor, destructor
////////////////////////////

aeWakeboard::aeWakeboard( aeAnimator *animator ){
	if( ! animator ){
		DETHROW( deeInvalidParam );
	}
	
	deEngine * const engine = animator->GetEngine();
	deWorld &engWorld = *animator->GetEngineWorld();
	decVector boxHalfSize( 5.0f, 0.05f, 5.0f );
	decVector boxPosition( 0.0f, -boxHalfSize.y, 0.0f );
	decLayerMask layermask;
	deSkin *engSkin = NULL;
	deModel *engModel = NULL;
	decShape *shapeBox = NULL;
	decShapeList shapeList;
	
	//layermask.SetBit( aeAnimator::eclTerrain );
	//layermask.SetBit( aeAnimator::eclElements );
	layermask.SetBit( aeAnimator::eclAI );
	layermask.SetBit( aeAnimator::eclGround );
	
	pAnimator = animator;
	
	pEngComponent = NULL;
	pEngCollider = NULL;
	
	pEnabled = false;
	
	pTiltUpDown = 0.0f;
	pMaxTiltUpDown = 30.0f;
	pTiltLeftRight = 0.0f;
	pMaxTiltLeftRight = 30.0f;
	
	deVirtualFileSystem * const vfsData = animator->GetEnvironment()->GetFileSystemIGDE();
	decString pathData;
	pathData.Format( "/data/modules/%s/",
		animator->GetWindowMain().GetEditorModule().GetEditorDirectory().GetString() );
	
	try{
		engModel = engine->GetModelManager()->LoadModel( vfsData,
			pathData + "models/wakeboard/wakeboard.demodel", "/" );
		engSkin = engine->GetSkinManager()->LoadSkin( vfsData,
			pathData + "models/wakeboard/wakeboard.deskin", "/" );
		
		pEngComponent = engine->GetComponentManager()->CreateComponent( engModel, engSkin );
		pEngComponent->SetVisible( pEnabled );
		engModel->FreeReference();
		engModel = NULL;
		engSkin->FreeReference();
		engSkin = NULL;
		engWorld.AddComponent( pEngComponent );
		
		pEngCollider = engine->GetColliderManager()->CreateColliderVolume();
		pEngCollider->SetResponseType( deCollider::ertKinematic );
		pEngCollider->SetUseLocalGravity( true );
		pEngCollider->SetEnabled( true );
		pEngCollider->SetCollisionFilter( decCollisionFilter( layermask ) );
		pEngCollider->SetEnabled( pEnabled );
		
		shapeBox = new decShapeBox( boxHalfSize, boxPosition );
		shapeList.Add( shapeBox );
		shapeBox = NULL;
		pEngCollider->SetShapes( shapeList );
		
		engWorld.AddCollider( pEngCollider );
		
	}catch( const deException & ){
		if( shapeBox ){
			delete shapeBox;
		}
		if( engSkin ){
			engSkin->FreeReference();
		}
		if( engModel ){
			engModel->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

aeWakeboard::~aeWakeboard(){
	pCleanUp();
}



// Management
///////////////

void aeWakeboard::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	pEngComponent->SetVisible( enabled );
	pEngCollider->SetEnabled( enabled );
}



void aeWakeboard::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		pEngComponent->SetPosition( position );
		pEngCollider->SetPosition( position );
	}
}



void aeWakeboard::SetTiltUpDown( float tilt ){
	if( tilt > pMaxTiltUpDown ){
		tilt = pMaxTiltUpDown;
		
	}else if( tilt < -pMaxTiltUpDown ){
		tilt = -pMaxTiltUpDown;
	}
	
	if( fabsf( tilt - pTiltUpDown ) > 1e-5f ){
		pTiltUpDown = tilt;
		UpdateOrientation();
	}
}

void aeWakeboard::SetMaximumTiltUpDown( float tilt ){
	if( tilt < 0.0f ){
		tilt = 0.0f;
	}
	
	pMaxTiltUpDown = tilt;
	SetTiltUpDown( pTiltUpDown );
}

void aeWakeboard::SetTiltLeftRight( float tilt ){
	if( tilt > pMaxTiltLeftRight ){
		tilt = pMaxTiltLeftRight;
		
	}else if( tilt < -pMaxTiltLeftRight ){
		tilt = -pMaxTiltLeftRight;
	}
	
	if( fabsf( tilt - pTiltLeftRight ) > 1e-5f ){
		pTiltLeftRight = tilt;
		UpdateOrientation();
	}
}

void aeWakeboard::SetMaximumTiltLeftRight( float tilt ){
	if( tilt < 0.0f ){
		tilt = 0.0f;
	}
	
	pMaxTiltLeftRight = tilt;
	SetTiltLeftRight( pTiltLeftRight );
}

void aeWakeboard::UpdateOrientation(){
	decQuaternion orientation = decDMatrix::CreateRotation( pTiltUpDown * DEG2RAD, 0.0f, pTiltLeftRight * DEG2RAD ).ToQuaternion();
	
	pEngComponent->SetOrientation( orientation );
	pEngCollider->SetOrientation( orientation );
}



// Private Functions
//////////////////////

void aeWakeboard::pCleanUp(){
	if( pAnimator ){
		deWorld &engWorld = *pAnimator->GetEngineWorld();
		
		if( pEngCollider ){
			engWorld.RemoveCollider( pEngCollider );
			pEngCollider->FreeReference();
		}
		
		if( pEngComponent ){
			engWorld.RemoveComponent( pEngComponent );
			pEngComponent->FreeReference();
		}
	}
}
