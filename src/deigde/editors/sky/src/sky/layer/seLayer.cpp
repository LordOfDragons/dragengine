/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seLayer.h"
#include "../seSky.h"
#include "../body/seBody.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Class seLayer
/////////////////////

// Constructor, destructor
////////////////////////////

seLayer::seLayer( igdeEnvironment &environment ) :
pEnvironment( environment ),

pSky( NULL ),

pName( "Layer" ),

pColor( 1.0f, 1.0f, 1.0f ),
pIntensity( 20.0f ),
pTransparency( 1.0f ),

pLightColor( 1.0f, 1.0f, 1.0f ),
pLightIntensity( 0.0f ),
pAmbientIntensity( 0.0f ),

pMulBySkyLight( false ),
pMulBySkyColor( false ),

pActiveBody( NULL ),

pActiveTarget( deSkyLayer::etRotationX ),

pSelected( false ),
pActive( false ){
}

seLayer::~seLayer(){
	RemoveAllBodies();
}



// Management
///////////////

void seLayer::SetSky( seSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	pSky = sky;
	UpdateRelativeResources();
}

void seLayer::SetName( const char *name ){
	if( pName == name ){
		return;
	}
	
	pName = name;
	if( pSky ){
		pSky->NotifyLayerNameChanged( this );
	}
}



void seLayer::SetOffset( const decVector &offset ){
	if( offset.IsEqualTo( pOffset ) ){
		return;
	}
	
	pOffset = offset;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetIntensity( float intensity ){
	intensity = decMath::max( intensity, 0.0f );
	if( fabsf( intensity - pIntensity ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pIntensity = intensity;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetTransparency( float transparency ){
	transparency = decMath::clamp( transparency, 0.0f, 1.0f );
	if( fabsf( transparency - pTransparency ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pTransparency = transparency;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetSkinPath( const char *skinPath ){
	if( pPathSkin == skinPath ){
		return;
	}
	
	pPathSkin = skinPath;
	pUpdateSkin();
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}



void seLayer::SetLightOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pLightOrientation ) ){
		return;
	}
	
	pLightOrientation = orientation;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetLightColor( const decColor &color ){
	if( color.IsEqualTo( pLightColor ) ){
		return;
	}
	
	pLightColor = color;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetLightIntensity( float intensity ){
	intensity = decMath::max( intensity, 0.0f );
	if( fabsf( intensity - pLightIntensity ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pLightIntensity = intensity;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetAmbientIntensity( float intensity ){
	intensity = decMath::max( intensity, 0.0f );
	if( fabsf( intensity - pAmbientIntensity ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pAmbientIntensity = intensity;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetMultiplyBySkyLight( bool multiply ){
	if( multiply == pMulBySkyLight ){
		return;
	}
	
	pMulBySkyLight = multiply;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetMultiplyBySkyColor( bool multiply ){
	if( multiply == pMulBySkyColor ){
		return;
	}
	
	pMulBySkyColor = multiply;
	if( pSky ){
		pSky->NotifyLayerChanged( this );
	}
}

void seLayer::SetSelected( bool selected ){
	pSelected = selected;
}

void seLayer::SetActive( bool active ){
	pActive = active;
}

void seLayer::UpdateRelativeResources(){
	pUpdateSkin();
	
	const int bodyCount = pBodies.GetCount();
	int i;
	for( i=0; i<bodyCount; i++ ){
		pBodies.GetAt( i )->UpdateRelativeResources();
	}
}



// Bodies
///////////

void seLayer::AddBody( seBody *body ){
	if( ! body ){
		DETHROW( deeInvalidParam );
	}
	
	pBodies.Add( body );
	body->SetLayer( this );
	
	if( pSky ){
		pSky->NotifyBodyStructureChanged( this );
	}
	
	if( ! pActiveBody ){
		SetActiveBody( body );
	}
}

void seLayer::InsertBodyAt( seBody *body, int index ){
	if( ! body ){
		DETHROW( deeInvalidParam );
	}
	
	pBodies.InsertAt( body, index );
	body->SetLayer( this );
	
	if( pSky ){
		pSky->NotifyBodyStructureChanged( this );
	}
	
	if( ! pActiveBody ){
		SetActiveBody( body );
	}
}

void seLayer::MoveBodyTo( seBody *body, int index ){
	pBodies.MoveTo( body, index );
	
	if( pSky ){
		pSky->NotifyBodyStructureChanged( this );
	}
}

void seLayer::RemoveBody( seBody *body ){
	if( ! pBodies.Has( body ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( body->GetActive() ){
		const int count = pBodies.GetCount();
		const int index = pBodies.IndexOf( body );
		
		if( index < count - 1 ){
			SetActiveBody( pBodies.GetAt( index + 1 ) );
			
		}else if( index > 0 ){
			SetActiveBody( pBodies.GetAt( index - 1 ) );
			
		}else{
			SetActiveBody( NULL );
		}
	}
	
	body->SetLayer( NULL );
	pBodies.Remove( body );
	
	if( pSky ){
		pSky->NotifyBodyStructureChanged( this );
	}
}

void seLayer::RemoveAllBodies(){
	const int count = pBodies.GetCount();
	int i;
	
	SetActiveBody( NULL );
	
	for( i=0; i<count; i++ ){
		pBodies.GetAt( i )->SetLayer( NULL );
	}
	pBodies.RemoveAll();
	
	if( pSky ){
		pSky->NotifyBodyStructureChanged( this );
	}
}

void seLayer::SetActiveBody( seBody *body ){
	if( body == pActiveBody ){
		return;
	}
	
	if( pActiveBody ){
		pActiveBody->SetActive( false );
		pActiveBody->FreeReference();
	}
	
	pActiveBody = body;
	
	if( body ){
		body->AddReference();
		body->SetActive( true );
	}
	
	if( pSky ){
		pSky->NotifyActiveBodyChanged( this );
	}
}



// Targets
////////////

const seControllerTarget &seLayer::GetTarget( deSkyLayer::eTargets target ) const{
	if( target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity ){
		DETHROW( deeInvalidParam );
	}
	return pTargets[ target ];
}

seControllerTarget &seLayer::GetTarget( deSkyLayer::eTargets target ){
	if( target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity ){
		DETHROW( deeInvalidParam );
	}
	return pTargets[ target ];
}

void seLayer::SetActiveTarget( deSkyLayer::eTargets target ){
	if( target == pActiveTarget ){
		return;
	}
	
	pActiveTarget = target;
	if( pSky ){
		pSky->NotifyActiveTargetChanged( this );
	}
}

void seLayer::NotifyTargetChanged( deSkyLayer::eTargets target ){
	if( target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSky ){
		pSky->NotifyTargetChanged( this, target );
	}
}



// Private Functions
//////////////////////

void seLayer::pUpdateSkin(){
	if( pPathSkin.IsEmpty() ){
		pEngSkin = NULL;
		return;
	}
	
	deSkinManager &skinMgr = *pEnvironment.GetEngineController()->GetEngine()->GetSkinManager();
	const char *basePath = "/";
	if( pSky ){
		basePath = pSky->GetDirectoryPath();
	}
	
	try{
		pEngSkin.TakeOver( skinMgr.LoadSkin( pPathSkin, basePath ) );
		
	}catch( const deException &e ){
		if( pSky ){
			pEngSkin = pSky->GetEnvironment()->GetErrorSkin();
		}
	}
}
