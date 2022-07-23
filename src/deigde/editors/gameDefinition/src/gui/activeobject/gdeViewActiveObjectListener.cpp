/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include <stdlib.h>

#include "gdeViewActiveObjectListener.h"
#include "gdeViewActiveObject.h"
#include "../../gamedef/gdeGameDefinition.h"



// Class gdeViewActiveObjectListener
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeViewActiveObjectListener::gdeViewActiveObjectListener( gdeViewActiveObject &view ) :
pView( view ){
}

gdeViewActiveObjectListener::~gdeViewActiveObjectListener(){
}



// Notifications
//////////////////

void gdeViewActiveObjectListener::SelectedObjectChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::BasePathChanged( gdeGameDefinition *gameDefinition ){
	if( pView.GetGameDefinition() != gameDefinition ){
		return;
	}
	
	pView.ClearResources();
	pView.InitSelectedObject();
	//pView.ResetCameraPosition();
}

void gdeViewActiveObjectListener::BaseGameDefinitionsChanged( gdeGameDefinition *gameDefinition ){
	BasePathChanged( gameDefinition );
}



void gdeViewActiveObjectListener::ObjectClassChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pView.GetObjectClass() != objectClass ){
		return;
	}
}

void gdeViewActiveObjectListener::OCBillboardsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCBillboards();
}

void gdeViewActiveObjectListener::OCBillboardChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCBillboard *billboard ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCBillboard( billboard );
}

void gdeViewActiveObjectListener::OCCamerasChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCCameras();
}

void gdeViewActiveObjectListener::OCCameraChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCCamera *camera ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCCamera( camera );
}

void gdeViewActiveObjectListener::OCComponentsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCComponents();
}

void gdeViewActiveObjectListener::OCComponentChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCComponent *component ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCComponent( component );
}

void gdeViewActiveObjectListener::OCComponentTextureChanged(
gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.UpdateOCComponentTexture( component, texture );
}

void gdeViewActiveObjectListener::OCComponentActiveTextureChanged(
gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass, gdeOCComponent *component ){
}

void gdeViewActiveObjectListener::OCComponentTextureNameChanged(
gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture ){
}

void gdeViewActiveObjectListener::OCComponentTexturePropertiesChanged(
gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass,
gdeOCComponent *component, gdeOCComponentTexture *texture ){
}

void gdeViewActiveObjectListener::OCEnvMapProbesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCEnvMapProbes();
}

void gdeViewActiveObjectListener::OCEnvMapProbeChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCEnvMapProbe( envMapProbe );
}

void gdeViewActiveObjectListener::OCLightsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCLights();
}

void gdeViewActiveObjectListener::OCLightChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCLight *light ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCLight( light );
}

void gdeViewActiveObjectListener::OCNavigationBlockersChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCNavBlockers();
}

void gdeViewActiveObjectListener::OCNavigationBlockerChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCNavBlocker( navblocker );
}

void gdeViewActiveObjectListener::OCNavigationSpacesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCNavSpaces();
}

void gdeViewActiveObjectListener::OCNavigationSpaceChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCNavSpace( navspace );
}

void gdeViewActiveObjectListener::OCParticleEmittersChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCParticleEmitters();
}

void gdeViewActiveObjectListener::OCParticleEmitterChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCParticleEmitter( emitter );
}

void gdeViewActiveObjectListener::OCForceFieldsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCForceFields();
}

void gdeViewActiveObjectListener::OCForceFieldChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCForceField *field ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCForceField( field );
}

void gdeViewActiveObjectListener::OCSnapPointsChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCSnapPoints();
}

void gdeViewActiveObjectListener::OCSnapPointChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCSnapPoint *snappoint ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCSnapPoint( snappoint );
}

void gdeViewActiveObjectListener::OCSpeakersChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCSpeakers();
}

void gdeViewActiveObjectListener::OCSpeakerChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCSpeaker *speaker ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCSpeaker( speaker );
}

void gdeViewActiveObjectListener::ActiveObjectClassChanged(
gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() != gdeGameDefinition::eotObjectClass ){
		return;
	}
	
	pView.ClearResources();
	pView.InitSelectedObject();
	pView.ResetCameraPosition();
}

void gdeViewActiveObjectListener::ActiveOCBillboardChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCCameraChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCComponentChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCEnvMapProbeChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCLightChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCNavigationBlockerChanged(
gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCNavigationSpaceChanged(
gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCParticleEmitterChanged(
gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCForceFieldChanged(
gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCSnapPointChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}

void gdeViewActiveObjectListener::ActiveOCSpeakerChanged( gdeGameDefinition *gameDefinition ){
	pOCActiveSubObjectChanged( *gameDefinition );
}



void gdeViewActiveObjectListener::ParticleEmitterChanged( gdeGameDefinition *gameDefinition,
gdeParticleEmitter *particleEmitter ){
	if( gameDefinition->GetSelectedObjectType() != gdeGameDefinition::eotParticleEmitter ){
		return;
	}
	
	pView.ClearResources();
	pView.InitSelectedObject();
}

void gdeViewActiveObjectListener::ParticleEmitterNameChanged( gdeGameDefinition *gameDefinition,
gdeParticleEmitter *particleEmitter ){
}

void gdeViewActiveObjectListener::ActiveParticleEmitterChanged(
gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() != gdeGameDefinition::eotParticleEmitter ){
		return;
	}
	
	pView.ClearResources();
	pView.InitSelectedObject();
	pView.ResetCameraPosition();
}



void gdeViewActiveObjectListener::SkinChanged( gdeGameDefinition *gameDefinition,
gdeSkin *skin ){
	if( gameDefinition->GetSelectedObjectType() != gdeGameDefinition::eotSkin ){
		return;
	}
	
	pView.ClearResources();
	pView.InitSelectedObject();
}

void gdeViewActiveObjectListener::SkinNameChanged( gdeGameDefinition *gameDefinition,
gdeSkin *skin ){
}

void gdeViewActiveObjectListener::ActiveSkinChanged( gdeGameDefinition *gameDefinition ){
	if( gameDefinition->GetSelectedObjectType() != gdeGameDefinition::eotSkin ){
		return;
	}
	
	pView.ClearResources();
	pView.InitSelectedObject();
	pView.ResetCameraPosition();
}



void gdeViewActiveObjectListener::OCTextureChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeOCComponentTexture* ){
	OCTexturesChanged( gameDefinition, objectClass );
}

void gdeViewActiveObjectListener::OCTexturesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	if( pGetSSOObjectClass( *gameDefinition ) != objectClass ){
		return;
	}
	
	pView.RebuildOCComponents();
}



void gdeViewActiveObjectListener::OCPropertyChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass, gdeProperty* ){
	OCPropertiesChanged( gameDefinition, objectClass );
}

void gdeViewActiveObjectListener::OCPropertiesChanged( gdeGameDefinition*, gdeObjectClass *objectClass ){
	if( pView.GetObjectClass() != objectClass ){
		return;
	}
	
	pView.ClearResources();
	pView.InitSelectedObject();
}

void gdeViewActiveObjectListener::OCPropertyValuesChanged( gdeGameDefinition *gameDefinition,
gdeObjectClass *objectClass ){
	OCPropertiesChanged( gameDefinition, objectClass );
}



// Private Functions
//////////////////////

gdeObjectClass *gdeViewActiveObjectListener::pGetSSOObjectClass(
const gdeGameDefinition &gameDefinition ) const{
	switch( gameDefinition.GetSelectedObjectType() ){
	case gdeGameDefinition::eotObjectClass:
	case gdeGameDefinition::eotOCBillboard:
	case gdeGameDefinition::eotOCCamera:
	case gdeGameDefinition::eotOCComponent:
	case gdeGameDefinition::eotOCEnvMapProbe:
	case gdeGameDefinition::eotOCLight:
	case gdeGameDefinition::eotOCNavigationBlocker:
	case gdeGameDefinition::eotOCNavigationSpace:
	case gdeGameDefinition::eotOCParticleEmitter:
	case gdeGameDefinition::eotOCSnapPoint:
	case gdeGameDefinition::eotOCSpeaker:
		return gameDefinition.GetActiveObjectClass();
		
	default:
		return NULL;
	}
}

void gdeViewActiveObjectListener::pOCActiveSubObjectChanged(
const gdeGameDefinition &gameDefinition ){
	gdeObjectClass * const ssoObjectClass = pGetSSOObjectClass( gameDefinition );
	
	if( ssoObjectClass && ssoObjectClass == pView.GetObjectClass() ){
		pView.SelectedSubObjectChanged();
		
	}else{
		pView.ClearResources();
		pView.InitSelectedObject();
		pView.ResetCameraPosition();
	}
}
